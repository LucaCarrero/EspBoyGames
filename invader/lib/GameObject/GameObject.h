#ifndef GameObject_h
#define GameObject_h
#include "Arduino.h"

/*
    Classe di base su cui basare gli oggetti di gioco.
    Estendendo questa classe si ha la possibilità  di usufruire di alcuni metodi relativi alla gestione delle coordinate.
*/
class GameObject {
private:
    /* attributi relativi alle cordinate e alle dimensioni dell'oggetto */
    int x;
    int y;
    int h;
    int b;
public:
    GameObject(int ,int ,int ,int );
    GameObject();
    // metodo da override per castomizzare casa disegnare sul display
     virtual void disegna() = 0; 
    
    //getter degli attributi della classe
    int getX();
    int getY();
    int getH();
    int getB();

    //setter degli attributi della classe
    void setX(int X);
    void setY(int Y);
    void setH(int H);
    void setB(int B);
    void setXY(int,int);

    //metodo per rilevare se il GameObject sul quale viene richiamato collide con un altro passato come parametro.
    int collideWhit(GameObject &r2) ;
};
#endif
