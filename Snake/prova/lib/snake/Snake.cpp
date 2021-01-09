#include <Snake.h>
#include <Pezzo.h>
#include <Arduino.h>
#include<EEPROM.h>

Snake::Snake(){ 
    p[0].init(8,8,4,4);
    lungezza = 1;
    dir = 1;
    randomSeed(analogRead(0));
    punteggio = 0;
    // inizializza il cibo
    cibo.init(random(1, 19)  * 4, random(2, 11)  * 4,4,4);
}
void Snake::start(){
    p[0].init(8,8,4,4);
    lungezza = 1;
    dir = 1;
    randomSeed(analogRead(0));
    perso = false;
    punteggio = 0;
    // inizializza il cibo
    cibo.init(random(1, 19)  * 4, random(2, 11)  * 4,4,4);
}

void Snake::DisegnaS(U8G2 &t){
    // visualizza il punteggio a schermo
    char buffer[7];        
    itoa(punteggio,buffer,10);

    t.drawStr(1,t.getFontAscent(),buffer);
    t.drawLine(0,t.getFontAscent(),t.getDisplayWidth(),t.getFontAscent());
    
    // visualizza il campo
    t.drawFrame(0, t.getFontAscent()+2, t.getDisplayWidth(), t.getDisplayHeight()-(t.getFontAscent()+2));

    // visualizza serpente e cibo
    int i =0;
    for(i=0;i<lungezza;i++){
        p[i].disegna(t);
    }
    cibo.disegna(t);
}

void Snake::setDir(int d){
    if(d != -dir && dir != d && (p[0].getX()<80 && p[0].getX()>=0))
        dir = d;
}

void Snake::move(){
    int i;
    
    //shifta e controlla le collisioni
    for(i=lungezza-1;i>0;i--){
        //collisione
        if(p[i].getX() == p[0].getX() && p[i].getY() == p[0].getY()){
            perso = true;
            EEPROM.begin(512);
            if(punteggio > EEPROM.read(0)){
                EEPROM.write(0,punteggio);
                EEPROM.end();
            }
            return;
        }
        //shift
        p[i].setX(p[i-1].getX());
        p[i].setY(p[i-1].getY());
	}
     
    //azioni da eseguire in caso che il serpente sia a contatto con il cibo
    if(eat()){
        piazzaCibo();
        if(lungezza < LUNG_MAX)
            aggiungiPezzo();
        punteggio++;
	}

    //sposta la testa
    switch(dir){
         case 1: //dx
            if(p[0].getX()-4<80)
                p[0].setX(p[0].getX()+4);
            else
                p[0].setX(0);
         break;
         case -1: //sx
             if(p[0].getX()>=0) 
                p[0].setX(p[0].getX()-4);
            else
                p[0].setX(84-4);
         break;
         case 2:    //giù
            if(p[0].getY()+4<48)
                p[0].setY(p[0].getY()+4);
            else
                p[0].setY(8);
         break;
         case -2: // su
            if(p[0].getY()>= 12)
                p[0].setY(p[0].getY()-4);
            else
                p[0].setY(48-4);
         break;
    }
}

void Snake::piazzaCibo(){
    int randNumber = random(1, 19)  * 4;
	cibo.setX(randNumber);
    randNumber = random(2, 11)  * 4;
	cibo.setY(randNumber);
}

bool Snake::eat(){
	return (p[0].getX() == cibo.getX()) && (p[0].getY() == cibo.getY());
}

void Snake::aggiungiPezzo(){
	lungezza++;
    p[lungezza-1].init(p[lungezza-2].getX(),p[lungezza-2].getY(),4,4);
}
