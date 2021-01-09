#ifndef nemici_h
#define nemici_h
    #include"Arduino.h"
    #include"Personaggio.h"

    #define NUM_RIG 3
    #define NUM_COL 5

    class Nemici{
        private:
            Personaggio nemici[NUM_RIG * NUM_COL];
            int dir = 1;
            int campSize;
            int offset;
            uint8_t enemyAlive;
        public:
            void initNemici(int,int);
            int muovi();
            bool isCompletaC(int);
            int primaCompletaC();
            int ultimaCompletaC();
            void spostaY();
            void spostaX();
            Personaggio * getNemici();
            bool isCompletaR(int i);
            int ultimaCompletaR();
            void enemyDie();
            uint8_t getEnemyAlive();
    };
#endif