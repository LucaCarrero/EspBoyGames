#include"Arduino.h"
#include "Base.h"
#include"GameObject.h"

void Base::init(int x, int y){
    setX(x);
    setY(y);
}

bool Base::setDanno(int x,int y, int dir){

    int xDam = x - getX();
    int yDam = y - getY();

   
    if(!Base::base[yDam][xDam] &&  !Base::base[yDam + dir][xDam]){
        if(Base::base[0][xDam]){
            Base::base[0][xDam] = 0;
        }
        return false;
    }

     if(dir == 1){
        yDam--;
        Base::base[yDam + (dir*2)][xDam] = 0; 
    }

    Base::base[yDam][xDam] = 0;
    Base::base[yDam + dir][xDam] = 0;   
    
    return true;
}

bool Base::getPixel(int x, int y){
    return base[x][y];
}

void Base::resetBase(){
    for(int i =0;i<HEIGHT_BASE;i++){
        for(int j =0;j<HEIGHT_BASE;j++){
            base[i][j] = originalBase[i][j];
        }
    }
}
