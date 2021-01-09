#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
#include <Snake.h>
#include <Menu.h>
#include <PCF8574.h>
#include<EEPROM.h>

// inizializzazione gioco e sistema
U8G2_PCD8544_84X48_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 14, /* data=*/ 13, /* cs=*/ 16, /* dc=*/ 0, /* reset=*/ 2);
Snake s;
PCF8574 pcf8574(0x20); 
int dir = 1;

Menu m;
int t =0;
char buf[7]; 

//gestione degli input
void input(){
  if(pcf8574.digitalRead(P0) == HIGH){ // giù
    dir = 2;
  }
  if (pcf8574.digitalRead(P7) == HIGH) { // su
    dir = -2;
  }
  if (pcf8574.digitalRead(P5) == HIGH) { //dx
    dir = 1;
  }
  if (pcf8574.digitalRead(P3) == HIGH) {// sx
    dir = -1;
  }
}

void setup() {
  // put your setup code here, to run once:
  u8g2.begin();
  Serial.begin(9600);
  
  //dichiarazione dei pin per i tasti
  pcf8574.pinMode(P0, INPUT); // giù
  pcf8574.pinMode(P1, INPUT); // A
  pcf8574.pinMode(P7, INPUT); // su
  pcf8574.pinMode(P5, INPUT); //dx
  pcf8574.pinMode(P3, INPUT); //sx
  pcf8574.pinMode(P2, INPUT); //B

  // inizializzazione extender
  pcf8574.begin();
}

void loop() {
  u8g2.setFont(u8g2_font_4x6_tf);
  if(s.perso == false){
    //rilevazione dell'input e cambio di direzione
    
    if(millis()% 300 == 0) {
      //inizializzazione schermo 
      u8g2.clear();                        
      u8g2.firstPage();

      //parte di calcolo del gioco
      s.setDir(dir);
      s.move(); 
        
      //loop per il disegno solo u8g2
      do {
        s.DisegnaS(u8g2);         
      } while ( u8g2.nextPage() );
    }
    input();
    if (pcf8574.digitalRead(P1) == HIGH) {}
  }
  
  if(t == 0 && s.perso == true){
      u8g2.clear();                        
      u8g2.firstPage();
      if(pcf8574.digitalRead(P0) == HIGH && m.getInd() < 3){ // giù
        m.incIndide();
        delay(5);
      }
      if (pcf8574.digitalRead(P7) == HIGH && m.getInd() > 1) { // su
        m.decIndice();
        delay(5);
      }
      if (pcf8574.digitalRead(P1) == HIGH) { // A conferma
        if(m.getInd() == 1){
          s.start();
          delay(10);
        }else if(m.getInd() == 3){
            t = 2;
            EEPROM.begin(512);
            itoa(EEPROM.read(0),buf,10);
            EEPROM.end();
        }
      }
      do { m.disegnaM(u8g2);} while ( u8g2.nextPage() );
      delay(100);
  }else if(t == 2 && s.perso == true){
    if(millis()% 500 == 0) {
      u8g2.clear();                        
     u8g2.firstPage();
      do { 
        u8g2.drawStr(0,24,"il record e': ");
        u8g2.drawStr(u8g2.getStrWidth("il record e': "),24,buf);
      } while ( u8g2.nextPage() );
    }
    if (pcf8574.digitalRead(P2) == HIGH) {
      t =0;
    }
  }
}