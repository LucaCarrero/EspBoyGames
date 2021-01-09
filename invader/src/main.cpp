#include<Arduino.h>
#include <Personaggio.h>
#include<Nemici.h>
#include<Proiettile.h>
#include<Base.h>
#include "KeyController.h"
#include "BootLoader.h"

#include "U8g2_ID942\src\U8g2lib.h"
#ifdef U8X8_HAVE_HW_SPI
  #include<SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
  #include<Wire.h>
#endif

#define BASE_NUM 4
#define MAX_BULLET_ARRAY 15

#define ENEMY_MOVE_TIME 1000
#define BULLET_P_MOVE_TIME 100
#define BULLET_E_MOVE_TIME 100
#define FIRE_E_INTERVAL_TIME 3000
#define FIRE_P_INTERVAL_TIME 2000
#define STAGE_INCREASE_EN_SPEED 50
#define KILL_E_BASE_POINT 10
#define CHANGE_STAGE_SCREEN 1000

U8G2_PCD8544_84X48_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 14, /* data=*/ 13, /* cs=*/ 16, /* dc=*/ 0, /* reset=*/ 2);
KeyController keyboard;
Personaggio pers;
Base basi[BASE_NUM];
Nemici n;
Proiettile proiettiliPlayer[MAX_BULLET_ARRAY];
Proiettile proiettiliEnemy[MAX_BULLET_ARRAY];

int lastMuveBulletP = 0;
int lastMuveBulletE = 0;
int lastEnemyMouve = 0;
int lastPlayerFire = 0;
int lastEnemyFire = 0;
bool canFire = false;
int visibleBase = 1;

int stage = -1;
int changingStage = 1;
int stageStarted;

int points = 10;

void initWord(){
  stageStarted = millis();
  changingStage = 1;
  pers.setTipo(3);
  pers.setVita(true);
  pers.setVite(3);
  pers.setX(0);
  pers.setY(u8g2.getDisplayHeight() - pers.getH());

  float marginStartEnd = 7;
  float between = (u8g2.getDisplayWidth() - (marginStartEnd * 2)) / BASE_NUM;
  
  for(int i = 0; i < BASE_NUM; i++){
    basi[i].setY(u8g2.getDisplayHeight() - pers.getH() - basi[i].getH() );
    basi[i].setX((marginStartEnd + (between * i)) + ((between/2) - (basi[i].getB() / 2)) );
    basi[i].resetBase();
  }

  n.initNemici(u8g2.getDisplayWidth(), 3);
  lastMuveBulletP = 0;
  lastMuveBulletE = 0;
  lastEnemyMouve = 0;
  lastPlayerFire = 0;
  lastEnemyFire = 0;
  visibleBase = 1;
  if(stage == -1){
    points = 0;
  }
  stage++;
  canFire= false;
}

void input(){
  if(pers.isVivo() && !changingStage){    
    if (keyboard.isRightPressed()) { //dx
      pers.setX(pers.getX()+1);
    }
    if (keyboard.isLeftPressed()) {// sx
      pers.setX(pers.getX()-1);
    }
    if(keyboard.isAPressed()){
      if(canFire){
        canFire = false;
        for (size_t i = 0; i < MAX_BULLET_ARRAY; i++){
          if(!proiettiliPlayer[i].isSparato() ){
            proiettiliPlayer[i].spara(true , pers.getX() +( pers.getH() / 2) + 1,pers.getY(), GIU);
            break;
          }
        }
      } 
    }else{
      if(millis() - lastPlayerFire  >= FIRE_P_INTERVAL_TIME ){
        lastPlayerFire = millis();
        canFire = true;
      }
    }
  }
  if(keyboard.isAPressed() && !pers.isVivo()){
    stage = -1;
    initWord();
  }
} 

void draw(void) {
  u8g2.setFont(u8g2_font_4x6_mf);
 if(pers.isVivo() && !changingStage){      
    u8g2.setCursor(u8g2.getDisplayWidth() - ((u8g2.getStrWidth("0") ) * String(points, DEC).length()) ,  u8g2.getDisplayHeight() );
    u8g2.print(points);

    for(int i = 0; i < pers.getVite() ; i++){
      u8g2.setCursor(  (u8g2.getStrWidth(".")  *  i),  u8g2.getDisplayHeight() - 2);
      u8g2.print(".");
    }
    
    u8g2.drawXBMP(pers.getX(), pers.getY(),pers.getB(),pers.getH(),InvaderMiddleGfx[pers.getTipo()]);

    Personaggio *nemici = n.getNemici();
    for(int i = 0; i < NUM_RIG * NUM_COL ; i++){
      //Serial.println(i);
      if(nemici[i].isVivo())
        u8g2.drawXBMP(nemici[i].getX(), nemici[i].getY(),nemici[i].getH(),nemici[i].getB(), InvaderMiddleGfx[nemici[i].getTipo()]);
    }

    if((nemici[n.ultimaCompletaR() * NUM_COL].getY() + HEIGHT_ALIENO_TIPO) <= basi[0].getY()  && visibleBase){
        for(int x =0; x < BASE_NUM; x++ ){
            for(int i = 0; i < basi[x].getH(); i++){
                for(int j = 0; j < basi[x].getB(); j++){
                  if(basi[x].getPixel(i,j))
                    u8g2.drawPixel(basi[x].getX()+j, basi[x].getY() + i);
                }
            }
        }
    }

    for (size_t i = 0; i < MAX_BULLET_ARRAY; i++){
      if(proiettiliPlayer[i].isSparato()){
        u8g2.drawLine(proiettiliPlayer[i].getX(), proiettiliPlayer[i].getY(), proiettiliPlayer[i].getX(), proiettiliPlayer[i].getY() + UNIT);
      }
    }

    for (size_t i = 0; i < MAX_BULLET_ARRAY; i++){
      if(proiettiliEnemy[i].isSparato()){
        u8g2.drawLine(proiettiliEnemy[i].getX(), proiettiliEnemy[i].getY(), proiettiliEnemy[i].getX(), proiettiliEnemy[i].getY() + UNIT);
      }
    }
 }else{
    if(pers.isVivo()){
      u8g2.setCursor((u8g2.getDisplayWidth() / 2) - (u8g2.getStrWidth("STAGE XXX") / 2),  u8g2.getDisplayHeight() / 2 );
      u8g2.print("STAGE");
      u8g2.setCursor((u8g2.getDisplayWidth() / 2) - (u8g2.getStrWidth("STAGE XXX") / 2) + u8g2.getStrWidth("STAGE ") ,  u8g2.getDisplayHeight() / 2 );
      u8g2.print(stage + 1);
      u8g2.setCursor((u8g2.getDisplayWidth() / 2) - (u8g2.getStrWidth("Vite: X") / 2),  (u8g2.getDisplayHeight() / 2) + u8g2.getFontAscent() + 2);
      u8g2.print("Vite: ");
      u8g2.setCursor((u8g2.getDisplayWidth() / 2) - (u8g2.getStrWidth("Vite: X") / 2) + u8g2.getStrWidth("Vite:"),  (u8g2.getDisplayHeight() / 2) + u8g2.getFontAscent() + 2);
      u8g2.print(pers.getVite());
    }else{
      u8g2.setCursor((u8g2.getDisplayWidth() / 2) - (u8g2.getStrWidth("GAME OVER") / 2),  u8g2.getDisplayHeight() / 2 );
      u8g2.print("GAME OVER");
       u8g2.setCursor((u8g2.getDisplayWidth() / 2) - (u8g2.getStrWidth("Press A to restart") / 2),  (u8g2.getDisplayHeight() / 2) + u8g2.getFontAscent() + 2);
      u8g2.print("press A to restart");
    }
 }
}
 
void update(){  
  // ciclo contemporaneamente i proiettili dei nemici e dei player per ottimizzare la procedura
  Personaggio *nemici = n.getNemici();
  for (size_t i = 0; i < MAX_BULLET_ARRAY; i++){
     
      //se il proiettile del giocatore nella posizione i  è stato sparato
      if(proiettiliPlayer[i].isSparato() ){
        for(int j = NUM_RIG * NUM_COL; j >= 0 ; j--){  //controllo se il proiettile ha colpito un nemico
          if(nemici[j].isVivo() && nemici[j].collideWhit(proiettiliPlayer[i])){ // collide con un nemico
            nemici[j].setVita(false); // uccido il nemico colpito
            proiettiliPlayer[i].esplosion(); // il proiettile esplode
            n.enemyDie(); // decremento il numero di nemici vivi
            points += (KILL_E_BASE_POINT * (stage + 1));
            if(n.getEnemyAlive() <= 0){ // se il numero dei nemici è zero ho vinto e ricomincio in un nuovo stage
               initWord();
            }
            break;
          }
        }
      }

      if((nemici[n.ultimaCompletaR() * NUM_COL].getY() + HEIGHT_ALIENO_TIPO) <= basi[0].getY() && visibleBase){ // se i nemici hanno raggiunto le basi salto questo pezzo
        if(proiettiliEnemy[i].isSparato()){ // contollo le collisioni tra i proiettili nemici e altre entita se il proiettile i è stato sparato
            if(proiettiliEnemy[i].getY() <  basi[0].getY() + 8 && proiettiliEnemy[i].getY() > basi[0].getY() ){ // se il proiettile è nella fascia delle basi
              for(int j =0; j < BASE_NUM; j++){
                if(basi[j].collideWhit(proiettiliEnemy[i])){ // se il proiettile collide con una base
                  if(basi[j].setDanno(proiettiliEnemy[i].getX() , proiettiliEnemy[i].getY() , proiettiliEnemy[i].getDir()) ){ // faccio esplodere il proettile solo se il proiettile collide con un pixel pieno della base
                    proiettiliEnemy[i].esplosion();
                    break;
                  }
              }
            }
          }  
        }

        if(proiettiliPlayer[i].isSparato() ){ // controllo se un proiettile del giocatore colpisce una base
          // se il proiettile e nella fascia delle basi procedo a controllare se collide con qualcuna di esse
          if(proiettiliPlayer[i].getY() <  basi[0].getY() + 8 && proiettiliPlayer[i].getY() > basi[0].getY() ){
            for(int j =0; j < BASE_NUM; j++){
              if(basi[j].collideWhit(proiettiliPlayer[i])){
                if(basi[j].setDanno(proiettiliPlayer[i].getX() , proiettiliPlayer[i].getY() , proiettiliPlayer[i].getDir()) ){
                  proiettiliPlayer[i].esplosion();
                  break;
                }
              }
            }
          }  
        }
      }else{
        visibleBase = 0;
      }

      
      if(pers.collideWhit(proiettiliEnemy[i])){
        proiettiliEnemy[i].esplosion();
        pers.setVite(pers.getVite() - 1);
        stageStarted = millis();
        changingStage = 1;
      }
  }

  if(millis() - lastEnemyMouve >= (ENEMY_MOVE_TIME - (stage * STAGE_INCREASE_EN_SPEED))){ // intervallo tra un movimento dei nemici e il successivo
    lastEnemyMouve = millis();
    n.muovi();
  }

  // muovo dei proiettili del player
  if(millis() - lastMuveBulletP >= BULLET_P_MOVE_TIME){
    lastMuveBulletP= millis();
    for (size_t i = 0; i < MAX_BULLET_ARRAY; i++){
      if(proiettiliPlayer[i].isSparato() )
        proiettiliPlayer[i].viaggia();
    }
  }

  // muovo i proiettili nemici
   if(millis() - lastMuveBulletE >= BULLET_P_MOVE_TIME){
    lastMuveBulletE = millis();
    for (size_t i = 0; i < MAX_BULLET_ARRAY; i++){
      if(proiettiliEnemy[i].isSparato() )
        proiettiliEnemy[i].viaggia();
    }
  }
  
  // sparo random dei nemici
  if(millis() - lastEnemyFire >= FIRE_E_INTERVAL_TIME){
    lastEnemyFire = millis();
    // estraggo linea e colonna del nemico che sparera
    int row = random(0,3); 
    int column = random(0,5);
    Personaggio * en = n.getNemici();
    if(en[(row * NUM_COL) + column].isVivo() ){ // se il nemico estratto e vivo sparo un proiettile partendo dalle sue cordinate
        for (size_t i = 0; i < MAX_BULLET_ARRAY; i++){
          if(!proiettiliEnemy[i].isSparato() ){
            proiettiliEnemy[i].spara(true , en[(row * NUM_COL) + column].getX() +( en[(row * NUM_COL) + column].getH() / 2) + 1, en[(row * NUM_COL) + column].getY() +  en[(row * NUM_COL) + column].getH() , SU);
            break;
          }
        }
    }
  }

  if((nemici[n.ultimaCompletaR() * NUM_COL].getY() + HEIGHT_ALIENO_TIPO) >= u8g2.getDisplayHeight() - 3 ){
    n.initNemici(u8g2.getDisplayWidth(), 3);
    pers.setVite(pers.getVite() - 1);
    stageStarted = millis();
    changingStage = 1;
  }

}

void setup(void) {
  boot();
  keyboard.begin(USE_PCF);
  keyboard.begin(USE_PCF);
  keyboard.begin(USE_PCF);
  u8g2.begin();
  u8g2.setContrast(115);
  initWord();

}
 
void loop(void) {
  // picture loop                   
  u8g2.firstPage();
  if(changingStage && ((millis() - stageStarted) >= CHANGE_STAGE_SCREEN)){
    changingStage = 0;
    return;
  }
  if(!changingStage){
    input();
    update();
  }
  do {
     draw();
  } while( u8g2.nextPage() );   
  //delay(100);
}