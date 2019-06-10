# -*- coding: cp1250 -*-
from turtle import*
from podluzne_obiekty import*
from nieregularne_obiekty import*
from dane import*
import os

class design:

    def __init__(self,area,s=3):
        self.__area=area
        self.__scale=s
        clear()
        self.__area.cout_area(s)
        self.__menu()

    def __menu(self):
        while True:
            os.system('cls')
            print("1. Add new object to your area")
            print("2. Delete an object from your area")
            print("3. Get information about object")
            print("4. Save your project")
            print("5. Return to the main menu")
            x=input()
            if x=="1":
                self.__add_object()
            elif x=="2":
                print("Write name of object")
                n=input()
                self.__area.delete(n)
                clear()
                self.__area.cout_area(self.__scale)
            elif x=="3":
                print("Write name of object")
                n=input()
                self.__area.info(n)
                wait() 
            elif x=="4":
                print("Name your project (for example project.txt)")
                n=input()
                try:
                    self.__area.save_as(n)
                except:
                    print("something went wrong while saving your project")
                    wait()
            elif x=="5":
                break
            else:
                print("Please choose one of the options")
                wait()

    def __set_name(self,s):
        print("How would you like to name your "+s+"?")
        n=input()
        default(self.__scale)
        write(n,False,"center",("Arial",9,"normal"))
        return n

    def __add_object(self):
        while True:
            os.system('cls')
            print ("1. Pipe")
            print ("2. Road")
            print ("3. Cable")
            print ("4. Building")
            print ("5. Green area")
            print ("6. Return to the design menu")
            x=input()
            if x=="1":
                print("choose the contents (watter, gas or sewage)")
                cont=input()
                if cont in pipes_colors.keys():
                    w,c=pipes_colors[cont]
                    width(w*self.__scale)
                    color(c)
                    cdnts=self.__design_object()
                    if len(cdnts)>0 :
                        n=self.__set_name("pipe")
                        print (cdnts)
                        self.__area.add_pipe(pipe(cont,cdnts,n))
                        self.__end_action("pipe")
                else:
                    print("wrong contents")
                    wait()
            elif x=="2":
                print("choose kind of your road (hard or dirt)")
                k=input()
                if k in roads_colors.keys():
                    col=roads_colors[k]
                    print("what thickness will have your road?")
                    try:
                        t=eval(input())
                        width(t*self.__scale)
                        color(col)
                        cdnts=self.__design_object()
                        if len(cdnts)>0:
                            n=self.__set_name("road")
                            self.__area.add_road(road(t,k,cdnts,n))
                            self.__end_action("road")
                    except NameError:
                        print("wrong thicness")
                        wait()
                else:
                    print("wrong kind")
                    wait()
            elif x=="3":
                width(0.3*self.__scale)
                color('red')
                cdnts=self.__design_object()
                if(len(cdnts)>0):
                    n=self.__set_name("cable")
                    self.__area.add_cable(cable(cdnts,n))
                    self.__end_action("cable")
            elif x=="4":
                print("choose destination of your building (education, residental, commercial or public)")
                d=input()
                if d in buildings_colors.keys():
                    default(self.__scale)
                    fillcolor(buildings_colors[d])
                    begin_fill()
                    cdnts=self.__design_object()
                    if  len(cdnts)>0:
                        goto(cdnts[0][0]*self.__scale,cdnts[0][1]*self.__scale)
                        end_fill()
                        n=self.__set_name("building")
                        self.__area.add_building(building(d,cdnts,n))
                        self.__end_action("building")
                else:
                    print("wrong destination")
                    wait()
            elif x=="5":
                width(0.3*self.__scale)
                color('green')
                fillcolor('green')
                begin_fill()
                cdnts=self.__design_object()
                if len(cdnts)>0:
                    goto(cdnts[0][0]*self.__scale,cdnts[0][1]*self.__scale)
                    end_fill()
                    n=self.__set_name("green area")
                    self.__area.add_green_area(green_area(cdnts,n))
                    self.__end_action("green area")
            elif x=="6":
                break         
            else:
                print("Please choose one of the options")
                wait()

    def __end_action(self,s):
        print(s+" added succesfully")
        wait()

    def __click(self,a,b,cdnts):
        goto(a,b)
        pd()
        a=round((a+self.__area.dx)/self.__scale,2)
        b=round((b++self.__area.dy)/self.__scale,2)
        cdnts.append((a,b))
        print (cdnts)

    def __design_object(self):
        print("click on the screen to choose coordinates or write cooridinate int tuple (x,y)")
        print("1. undo")
        print("2. done")
        print("3. break")
        cdnts=[]
        pu()
        onscreenclick(lambda a,b: self.__click(a,b,cdnts),1,False)
        while True:
            x=input()
            if x=="1":
                if(len(cdnts)>0):
                    undo()
                    cdnts[len(cdnts)-1:len(cdnts)]=[]
                    print (cdnts)
                else:
                    print("nothing to undo")
            elif x=="2":
                if(len(cdnts)<3):
                    print("your object must have at lest 3 coordinates")
                else:
                    onscreenclick(f,1,False)
                    return cdnts
            elif x=="3":
                onscreenclick(f,1,False)
                for i in range(len(cdnts)):
                    undo()
                return []
            else:
                try:
                    x=eval(x)
                    if isinstance(x,tuple) and len(x)==2 and isinstance(x[0],(int, float)) and isinstance(x[1],(int, float)):
                        self.__click(x[0]*self.__scale-self.__area.dx,x[1]*self.__scale-self.__area.dy,cdnts)
                    else:
                        print ("Please choose one of the options")
                except:
                    print ("Please choose one of the options")
            
