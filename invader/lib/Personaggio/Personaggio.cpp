#include"Arduino.h"
#include "Personaggio.h"
#include"GameObject.h"

void Personaggio::setTipo(int tipo){
    switch(tipo){
        case(elieno_tipo1):
            Personaggio::t = elieno_tipo1;
            setB(WIDTH_ALIENO_TIPO);
            setH(HEIGHT_ALIENO_TIPO);
        break;
        case(elieno_tipo2):
            Personaggio::t = elieno_tipo2;
            setB(WIDTH_ALIENO_TIPO);
            setH(HEIGHT_ALIENO_TIPO);
        break;
        case(elieno_tipo3):
            Personaggio::t = elieno_tipo3;
            setB(WIDTH_ALIENO_TIPO);
            setH(HEIGHT_ALIENO_TIPO);
        break;
        case(player):
            Personaggio::t = player;
            setB(WIDTH_ALIENO_PLAYER);
            setH(HEIGHT_ALIENO_PLAYER);
        break;
    }
    
}

int Personaggio::getTipo(){
     switch(t){
        case(elieno_tipo1):
            return 0;
        break;
        case(elieno_tipo2):
            return 1;
        break;
        case(elieno_tipo3):
           return 2;
        break;
        case(player):
            return 3;
        break;
    }
}

bool Personaggio::isVivo(){
    if(vite < 0 && t == player){
        Personaggio::vivo = false;
    }
    return Personaggio::vivo;
}

void Personaggio::setVita(bool v){
    Personaggio::vivo = v;
}

void Personaggio::setVite(int v){
    Personaggio::vite = v;
}

 int Personaggio::getVite(){
    return Personaggio::vite;
 }  