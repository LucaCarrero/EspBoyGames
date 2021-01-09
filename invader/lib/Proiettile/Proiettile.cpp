#include"Arduino.h"
#include "Proiettile.h"
#include"GameObject.h"

bool Proiettile::isSparato(){
    return Proiettile::sparato;
}
            
void Proiettile::spara(bool v, int X,int Y,int dire){
    Proiettile::sparato = v;
    setX(X);
    Proiettile::dir = dire;
    setY(Y);
}

void Proiettile::setDir(int d){
    if(d == 1)
        dir = SU;
    if(d == -1)
        dir = GIU;
    if(d == 2)
        dir = DX;
    if(d == -2)
        dir = SX;
}

int Proiettile::getDir(){
     if(dir == 1)
        return SU;
    if(dir == -1)
        return GIU;
    if(dir == 2)
        return DX;
    if(dir == -2)
        return SX;
}

void Proiettile::viaggia(){
  if(abs(dir) == 1 && sparato == true)
  {
      setY(getY() + (dir * UNIT) );
      if(getY() > HEIGHT+UNIT || getY() < 0)
        sparato = false;
  }
}

void Proiettile::esplosion(){
    sparato = false;
    setX(0);
    setY(0);
}