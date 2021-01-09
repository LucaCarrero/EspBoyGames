#ifndef Pezzo_h
#define Pezzo_h

#include "Arduino.h"

#ifndef _U8G2LIB_HH
#include <U8g2lib.h>
#endif

class Pezzo {
private:
    /* data */
    int x;
    int y;
    int h;
    int b;
    
public:
    Pezzo();
    void init(int,int,int,int);
    void disegna(U8G2 &t);
    void moveP(int,int);
    int getX();
    int getY();
    void setX(int X);
    void setY(int Y);
};
#endif


