#ifndef sdUtility_h
#define sdUtility_h

  #include "Arduino.h"
  #include <SPI.h>
  #include <SD.h>
  #include <LinkedList.h>

// classe che descrive il file
  class MyFile {
  private:
    const char* nomeFile;
    const char* path;
  public:
    void setNome(String nome);
    void setPath(String p);
    char * getPath();
    char * getNome();

  };

// classe utility per lavorare più agilmente con l'elenco dei file
// ATTENZIONE: usare con parsimonia libreria molto pesante, grande consumo di memoria
// TODO garbege collector, eliminazione dei file dalla sd
  class SdUtility {
  private:
    LinkedList<MyFile*> fileList;
  public:
    bool init(int cs);
    void loadFileList(File dir, String path);
    File getRoot(); 
    int fileNumber();
    char* getFileExt(const char *string);
    String findFilePath(char * fileName);
    bool loadSketchFromSD(char * fileName);
    char ** getFileList();
    int getNumOfFile();
    bool sdBegin(int cs);
  };
  
#endif