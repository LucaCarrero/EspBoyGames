#include <Menu.h>

Menu::Menu(){
    indice = 1;
}

void Menu::disegnaM(U8G2 &t){
    int tit = ((84/2)-(t.getStrWidth(titolo[0])/2));
    t.drawStr(tit,t.getFontAscent(),titolo[0]);
    t.drawLine(0,t.getFontAscent()+1,t.getDisplayWidth(),t.getFontAscent()+1);

    int h = t.getFontAscent()-t.getFontDescent();
    for(int i = 1; i<4; i++){
      int d = ((84/2)-(t.getStrWidth(s[i-1])/2));
      t.drawStr(d,(i*h*2)+1,s[i-1]);     
    }
    t.drawStr(17,(indice*h*2)+1,">");   
}

void Menu::incIndide(){
  indice++;
}

void Menu::decIndice(){
  indice--;
}

int Menu::getInd(){
  return indice;
}