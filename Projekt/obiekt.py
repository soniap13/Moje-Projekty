# -*- coding: cp1250
from turtle import*
from dane import*

class area_object:

    def __init__(self, c, n):
        self._cdnts=c
        self.name=n

    def cout(self,scale,dx,dy):
        self.set_color_and_width(scale)
        pu()
        for i,j in self._cdnts:
            goto(i*scale-dx, j*scale-dy)
            pd()
        self.final_action(scale,dx,dy)

    def info(self):
        print ("name: "+self.name)
        print ("coordinates: "+str(self._cdnts))
        self.more_info()

    
    def write_name(self, scale, dx, dy):
        default(scale)
        pu()
        goto((self._cdnts[0][0]-dx)*scale,(self._cdnts[0][1]-dy)*scale)
        pd()
        write(self.name,False,"left",("Arial",9,"normal"))
