# -*- coding: cp1250 -*-
from turtle import*
from obszar import*
from podluzne_obiekty import*
from nieregularne_obiekty import*
from projektowanie import*
from dane import*
hideturtle()
title("Design your own area!")
while True:
    os.system('cls')
    print ("Welcame in programm which will help you design your own city or other area")
    print ("1. Load existing project")
    print ("2. Start a new project")
    print ("3. Exit")
    x=input()
    if x=="1":
        print ("Write a name of your project (for example przyklad.txt)")
        name=input()
        try:
            a=load_project(name)
            d=design(a)
        except FileNotFoundError:
            print ("No such file")
            wait()
    elif x=="2":
        print ("Wrtite coordinates of area.")
        print ("For example: [(0, 100),( 100, 0), (0,-100), (-100, 0),( -100, 100)]")
        try:
            c=eval(input())
            if isinstance(c, list) and all([isinstance(x,tuple) and len(x)==2 and isinstance(x[0],(int, float)) and isinstance(x[1],(int, float)) for i in c]):
                if(len(c)<3):
                    print ("Area must have at least 3 coordinates")
                    wait()
                else:
                    a=area(c)
                    d=design(a)
            else:
                print("Wrong coordinates")
                wait()
        except:
            print("Wrong coordinates")
            wait()
    elif x=="3":
        break
    else:
        print ("Please choose one of the options")
        wait()



