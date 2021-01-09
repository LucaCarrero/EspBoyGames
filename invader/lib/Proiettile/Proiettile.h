#ifndef proiettile_h
#define proiettile_h
    #include"Arduino.h"
    #include"GameObject.h"

    #define HEIGHT 48
    #define SU 1
    #define GIU -1
    #define DX 2
    #define SX -2 

    #define UNIT 2

    
    #define ARRIVO 50

    class Proiettile  : public GameObject {
        private:
            bool sparato;
            int dir;
        public:
            Proiettile(int x, int y, int w, int h) : GameObject(x,y,w,h){sparato= false; }        
            Proiettile() : GameObject(){sparato= false;}
            void disegna(){}
            bool isSparato();
            void spara(bool,int,int,int);
            void setDir(int);
            int getDir();
            void viaggia();
            void esplosion();
    };
#endif