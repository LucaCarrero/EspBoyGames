#ifndef Base_h
#define Base_h
    #include"Arduino.h"
    #include"GameObject.h"

    #define HEIGHT_BASE 8
    #define WIDTH_BASE 8

    const byte originalBase[8][8] ={
                {0,0,0,1,1,0,0,0,},
                {0,1,1,1,1,1,1,0,},
                {1,1,1,1,1,1,1,1,},
                {1,1,1,1,1,1,1,1,},
                {1,1,1,1,1,1,1,1,},
                {1,1,1,0,0,1,1,1,},
                {1,1,0,0,0,0,1,1,},
                {0,0,0,0,0,0,0,0,},};

    class Base : public GameObject
    {
        private:
          /*   bool danno[2][3] ={
                {1,0,1},
                {1,0,1}
            };     */    
            bool base[8][8] ={
                {0,0,0,1,1,0,0,0,},
                {0,1,1,1,1,1,1,0,},
                {1,1,1,1,1,1,1,1,},
                {1,1,1,1,1,1,1,1,},
                {1,1,1,1,1,1,1,1,},
                {1,1,1,0,0,1,1,1,},
                {1,1,0,0,0,0,1,1,},
                {0,0,0,0,0,0,0,0,},};
        public:   
            Base(int x, int y, int w, int h) : GameObject(x,y,HEIGHT_BASE,WIDTH_BASE){}        
            Base() : GameObject(){
                setB(WIDTH_BASE);
                setH(HEIGHT_BASE);
            }
            void disegna(){   
            }
            bool getPixel(int x, int y);
            void init(int,int);
            bool setDanno(int,int, int);
            void resetBase();
    };
#endif
