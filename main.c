#include <stdio.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <cairo.h>

#define MAKS_DL_NAZWY_PLIKU 100
#define MAKS_ILOSC_DANYCH 1000000

typedef struct moment{
    float dist,x,y,alt;
    int heart_rate,cadence;
    time_t t;
}moment;

moment dane[MAKS_ILOSC_DANYCH];
float *V,*wyniki;
int n=0,ile=10;
GtkWidget *wykres_Vt,*wykres_Ct,*wykres_HB,*trasa;
static GtkWidget *text, *window, *checkButton1,*checkButton2, *grid,*analizuj_button,*V_info,*description;

extern int czytaj_dane(FILE *file, moment dane[MAKS_ILOSC_DANYCH]);
extern void usun_odbiegajace_wyniki(int n,float V[n],moment dane[MAKS_ILOSC_DANYCH]);
extern void usredniaj_wyniki(int n,float V[n]);
extern float* wykonaj_obliczenia(int ile,moment dane[MAKS_ILOSC_DANYCH],int n,float V[n]);
extern float* licz_predkosci(moment dane[MAKS_ILOSC_DANYCH], int n);
extern char* info(int n,float wyniki[3],moment dane[MAKS_ILOSC_DANYCH]);

void pokazBlad(char *str);
void rysuj_osie(GtkWidget *widget, cairo_t *cr,int x, int y, float dx, float dy,char *opis2,float max);
gboolean rysuj_wykres_Vt(GtkWidget *widget, cairo_t *cr, gpointer data);
gboolean rysuj_wykres_Cad(GtkWidget *widget, cairo_t *cr, gpointer data);
gboolean rysuj_wykres_HB(GtkWidget *widget, cairo_t *cr, gpointer data);
gboolean rysuj_trase(GtkWidget *widget, cairo_t *cr, gpointer data);
static void analizuj(GtkWidget *widget, gchar *input);

int main(int argc,char *argv[])
{
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"Sport Manager");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    g_signal_connect(G_OBJECT(window), "destroy",G_CALLBACK(gtk_main_quit), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10); // grubosc krawedzi bez niczego
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10); // odleglosc miedzy rzedami
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10); // pdleglosc miedzy kolmunami
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_container_add(GTK_CONTAINER(window), grid);

    GtkWidget *instrukcja=gtk_label_new("Tu wprowadź nazwę pliku TCX:");
    gtk_grid_attach(GTK_GRID(grid),instrukcja,0,0,1,1);

    text = gtk_entry_new(); // do wprowadzania wyrazenia
    gtk_entry_set_max_length(GTK_ENTRY(text), MAKS_DL_NAZWY_PLIKU);
    gtk_entry_set_alignment(GTK_ENTRY(text), 1); // wyrównanie do prawej strony
    gtk_entry_set_text(GTK_ENTRY(text), "przyklad1.tcx"); // co się wyswietla na poczatku w okienku
    gtk_grid_attach(GTK_GRID(grid), text, 1, 0, 1, 1); // kolumna,rzad,dlugosc,szeroks

    analizuj_button = gtk_button_new_with_label("Analizuj"); // co sie wyswietla na przyciku
    checkButton1 = gtk_check_button_new_with_label ("Uśredniaj sąsiadujące wyniki");
    checkButton2 = gtk_check_button_new_with_label ("Odrzucaj odbiegające wyniki");
    g_signal_connect(G_OBJECT(analizuj_button), "clicked",G_CALLBACK(analizuj),text);
    gtk_grid_attach(GTK_GRID(grid), analizuj_button, 0, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), checkButton1,0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), checkButton2, 1, 1, 1, 1);

    V_info=gtk_label_new("");
    gtk_grid_attach(GTK_GRID(grid),V_info,2,0,2,18);

    description=gtk_label_new("");
    gtk_grid_attach(GTK_GRID(grid),description,0,3,2,1);

    wykres_Vt=gtk_drawing_area_new();
    g_signal_connect(G_OBJECT(wykres_Vt), "draw", G_CALLBACK(rysuj_wykres_Vt), NULL);
    gtk_grid_attach(GTK_GRID(grid), wykres_Vt, 4, 0, 3, 6);

    wykres_Ct=gtk_drawing_area_new();
    g_signal_connect(G_OBJECT(wykres_Ct), "draw", G_CALLBACK(rysuj_wykres_Cad), NULL);
    gtk_grid_attach(GTK_GRID(grid), wykres_Ct, 4, 6, 3, 6);

    wykres_HB=gtk_drawing_area_new();
    g_signal_connect(G_OBJECT(wykres_HB), "draw", G_CALLBACK(rysuj_wykres_HB), NULL);
    gtk_grid_attach(GTK_GRID(grid), wykres_HB, 4, 12, 3, 6);

    trasa=gtk_drawing_area_new();
    g_signal_connect(G_OBJECT(trasa), "draw", G_CALLBACK(rysuj_trase), NULL);
    gtk_grid_attach(GTK_GRID(grid), trasa, 0, 4, 2, 14);

    gtk_widget_show_all(window);
    gtk_widget_hide(wykres_Vt);
    gtk_widget_hide(wykres_Ct);
    gtk_widget_hide(wykres_HB);
    gtk_widget_hide(trasa);

    gtk_main();
    return 0;
}

void pokazBlad(char *str)
{
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
    GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "%s", str);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void rysuj_osie(GtkWidget *widget, cairo_t *cr,int x, int y, float dx, float dy,char *opis2,float max){
    cairo_set_source_rgb(cr, 0,0,0);
    int i,d = 20;
    char s[8],w[5];
    int odstep=(long long int)dane[0].t/10;
    float dl=odstep;
    if(odstep<60) strcpy(s,"t[sec]");
    else{
        odstep=odstep/60;
        if(odstep<60) {
                strcpy(s,"t[min]");
                dl=odstep*60;
        }
        else {strcpy(s,"t[h]");
            odstep=odstep/60;
            dl=odstep*3600;
        }
    }
    i=0;
    while(i*dl<dane[0].t){
        cairo_move_to(cr,d+i*dl*dx,y+d);
        cairo_line_to(cr,d+i*dl*dx,y+d+5);
        cairo_move_to(cr,d+i*dl*dx+1,y+d+10);
        snprintf(w, 4, "%d", odstep*i);
        cairo_show_text(cr,w);
        i++;
    }
    cairo_move_to(cr,x+d-10,y+d+15);
    cairo_show_text(cr,s);
    i=0;
    odstep=max/5;
    while(i*odstep<1.05*max){
        cairo_move_to(cr,d,y+d-i*odstep*dy);
        cairo_line_to(cr,15,y+d-i*odstep*dy);
        cairo_move_to(cr,1,y+d-i*odstep*dy-1);
        snprintf(w, 4, "%d", odstep*i);
        cairo_show_text(cr,w);
        i++;
    }
    cairo_move_to(cr,10,15);
    cairo_show_text(cr,opis2);
    cairo_move_to(cr,15,25);
    cairo_line_to(cr,d,d);
    cairo_line_to(cr,25,25);
    cairo_move_to(cr,d,d);
    cairo_line_to(cr,d,y+d);
    cairo_line_to(cr,x+d,y+d);
    cairo_line_to(cr,x+d-5,y+d-5);
    cairo_line_to(cr,x+d,y+d);
    cairo_line_to(cr,x+d-5,y+d+5);
    cairo_stroke ( cr );
}

gboolean rysuj_wykres_Vt(GtkWidget *widget, cairo_t *cr, gpointer data){
    int i,d = 20, x = gtk_widget_get_allocated_width(widget)-2*d,
                y = gtk_widget_get_allocated_height(widget)-2*d;
    float dx=(x-2*d)/(float)(long long int)dane[0].t, dy=y/(1.1*wyniki[2]);
    rysuj_osie(widget,cr,x,y,dx,dy,"V[km/h]",wyniki[2]);
    cairo_set_source_rgb( cr, 0, 0, 1);
    cairo_move_to ( cr, d, y-(V[0]*dy)+d);
    for(i=2; i<n-1; i++){
        cairo_line_to (cr, d+((long long int)dane[i].t-(long long int)dane[1].t)*dx, y+d-(V[i-1]*dy));
    }
    cairo_stroke ( cr );
    return FALSE;
}

gboolean rysuj_wykres_Cad(GtkWidget *widget, cairo_t *cr, gpointer data){
    int i,d = 20, x = gtk_widget_get_allocated_width(widget)-2*d,
                y = gtk_widget_get_allocated_height(widget)-2*d;
    float dx=(x-2*d)/(float)(long long int)dane[0].t, dy=y/(1.1*wyniki[9]);
    rysuj_osie(widget,cr,x,y,dx,dy,"Cad[1/min]",wyniki[9]);
    cairo_set_source_rgb( cr, 0, 1, 0);
    cairo_move_to ( cr, d, y-(dane[2].cadence*dy)+d);
    for(i=3; i<=n; i++){
        cairo_line_to (cr, d+((long long int)dane[i].t-(long long int)dane[1].t)*dx, y+d-(dane[i].cadence*dy));
    }
    cairo_stroke ( cr );
    return FALSE;
}

gboolean rysuj_wykres_HB(GtkWidget *widget, cairo_t *cr, gpointer data){
    int i,d = 20, x = gtk_widget_get_allocated_width(widget)-2*d,
                y = gtk_widget_get_allocated_height(widget)-2*d;
    float dx=(x-2*d)/(float)(long long int)dane[0].t, dy=y/(1.1*wyniki[7]);
    rysuj_osie(widget,cr,x,y,dx,dy,"HB[1/min]",wyniki[7]);
    cairo_set_source_rgb( cr, 1, 0, 0);
    cairo_move_to ( cr, d, y-(dane[1].heart_rate*dy)+d);
    for(i=2; i<=n; i++){
        cairo_line_to (cr, d+((long long int)dane[i].t-(long long int)dane[1].t)*dx, y+d-dane[i].heart_rate*dy);
    }
    cairo_stroke ( cr );
    return FALSE;
}

gboolean rysuj_trase(GtkWidget *widget, cairo_t *cr, gpointer data){
    GdkRGBA color;
    int d=20,
        x = gtk_widget_get_allocated_width(widget)-2*d,
        y = gtk_widget_get_allocated_height(widget)-2*d;
    if(x>y) x=y; else y=x;
    float minx=dane[1].x,maxx=dane[1].x,miny=dane[1].y,maxy=dane[1].y;
    for(int i=2; i<=n;i++){
        if(dane[i].x>maxx) maxx=dane[i].x;
        if(dane[i].x<minx) minx=dane[i].x;
        if(dane[i].y>maxy) maxy=dane[i].y;
        if(dane[i].x<miny) miny=dane[i].y;
    }
    float a;
    if(maxx-minx>maxy-miny) a=maxx-minx; else a=maxy-miny;
    float dx=x/a, dy=y/a;
    gtk_style_context_get_color (gtk_widget_get_style_context (widget), 0, &color);
    gdk_cairo_set_source_rgba (cr, &color);
    cairo_move_to ( cr, d+(dane[1].x-minx)*dx, y+(dane[1].y-maxy)*dy+d);
    float c;
    for(int i=2; i<=n; i++){
            c=y+(dane[i].y-maxy)*dy+d;
            cairo_line_to (cr, (dane[i].x-minx)*dx+d,c);
    }
    cairo_stroke ( cr );
    return FALSE;
}

static void analizuj(GtkWidget *widget, gchar *input){
     gtk_widget_hide(wykres_Vt);
     gtk_widget_hide(wykres_Ct);
     gtk_widget_hide(wykres_HB);
     gtk_widget_hide(trasa);
     FILE *file;
     if((file=fopen(gtk_entry_get_text(GTK_ENTRY(input)),"r"))==NULL){
        pokazBlad("Nie udało się otworzyć pliku");
     }else{
         n=czytaj_dane(file,dane);
         if(n==0) pokazBlad("Plik jest niepoprawny");
         else{
            fclose(file);
             V=licz_predkosci(dane,n);
             if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkButton1)))
                usredniaj_wyniki(n,V);
             if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkButton2)))
                usun_odbiegajace_wyniki(n,V,dane);
            wyniki=wykonaj_obliczenia(ile,dane,n,V);
            gtk_label_set_text(GTK_LABEL(V_info),info(n,wyniki,dane));
            gtk_label_set_text(GTK_LABEL(description),"Twoja trasa:");
            gtk_widget_show(wykres_Vt);
            if(dane[0].cadence!=0) gtk_widget_show(wykres_Ct);
            if(dane[0].heart_rate!=0) gtk_widget_show(wykres_HB);
            gtk_widget_show(trasa);
         }
    }
}
