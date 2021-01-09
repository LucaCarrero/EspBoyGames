#ifndef Snake_h
#define LUNG_MAX 100
#define B 3
#define H 3
#define Snake_h
#include <Pezzo.h>
#include <Arduino.h>
#include<EEPROM.h>

class Snake {
private:
    /* data */
    Pezzo p[LUNG_MAX];
    Pezzo cibo;
    int lungezza;
    int dir;
    int punteggio;
    void piazzaCibo();
    bool eat();
public:
    bool perso=true;
    Snake();
    void DisegnaS(U8G2 &t);
    void setDir(int);
    void move();
    void aggiungiPezzo();
    void start();
};

#endif