# -*- coding: cp1250
from turtle import*
from dane import*
from obiekt import*

class long_object(area_object):

    def final_action(self,scale,dx,dy):
        pass

class pipe(long_object):

    def __init__(self, cont, c, n):
        self.__cont=cont
        area_object.__init__(self,c, n)

    def set_color_and_width(self, scale):
        w,c=pipes_colors[self.__cont]
        width(w*scale)
        color(c)

    def more_info(self):
        print ("contents: "+self.__cont)

    def __getinitargs__(self):
        return (self.__cont,self._cdnts,self.name)
        
class cable(long_object):

    def set_color_and_width(self, scale):
        default(scale)

    def __getinitargs__(self):
        return (self._cdnts,self.name)
    

class road(long_object):

    def __init__(self, w, k, c, n):
        area_object.__init__(self, c, n)
        self.__wid=w;
        self.__kind=k;

    def set_color_and_width(self, scale):
        width(scale*self.__wid)
        color(roads_colors[self.__kind])

    def more_info(self):
        print ("kind: "+self.__kind)

    def __getinitargs__(self):
        return (self.__wid,self.__kind,self._cdnts,self.name)
