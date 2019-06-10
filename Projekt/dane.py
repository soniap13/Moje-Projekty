from turtle import*
from pickle import*

pipes_colors={
    "gas":(1,"gray"),
    
    "watter":(2,"blue"),
    "sewage":(3,"brown")
    }

roads_colors={
    "hard" : ("gray"),
    "dirt" :("brown")
    }

buildings_colors={
    "education" : ("khaki"),
    "residental" : ("lightblue"),
    "commercial" : ("plum"),
    "public" : ("peachpuff")
    }

def default(s):
    width(s*0.3)
    color('black')

def load_project(n):
    f=Unpickler(open(n,"rb"))
    return f.load()

def wait():
    print("Press any button to continue")
    wd=input()

def f(a,b):
    pass
