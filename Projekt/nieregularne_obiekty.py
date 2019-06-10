# -*- coding: cp1250 -*-
from turtle import*
from obiekt import*
from dane import*

class irregular_object(area_object):

    def final_action(self,scale,dx,dy):
        goto(self._cdnts[0][0]*scale-dx, self._cdnts[0][1]*scale-dy)
        end_fill()

class green_area(irregular_object):

    def set_color_and_width(self, scale):
        width(scale)
        fillcolor("green")
        color("green")
        begin_fill()

    def __getinitargs__(self):
        return (self._cdnts,self.name)

    

class building(irregular_object):

    def __init__(self, d, c, n):
        area_object.__init__(self, c, n)
        self.__dest=d

    def set_color_and_width(self, scale):
        default(scale)
        fillcolor(buildings_colors[self.__dest])
        begin_fill()

    def more_info(self):
        print ("destination: "+self.__dest)

    def __getinitargs__(self):
        return (self.__dest,self._cdnts,self.name)

    
