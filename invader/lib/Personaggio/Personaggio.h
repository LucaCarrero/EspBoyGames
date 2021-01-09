#ifndef personaggio_h
#define personaggio_h
    #include"Arduino.h"
    #include"GameObject.h"

    #define HEIGHT_ALIENO_TIPO 10
    #define WIDTH_ALIENO_TIPO 8

    #define HEIGHT_ALIENO_PLAYER 5
    #define WIDTH_ALIENO_PLAYER 8

    #define NUM_TIPI 4
    #define SCREEN_HEIGHT 80
    
    enum tipo {elieno_tipo1, elieno_tipo2, elieno_tipo3, player};

    const byte InvaderMiddleGfx[4][60] PROGMEM = {
        {0x30,0x0,0x78,0x0,0xfc,0x0,0xb6,0x1,0xfe,0x1,0x48,0x0,0xb4,0x0,0x4a,0x1,},//elieno_tipo1
        {0x84,0x0,0x49,0x2,0xfd,0x2,0xb7,0x3,0xfe,0x1,0xfc,0x0,0x84,0x0,0x48,0x0,},//elieno_tipo2
        {0x78,0x0,0xfe,0x1,0xff,0x3,0x33,0x3,0xff,0x3,0x48,0x0,0xb4,0x0,0x4a,0x1,},//elieno_tipo3
        {0x18,0x18,0x7e,0xff,0xff,0x00,0x00,},//player
    };

   
    class Personaggio : public GameObject 
    {
        private:
            tipo t;
            bool vivo = true;
            int vite;
        public:
            Personaggio(int x, int y, int w, int h) : GameObject(x,y,h,w){vivo= false; vite = 0; }        
            Personaggio() : GameObject(){vivo= false; }
            void disegna(){}
            void setTipo(int);
            int getTipo();
            bool isVivo();
            void setVita(bool);
            void setVite(int);
            int getVite();        
    };
#endif
