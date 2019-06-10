# -*- coding: cp1250 -*-
from turtle import*
from pickle import*
from dane import*

class area:

    def __init__(self,c, r={}, b={}, k={}, p={}, g={}):
            self.cdnts=c
            self.__roads=r
            self.__buildings=b
            self.__cables=k
            self.__pipes=p
            self.__greenth=g

    def __getinitargs__(self):
        return (self.cdnts,self.__roads,self.__buildings,self.__cables,self.__pipes,self.__greenth)

    def add_road(self,new):
        self.__roads[new.name]=new

    def add_building(self,new):
        self.__buildings[new.name]=new

    def add_cable(self,new):
        self.__cables[new.name]=new

    def add_pipe(self,new):
        self.__pipes[new.name]=new

    def add_green_area(self,new):
        self.__greenth[new.name]=new

    def info(self,n):
        if n in self.__roads.keys():
            print ("ROAD")
            self.__roads[n].info()

        if n in self.__buildings.keys():
            print ("BUILDING")
            self.__buildings[n].info()

        if n in self.__cables.keys():
            print ("CABLE")
            self.__cables[n].info()

        if n in self.__pipes.keys():
            print ("PIPE")
            self.__pipes[n].info()

        if n in self.__greenth.keys():
            print ("GREEN AREA")
            self.__greenth[n].info()

        if n not in self.__roads.keys() and n not in self.__buildings.keys() and n not in self.__cables.keys() and n not in self.__pipes.keys() and n not in self.__greenth.keys():
            print("no such object")

    def save_as(self,n):
        f=Pickler(open(n,"wb"))
        f.dump(self)

    def delete(self,n):
        if n not in self.__roads.keys() and n not in self.__buildings.keys() and n not in self.__cables.keys() and n not in self.__pipes.keys() and n not in self.__greenth.keys():
            print("no such object")
        else:
            self.__pipes.pop(n, None)
            self.__roads.pop(n, None)
            self.__cables.pop(n, None)
            self.__buildings.pop(n, None)
            self.__greenth.pop(n, None)
            print(n+" deleted successfully")
        wait()

         

    def cout_area(self, scale):
        tracer(0,1)
        width(1)
        x=10
        
        min1=min(self.cdnts,key=(lambda x: x[0]))
        min2=min(self.cdnts,key=(lambda x: x[1]))
        max1=max(self.cdnts,key=(lambda x: x[0]))
        max2=max(self.cdnts,key=(lambda x: x[1]))

        color('gray')

        a=((min1[0]-x)*scale,(max2[1]+x)*scale)
        b=((max1[0]+x)*scale,(max2[1]+x)*scale)
        c=((max1[0]+x)*scale,(min2[1]-x)*scale)
        d=((min1[0]-x)*scale,(min2[1]-x)*scale)
        dy=(min2[1]+max2[1])*scale/2
        dx=(min1[0]+max1[0])*scale/2
        self.dx=dx
        self.dy=dy

        while(a>d):
            pu()
            goto(a[0]-dx,a[1]-dy)
            write(a[1]//scale,False,"right",("Arial",3*scale,"normal"))
            pd()
            goto(b[0]-dx,b[1]-dy)
            write(b[1]//scale,False,"left",("Arial",3*scale,"normal"))
            a=(a[0], a[1]-x*scale)
            b=(b[0], b[1]-x*scale)
        pu()
        goto(a[0]-dx,a[1]-dy)
        write(a[1]//scale,False,"right",("Arial",3*scale,"normal"))
        pd()
        goto(b[0]-dx,b[1]-dy)
        write(b[1]//scale,False,"left",("Arial",3*scale,"normal"))
            

        a=((min1[0]-x)*scale,(max2[1]+x)*scale)
        b=((max1[0]+x)*scale,(max2[1]+x)*scale)

        while(b>a):
            pu()
            goto(b[0]-dx,b[1]-dy)
            write(b[0]//scale,False,"left",("Arial",3*scale,"normal"))
            pd()
            goto(c[0]-dx,c[1]-dy)
            pu()
            goto(c[0]-dx,c[1]-x*scale/2-dy)
            write(c[0]//scale,False,"left",("Arial",3*scale,"normal"))
            c=(c[0]-x*scale, c[1])
            b=(b[0]-x*scale, b[1])
        pu()
        goto(b[0]-dx,b[1]-dy)
        write(b[0]//scale,False,"left",("Arial",3*scale,"normal"))
        pd()
        goto(c[0]-dx,c[1]-dy)
        pu()
        goto(c[0]-dx,c[1]-x*scale/2-dy)
        write(c[0]//scale,False,"left",("Arial",3*scale,"normal"))
        
        pu()
        color('black')
        for i,j in self.cdnts:
            goto(i*scale-dx, j*scale-dy)
            pd()
        goto(self.cdnts[0][0]*scale-dx, self.cdnts[0][1]*scale-dy)
        for i in self.__cables.values():
            i.cout(scale,dx,dy)
        for i in self.__pipes.values():
            i.cout(scale,dx,dy)
        for i in self.__roads.values():
            i.cout(scale,dx,dy)
        for i in self.__buildings.values():
            i.cout(scale,dx,dy)
        for i in self.__greenth.values():
            i.cout(scale,dx,dy)
        for i in self.__cables.values():
            i.write_name(scale,dx,dy)
        for i in self.__pipes.values():
            i.write_name(scale,dx,dy)
        for i in self.__roads.values():
            i.write_name(scale,dx,dy)
        for i in self.__buildings.values():
            i.write_name(scale,dx,dy)
        for i in self.__greenth.values():
            i.write_name(scale,dx,dy)
        update()

        
        


