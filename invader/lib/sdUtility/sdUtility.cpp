#include "sdUtility.h"
#include <LinkedList.h>
#include <SPI.h>
#include <SD.h>

// gestione della micro sd

//inizializzo la sd e struttura dati per lavorare con l'elenco dei file
bool SdUtility::init(int cs)
{
  bool ris = sdBegin(cs); //inizializzo la sd

  fileList = LinkedList<MyFile *>(); //inizializzo la lista che conterrà l'elenco dei file
  
  return ris;
}

// begin della comunicazione SPI con la sd
bool SdUtility::sdBegin(int cs){
 if (!SD.begin(cs)) //avvio la sd
  {
    return false;
  }
}
// carica l'elenco dei file presenti nella sd con estensione .bin o .sav
// salvo il file con nome e percorso separati in una classe apposita da me creata per descriverlo come precedentemente descritto
//funzione ricorsiva
void SdUtility::loadFileList(File dir, String path)
{
  if(fileList.size() > 0)
    return;
    
  while (true)
  {

    File entry = dir.openNextFile(); // apro la directory successyva
    if (!entry) // se è vuota esco
    {
      // no more files
      break;
    }

    if (entry.isDirectory()) // se il file è una directory concateno la cartella al persorso
    {
      String temp = String(path + entry.name());
      temp = String(temp + "/");
      loadFileList(entry, temp);
    }
    else  // se è un file:
    {
      char *ext = getFileExt(entry.name()); 
      if (strstr(ext, ".bin") || strstr(ext, ".sav")) // controllo se l'estensione del file è tra quelle accettate
      {
        MyFile *temp = new MyFile(); // creo il descrittore del file
        temp->setPath(path);
        temp->setNome(entry.name());

        fileList.add(temp); // aggiungo il file alla lista
      }
    }
    entry.close();
  }
}

File SdUtility::getRoot() // ottengo la root della sd
{
  return SD.open("/");
}

// funzione per ottenere l'estensione di un file
char *SdUtility::getFileExt(const char *string)
{
  assert(string != NULL);
  char *ext = strrchr(string, '.'); // trovo il primo punto presente nel nome del file

  if (ext == NULL)
    return (char *)string + strlen(string);

  for (char *iter = ext + 1; *iter != '\0'; iter++)  // controllo che dopo il punto siano presenti solo caratteri alfanumerici
  {
    if (!isalnum((unsigned char)*iter))
      return (char *)string + strlen(string);
  }

  return ext;
}

String SdUtility::findFilePath(char *fileName) //dato il nome di un file recupero il percorso dalla lista
{
  for (int i = 0; i < fileList.size(); i++) // scorro la lista in cerca del file
  {
    MyFile cur = *(fileList.get(i));
    String temp = cur.getNome();

    if (strcmp(fileName, temp.c_str()) == 0) // se lo trovo ritorno il percorso
    {
      return cur.getPath();
    }
  }
  return String("");
}

// funzione per fleshare uno sketch precompilato da un file .bin
//viene caricato in una porzione temporanea della flash una volta conclusa sovrascriverà lo sketch esistente
// sketch corrente + sketch da caricare < dimensione totale della flash
bool SdUtility::loadSketchFromSD(char *fileName)
{
  uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;  //calcolo lo spazio dello sketch
  if (Update.begin(maxSketchSpace, U_FLASH)){ // inizializzo lo spazio temporaneo

    File myDataFile = SD.open(fileName, FILE_READ); // apro il file

    while (myDataFile.available()) // leggo con un buffre di 128 byte finchè nel file è presente qualcosa
    {
      uint8_t ibuffer[128];
      myDataFile.read((uint8_t *)ibuffer, 128);
      Update.write(ibuffer, sizeof(ibuffer)); // lo salvo sulla flash
    }
    myDataFile.close(); // chiudo il file
    Update.end(true); // termino il caricamento
    ESP.restart(); // applico le modifiche resettando l'esp
  }
}

// esporta la lista di file in un array di stringe
// ogni file diventa una stringa concatenando percorsodel file con nome del file
char **  SdUtility::getFileList(){ 
  int numberOfFile = fileList.size();
  char ** file = (char **)malloc(numberOfFile * sizeof(char *));

  for(int i = 0; i < numberOfFile; i++){
      MyFile *temp = fileList.get(i);

      const char * path = temp->getPath();
      const char * nameFile =  temp->getNome();
      int strL = strlen(path) + strlen(nameFile) +1;
      file[i] = (char *)malloc(strL * sizeof(char ));
      strcpy(file[i] , path  );
      strcat(file[i],nameFile);
  }
  return file;
}

int SdUtility::getNumOfFile(){ // ottengo il numero di file presenti nell'elenco caricato
  return  fileList.size();
}

//Funzioni della classe che descrive il file

void MyFile::setNome(String nome)
{
  char *temp = (char *)malloc(nome.length() * sizeof(char));
  strcpy(temp, nome.c_str());

  MyFile::nomeFile = temp;
}

void MyFile::setPath(String p)
{
  char *temp = (char *)malloc(p.length() * sizeof(char));
  strcpy(temp, p.c_str());

  MyFile::path = temp;
}

char * MyFile::getPath()
{

  return (char *)MyFile::path;
}

char * MyFile::getNome()
{
  return (char *)MyFile::nomeFile;
}