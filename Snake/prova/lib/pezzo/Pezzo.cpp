#include <Pezzo.h>


Pezzo::Pezzo(){
}

void Pezzo::init(int x,int y,int base,int altezza){
   Pezzo::x = x;
   Pezzo::y=y;
   Pezzo::b = base;
   Pezzo::h = altezza;
}
void Pezzo::disegna(U8G2 &t){
   t.drawFrame(x, y, b, h);
}

void Pezzo::moveP(int nX,int nY){
   setX(nX);
   setY(nY);
}

int Pezzo::getX(){
   return x;
}

int Pezzo::getY(){
   return y;
}

void Pezzo::setX(int X){
   Pezzo::x =X;
}

void Pezzo::setY(int Y){
   Pezzo::y=Y;
}