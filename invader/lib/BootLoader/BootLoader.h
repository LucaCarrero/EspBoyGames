#ifndef Bootloader_h
#define BootLoader_h

#include "Arduino.h"
#include "sdUtility.h"
#include <SPI.h>
#include <SD.h>
#include "ArduinoJson.h" // libreria contenente il parser json per arduino

//TODO ottimizzazione della memoria in caso di sviluppi della libreria
//libreria che ha la funzione di bios,kernel e boot per l'ecosistema

// è obbligatorio usare questa funzione come prima riga di ogni gioco
/*
    quando ci si trova nel firmware di base lo stato (gameLoaded) sarà a 1 ovvero il sistema si aspetta che una volta
    caricato il gioco questo venga eseguito almeno una volta, una volta che un gioco è stato eseguito lo stato (gameLoaded)
    verrà messo a 0 e al sucessivo avvio del gameboy esso ricaricherà il firmware di base permettendone di selezionare un gioco nuovo.

    Sarà obbligatorio che sulla sd sia no presenti i due file (config.conf fil contenente le configutrazioni, boot.bin file contenente il firmware di base compilato per l'esp8266)
*/
void boot(int cs = 1){ 
  
    if(!SD.begin(cs)){ // inizializzo la comunicazion econ la sd
        return; // se fallisce termino
    }

    File config = SD.open("config.conf", FILE_READ); // apro il file con le configurazioni
    File boot = SD.open("boot.bin", FILE_READ); // apro il file con il firmware di base

    if(!config){ // verivico che i file esistano
        return;
    }
    if(!boot){
        return;
    }

    //leggo il contenuyto del file (il contenuto del file salvato in json)
    String configContent = String(""); 

    while (config.available())
    {
      configContent = configContent + (char)config.read();
    } 
    config.close();
   
   StaticJsonDocument<200> doc; // inizializzo il parser json
    DeserializationError err = deserializeJson(doc, configContent.c_str()); // deserializzo il file di configurazione
    config = SD.open("config.conf", "w"); // ri-apro il file delle configurazioni per cambiare lo stato

    if(doc["gameLoaded"] == 1){ // gameLoaded è uguale a 1 quando il gameboy è stato appena fleshato con un gioco
        doc["gameLoaded"] = 0; // lo riporto a 0 per dire che il gioco è stato eseguaito almeno una volta
        
        config.println(doc.as<String>()); // salvo il nuovo stato nel file di configurazione

        return;
    }else{ // gameLoaded è uguale a 1 quando il gioco è già stato eseguito e in seguito a un riavvio si vuole ricaricare il firmware di base che ci permette di selezionare un nuovo gioco
        doc["gameLoaded"] = 1; // cambio lo stato a 1 per dire vhe nessun gioco è caricato
 
        config.println(doc.as<String>()); // salvo lo stato nel file di configurazione

        SdUtility s; // inizializzo la libreria con la gestione delle funzionalita avanzate della sd
        s.loadSketchFromSD("/boot.bin"); // carico il firmare di base(per sapere come lavora qiesta funzione vedere i commenti di questo metodo)

        return; 
    }
    config.close();
    
}
#endif