#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAKS_ILOSC_DANYCH 1000000

typedef struct moment{
    float dist,x,y,alt;
    int heart_rate,cadence;
    time_t t;
}moment;

float num(char *s){
    int w=0;
    double w1=0,d=0.1;
    while(*s=='0') s++;
    while(*s>='0' && *s<='9'){
        w=w*10+(*s-'0');
        s++;
    }
    if(*s=='.'){
       s++;
       while(*s>='0' && *s<='9'){
            w1=w1+(*s-'0')*d;
            d=d*0.1;
            s++;
       }
    }
    return w+w1;
}

time_t czas(char* s){
    struct tm t;
    t.tm_year=(int)num(s)-1900;
    s=s+5;
    t.tm_mon=(int)num(s)-1;
    s=s+3;
    t.tm_mday=(int)num(s);
    s=s+3;
    t.tm_hour=(int)num(s);
    s=s+3;
    t.tm_min=(int)num(s);
    s=s+3;
    t.tm_sec=(int)num(s);
    t.tm_wday=3;
    return mktime(&t);
}

int czytaj_dane(FILE *file, moment dane[MAKS_ILOSC_DANYCH]){
    int i=1,R=6371;
    float lon,lat,pi=3.14159265359;
    char *s=(char*)malloc(100000);
    dane[0].dist=dane[0].t=dane[0].x=dane[0].cadence=dane[0].heart_rate=dane[0].alt=0;
    while(fscanf(file,"%s",s)!=EOF && memcmp("<Lap",s,4)!=0);
    while(memcmp("<Lap",s,4)==0){
        while(fscanf(file,"%s",s)!=EOF && memcmp("<Track>",s,7)!=0){
            if(memcmp("<TotalTimeSeconds>",s,18)==0){
                dane[0].t=dane[0].t+(time_t)num(s+18);
            }
            else if(memcmp("<DistanceMeters>",s,16)==0){
                dane[0].dist=dane[0].dist+num(s+16);
            }
            else if(memcmp("<Calories>",s,10)==0){
                dane[0].x=dane[0].x+num(s+10);
            }
        }
        while(fscanf(file,"%s",s)!=EOF && strcmp("</Track>",s)!=0){
                do{
                    if(memcmp("<LatitudeDegrees>",s,17)==0){
                        lat=num(s+17);
                        fscanf(file,"%s",s);
                        lon=num(s+18);
                    }
                    if(memcmp("<DistanceMeters>",s,16)==0){
                        dane[i].dist=num(s+16);
                    }
                    if(memcmp("<Time>",s,6)==0){
                        dane[i].t=czas(s+6);
                    }
                    if(memcmp("<HeartRateBpm",s,13)==0){
                        fscanf(file,"%s",s);
                        dane[i].heart_rate=num(s+7);
                        dane[0].heart_rate=dane[0].heart_rate+dane[i].heart_rate;
                    }
                    if(memcmp("<AltitudeMeters>",s,16)==0){
                        dane[i].alt=num(s+16);
                        dane[0].alt=dane[0].alt+dane[i].alt;
                    }
                    if(memcmp("<RunCadence>",s,12)==0){
                        dane[i].cadence=num(s+12);
                        dane[0].cadence=dane[0].cadence+dane[i].cadence;
                    }
                dane[i].y=R*cos(lon/180.0*pi)*cos(lat/180.0*pi);
                dane[i].x=R*sin(lon/180.0*pi)*cos(lat/180.0*pi);
                fscanf(file,"%s",s);
                }while(strcmp("</Trackpoint>",s)!=0);
                i++;
        }while(fscanf(file,"%s",s)!=EOF && memcmp("<Lap",s,4)!=0 && i<MAKS_ILOSC_DANYCH);
    }

return i-1;
}

void usun_odbiegajace_wyniki(int n,float V[n],moment dane[MAKS_ILOSC_DANYCH]){
    float kopia[n];
    for(int i=0;i<n-2;i++) kopia[i]=V[i];
    float delta_V,delta_t,a;
    for(int i=1; i<n-2; i++){
        delta_V=abs(kopia[i-1]-kopia[i]);
        delta_t=(dane[i+2].t-dane[i+1].t);
        a=delta_V/delta_t;
        if(a>0.5){
            V[i-1]=(1.05*kopia[i-1]+kopia[i])/2.2;
            V[i]=(kopia[i-1]+1.05*kopia[i])/2.2;
        }

    }
}

void usredniaj_wyniki(int n,float V[n]){
    float kopia[n];
    for(int i=0;i<n-2;i++) kopia[i]=V[i];
    for(int i=1; i<n-2; i++){
        V[i]=(kopia[i]+kopia[i+1]+kopia[i-1])/3;
    }
}

float* licz_predkosci(moment dane[MAKS_ILOSC_DANYCH], int n){
    float *V=(float*)malloc((n-1)*sizeof(float));
    int delta_t,delta_V;
    for(int i=1; i<n-1;i++){
        delta_t=(long long int)(dane[i+1].t-dane[i].t);
        delta_V=dane[i+1].dist-dane[i].dist;
        if(delta_t==0 || delta_V==0) {if(i>1) V[i-1]=V[i-2]; else V[i-1]=0;}
        else V[i-1]=delta_V/(float)delta_t*3.6;
    }
    return V;
}

float* wykonaj_obliczenia(int ile,moment dane[MAKS_ILOSC_DANYCH], int n,float V[n]){
    float *wynik=(float*)malloc(ile*sizeof(float));
    *wynik=dane[0].dist/dane[0].t*3.6;
    float minV=V[0],maxV=V[0];
    float minAlt=dane[1].alt,maxAlt=dane[1].alt;
    float minHB=0,maxHB=0,minCad=0,maxCad=0;
    for(int i=1; i<n-2; i++){
        if(dane[i].alt>maxAlt) maxAlt=dane[i].alt;
        if(dane[i].alt<minAlt) minAlt=dane[i].alt;
        if(dane[i].heart_rate>maxHB) maxHB=dane[i].heart_rate;
        if(dane[i].heart_rate<minHB || minHB==0) minHB=dane[i].heart_rate;
        if(dane[i].cadence>maxCad) maxCad=dane[i].cadence;
        if(dane[i].cadence<minCad || minCad==0) minCad=dane[i].cadence;
        if(i<n-2){
            if(V[i]<minV) minV=V[i];
            if(V[i]>maxV) maxV=V[i];
        }
    }
    wynik[1]=minV;
    wynik[2]=maxV;
    wynik[3]=minAlt;
    wynik[5]=maxAlt;
    wynik[6]=minHB;
    wynik[7]=maxHB;
    wynik[8]=minCad;
    wynik[9]=maxCad;
return wynik;
}

char* info(int n,float wyniki[10],moment dane[MAKS_ILOSC_DANYCH]){
    char* w=(char*)malloc(1000*sizeof(char));
    *w=0;
    char s[8]={0};
    strcat(w,"Czas calkowity: ");
    int h=(long long int)dane[0].t /3600,
        m=(long long int)dane[0].t /60-h*60,
        sec=(long long int)dane[0].t -60*m -3600*h;
    if(h!=0){
        snprintf(s,2,"%d",h);
        strcat(w,s);
        strcat(w," h ");
    }
    if(m!=0){
        snprintf(s,2,"%d",m);
        strcat(w,s);
        strcat(w," min ");
    }
    if(sec!=0){
        snprintf(s,2,"%d",sec);
        strcat(w,s);
        strcat(w," s");
    }
    strcat(w,"\nDlugosc trasy: ");
    snprintf(s,5,"%f",dane[0].dist*0.001);
    strcat(w,s);
    strcat(w," km\n\nPREDKOSC\nMaksymalna: ");
    snprintf(s, 4, "%f", wyniki[2]);
    strcat(w,s);
    strcat(w," km/h\nMinimalna: ");
    snprintf(s, 4, "%f", wyniki[1]);
    strcat(w,s);
    strcat(w," km/h\nSrednia: ");
    snprintf(s, 4, "%f", wyniki[0]);
    strcat(w,s);
    strcat(w," km/h\n\nTempo: ");
    snprintf(s,4,"%f",(float)(long long int)dane[0].t/60/dane[0].dist*1000);
    strcat(w,s);
    strcat(w," min/km");
    if(dane[0].x!=0){
        strcat(w,"\n\nSpalone kalorie: ");
        snprintf(s,4,"%d",(int)dane[0].x);
        strcat(w,s);
        strcat(w," kcal");
    }
    if(dane[0].cadence!=0){
        strcat(w,"\n\nKADENCJA:\nMaksymalna: ");
        snprintf(s,4,"%d",(int)wyniki[9]);
        strcat(w,s);
        strcat(w," na minute\nMinimalna: ");
        snprintf(s,4,"%d",(int)wyniki[8]);
        strcat(w,s);
        strcat(w," na minute\nSrednia: ");
        snprintf(s,4,"%f",dane[0].cadence/(float)n);
        strcat(w,s);
        strcat(w," na minute");
    }
    if(dane[0].alt!=0){
        strcat(w,"\n\nWYSOKOSC\nMaksymalna: ");
        snprintf(s,4,"%d",(int)wyniki[5]);
        strcat(w,s);
        strcat(w," m\nMinimalna: ");
        snprintf(s,4,"%d",(int)wyniki[3]);
        strcat(w,s);
        strcat(w," m\nSrednia: ");
        snprintf(s,4,"%d",(int)(dane[0].alt/(float)n));
        strcat(w,s);
        strcat(w," m");
    }
     if(dane[0].heart_rate!=0){
        strcat(w,"\n\nPULS\nMaksymalny: ");
        snprintf(s,4,"%d",(int)wyniki[7]);
        strcat(w,s);
        strcat(w," uderzen/min\nMinimalny: ");
        snprintf(s,4,"%d",(int)wyniki[6]);
        strcat(w,s);
        strcat(w," m");
        strcat(w," uderzen/min\nSredni: ");
        snprintf(s,4,"%d",dane[0].heart_rate/n);
        strcat(w,s);
        strcat(w," uderzen/min");
    }
    return w;
}
