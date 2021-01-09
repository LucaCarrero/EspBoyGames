#ifndef Menu_h
#define Menu_h

#include "Arduino.h"

#include <U8g2lib.h>

class Menu {
private:
    char *s[10] = {"start" , "livelli", "record"};
    char *titolo[10] = {"Snake"};
    int indice;
public:
    void disegnaM(U8G2 &t);
    Menu();
    void incIndide();
    void decIndice();
    int getInd();
};
#endif