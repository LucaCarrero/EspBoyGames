#include"Arduino.h"
#include "Nemici.h"
#include "Personaggio.h"

void Nemici::initNemici(int campSize,int offSet){
  int cont = 0;
  Nemici::campSize = campSize;
  Nemici::offset = offSet;
  Nemici::enemyAlive = NUM_COL * NUM_RIG;
  for(int j = 0 ; j < NUM_RIG ; j++){
    for(int i = 0 ; i< NUM_COL ; i++){
        nemici[cont].setTipo(j);
        nemici[cont].setY(9*j);
        nemici[cont].setVita(true);
        if(i!=0)
         nemici[cont].setX( offSet + ( (nemici[cont].getH()+1) * i));
        else
          nemici[cont].setX(offSet);                     
        cont++;
    }
  }
}

int Nemici::muovi(){

    int indicePrimo = primaCompletaC();
    int indiceUltimo = ultimaCompletaC();

    Serial.print("prima fila: ");
    Serial.println(indicePrimo);
     Serial.print("ultima fila: ");
    Serial.println(indiceUltimo);

    if(dir == -1 && nemici[indicePrimo].getX() <= Nemici::offset){
        Serial.println("ritorno inizio");
        Nemici::dir = Nemici::dir*-1;
        spostaY();
        return 1;
    }else if(dir == 1 && nemici[indiceUltimo].getX() + nemici[indiceUltimo].getH() >= campSize-offset ) {
         Serial.println("mezzo");
        Nemici::dir = Nemici::dir*-1;
        spostaY();
        return 2;
    }else{
         Serial.println("fine");
        spostaX();
        return 2;
    }
}

int Nemici::primaCompletaC(){
    for(int i = 0;i<NUM_COL;i++){
        if(isCompletaC(i))
            return i;
    }
    return -1;
}

int Nemici::ultimaCompletaC(){
    for(int i = NUM_COL-1;i>0;i--){
        if(isCompletaC(i))
            return i;
    }
    return -1;
}

bool Nemici::isCompletaC(int i){
    if((nemici[i].isVivo() == true)
    || (nemici[i+NUM_COL].isVivo() == true)
    ||(nemici[i+(NUM_COL*2)].isVivo() == true)){return true;}
    return false;
}

void Nemici::spostaX(){
    for(int i =0;i<NUM_COL*NUM_RIG;i++){
        nemici[i].setX(nemici[i].getX() +dir);
    }
}


void Nemici::spostaY(){
    for(int i =0;i<NUM_COL*NUM_RIG;i++){
        nemici[i].setY(nemici[i].getY() + 3);
    }
}

Personaggio * Nemici::getNemici(){
    return nemici;
}

int Nemici::ultimaCompletaR(){
    for(int i = NUM_RIG-1;i>0;i--){
        if(isCompletaR(i))
            return i;
    }
    return -1;
}

bool Nemici::isCompletaR(int i){
    if((nemici[(i * NUM_COL) + 0].isVivo() == true)
    || (nemici[(i * NUM_COL) + 1].isVivo() == true)
    || (nemici[(i * NUM_COL) + 2].isVivo() == true)
    || (nemici[(i * NUM_COL) + 3].isVivo() == true)
    || (nemici[(i * NUM_COL) + 4].isVivo() == true)){return true;}
    return false;
}

void Nemici::enemyDie(){
     Nemici::enemyAlive--;
}

uint8_t Nemici::getEnemyAlive(){
    return Nemici::enemyAlive;
}