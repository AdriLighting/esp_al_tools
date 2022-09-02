/*
https://stackoverflow.com/questions/16982015/getting-const-char-array-from-function
*/

#include "../include/altools.h"

#include <Arduino.h>

#ifdef FILESYSTEM
  #if defined USE_SPIFFS
    #include <FS.h>
  #elif defined USE_LITTLEFS
    #include <LittleFS.h> 
  #endif
#endif

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

#ifdef ALT_DEBUG_TARCE
  DebugPrintList _DebugPrintList;  
#endif

char * al_tools_cstrBuffer = nullptr;

namespace al_tools {
  #ifdef FILESYSTEM
    unsigned int _SPIFFS_printFiles_size;
    void SPIFFS_printFiles(fs::FS &fs, const String & path, JsonObject & obj, JsonArray & folders, boolean display){
      int         totalsize = 0;

      String addF = path;
      if (addF != "/" && addF.substring(0, 1) == "/") addF.remove(0,1);

      JsonObject  root      = obj.createNestedObject(addF);
      JsonArray   arr       = root.createNestedArray(F("items"));
   
      folders.add(addF);
      // Serial.printf("B -> %s\n", path);
      #if defined(ESP8266)
        Dir sub_dir = FILESYSTEM.openDir(path);
        while (sub_dir.next()) {
          if (sub_dir.isDirectory()) {
            // Serial.printf("B isDirectory -> %s\n", sub_dir.fileName());
            SPIFFS_printFiles(fs, path + "/" + sub_dir.fileName(), obj, folders, display);
          }
          else {
              JsonObject var = arr.createNestedObject();     
              var[F("file")] = sub_dir.fileName();
              var[F("size")] = sub_dir.fileSize();   
              totalsize += sub_dir.fileSize();           
          }
        }
        _SPIFFS_printFiles_size += totalsize;
      #elif defined(ESP32)

        Serial.print("SPIFFS_printFiles[S] open DIR : ");
        Serial.println(path);

        File dir = FILESYSTEM.open(path);
        if(!dir){
          Serial.println("- failed to open directory");
          return;
        }
        if(!dir.isDirectory()){
          Serial.println(" - not a directory");
          return;
        }

        File file = dir.openNextFile();
        while(file){
          if(file.isDirectory()){
            
            Serial.print("  DIR : ");
            Serial.println(file.name());

            // if (subFolder) {
              Serial.print(" SPIFFS_printFiles : ");
              Serial.println(path + "/" + file.name());               
              SPIFFS_printFiles(fs, path + "/" + file.name(), obj, folders, display);
            // }
          } else {
            const String name = String(file.name());
            
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());

            JsonObject var = arr.createNestedObject();     
            var[F("file")] = name;
            var[F("size")] = file.size();   
            totalsize += file.size(); 
          }
          file = dir.openNextFile();
        }
        _SPIFFS_printFiles_size += totalsize;
      #endif
    }  
    void SPIFFS_printFiles(const String & path, JsonObject & obj, boolean display, boolean subFolder){
      al_tools::_SPIFFS_printFiles_size = 0;
      int         totalsize = 0;

      JsonObject  root;


      if (path != "/") root = obj.createNestedObject(F("/"));

      String sPath = path;
      if (path.substring(0, 1) != "/") sPath = "/" + path;

      String addF = path;
      if (addF != "/" && addF.substring(0, 1) == "/") addF.remove(0,1);

      if (path != "/") root = root.createNestedObject(addF);
      else root = obj.createNestedObject(addF);

      JsonArray   aFolder   = obj.createNestedArray(F("folders"));
      JsonArray   arr       = root.createNestedArray(F("items")); 

      if (path != "/") aFolder.add("/");
      aFolder.add(addF);
      #if defined(ESP8266)
        Dir dir = FILESYSTEM.openDir(sPath);
        while (dir.next()) {

          if (dir.isDirectory() && subFolder) {
            // Serial.printf("A isDirectory -> %s\n", dir.fileName());
            if (path == "/") al_tools::SPIFFS_printFiles(FILESYSTEM, dir.fileName(), root, aFolder, display);
            else al_tools::SPIFFS_printFiles(FILESYSTEM, path + "/" + dir.fileName(), root, aFolder, display);
          } else  {
             // Serial.printf("isFile -> %s\n", dir.fileName());
              JsonObject var = arr.createNestedObject();          
              var[F("file")] = dir.fileName();
              var[F("size")] = dir.fileSize();   
              totalsize += dir.fileSize();              
          }
        }
        root[F("size")] = totalsize;
        root[F("sizeTotal")] = totalsize+al_tools::_SPIFFS_printFiles_size;
      #elif defined(ESP32)

        Serial.print("SPIFFS_printFiles open DIR : ");
        Serial.println(sPath);

        File dir = FILESYSTEM.open(sPath);
        if(!dir){
          Serial.println("- failed to open directory");
          return;
        }
        if(!dir.isDirectory()){
          Serial.println(" - not a directory");
          return;
        }

        File file = dir.openNextFile();
        while(file){
          if(file.isDirectory()){

            Serial.print("  DIR : ");
            Serial.println(file.name());

            if (subFolder) {
              if (path == "/") {
                Serial.print(" SPIFFS_printFiles : ");
                Serial.println(file.name());                    
                al_tools::SPIFFS_printFiles(FILESYSTEM, path+file.name(), root, aFolder, display);
              }
              else {
                Serial.print(" SPIFFS_printFiles : ");
                Serial.println(path + "/" + file.name()); 
                al_tools::SPIFFS_printFiles(FILESYSTEM, path + "/" + file.name(), root, aFolder, display); 
              }                 
            }
          } else {
            const String name = String(file.name());
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");

            Serial.println(file.size());

            JsonObject var = arr.createNestedObject(); 
            var[F("file")] = name;
            var[F("size")] = file.size();   
            totalsize += file.size();
          }
          file = dir.openNextFile();
        }
        root[F("size")] = totalsize;
        root[F("sizeTotal")] = totalsize+_SPIFFS_printFiles_size;
      #else
      #endif    
    } 
    void SPIFFS_readFile(const String & path){
      Serial.printf_P(PSTR("[read file][%s]\n"), path.c_str()); 
      int nbr = 0;
      File file = FILESYSTEM.open(path, "r");
      if (file) {
        while (file.position()<file.size()) {
          String xml = file.readStringUntil('\n');
          if (xml != "") {
            Serial.printf_P(PSTR("[%-3d] %s\n"), nbr, xml.c_str());
            delay(0);
            nbr++;
          }
        }
        file.close(); 
      }
    }    
    /**
     * @brief      { function_description }
     *
     * @param[in]  in              root folder
     * @param[in]  SerializePrint  pretty print
     * @param[in]  display         print file content
     */
    void SPIFFS_PRINT(const String & in, boolean SerializePrint, boolean display, boolean subFolder) {
      JsonObject obj_1 ;
      JsonArray oPath;

      DynamicJsonDocument doc(5000);
      JsonObject root = doc.to<JsonObject>();
      al_tools::SPIFFS_printFiles(in, root, false, subFolder);
      if (SerializePrint) {serializeJsonPretty(doc, Serial);Serial.println("");}
      Serial.printf_P(PSTR("[AP_SPIFFS_PRINT]\n"));
      JsonArray arr = doc[F("folders")].as<JsonArray>();
      for (size_t i = 0; i < arr.size(); i++) {
        String path = arr[i].as<String>();
        Serial.printf_P(PSTR("[%-3d][%s]\n"), i, path.c_str());
        JsonArray oPath;
        if (path == "/") oPath = doc[path][F("items")].as<JsonArray>();
        else {
          if (in == "/") oPath = doc[F("/")][path][F("items")].as<JsonArray>();
          else {
            String addF = in;
            if (addF.substring(0, 1) == "/") addF.remove(0,1);
            obj_1 = doc[F("/")][addF];
            uint8_t j=0;
            for (JsonObject item : obj_1[F("items")].as<JsonArray>()) {
              String file =  item["file"].as<String>();
              Serial.printf_P(PSTR("\t[%-3d][%s]\n"), j, file.c_str());
              if (display) {
                String filePath = "";
                if (path.substring(0, 1) != "/") filePath = "/" + path + "/" + file; 
                else filePath = path + "/" + file; 
                SPIFFS_readFile(filePath);                
              }
              j++;
            }
            oPath = obj_1[path][F("items")].as<JsonArray>();
          }
        }
        uint8_t j=0;
        for (JsonObject item : oPath) {
          String file =  item["file"].as<String>();
          Serial.printf_P(PSTR("\t[%-3d][%s]\n"), j, file.c_str());
          if (display) {
            String filePath = "";
            if (path.substring(0, 1) != "/") filePath = "/" + path + "/" + file; 
            else filePath = path + "/" + file; 
            SPIFFS_readFile(filePath);                
          }          
          j++;
        }
      }
    } 
   
    void SPIFFS_listFiles(LList<FilePathList * > * ptr, const String & in, boolean subFolder){
      JsonArray oPath;      
      DynamicJsonDocument doc(10000);
      JsonObject root = doc.to<JsonObject>();
      al_tools::SPIFFS_printFiles(in, root, false, subFolder);  
      JsonArray arr = doc[F("folders")].as<JsonArray>();  
      for (size_t i = 0; i < arr.size(); i++) {
        String path = arr[i].as<String>();
        if (path == "/") {
          oPath = doc[path][F("items")].as<JsonArray>();
          // Serial.printf("[%d] oPath: [%s][items]\n", i, path.c_str());
        }
        else {
          if (in == "/") {
            oPath = doc[F("/")][path][F("items")].as<JsonArray>();
            // Serial.printf("[%d] oPath: [/][%s][items]\n", i, path.c_str());
          }
          else {
            if (path.substring(0, 1) != "/") {
              if ("/" + path == in) {
                oPath = doc[F("/")][path][F("items")].as<JsonArray>();
                // Serial.printf("[%d] oPath: [/][%s][items]\n", i, path.c_str());            
              } else {
                String addF = in; addF.remove(0,1);
                oPath = doc[F("/")][addF][path][F("items")].as<JsonArray>();
                // Serial.printf("[%d] oPath: [/][%s][%s][items]\n", i, addF.c_str(), path.c_str());
              }
            }
          }
        }
        uint8_t j=0;
        for (JsonObject item : oPath) {
          String file =  item["file"].as<String>();
          size_t size = oPath[j][F("size")].as<size_t>();
          // String rPath = "";
          String fo = "";
          if (path == "/") {
            // rPath = "/" + file;
            fo = "/";
          } else {
            // rPath = "/" + path + "/" + file;
            fo = "/" + path + "/";
          }
          // Serial.printf("\t[%3d] file: %30s -> %s\n", j, file.c_str(), fo.c_str());
          ptr->add( new FilePathList(fo, file, size));
          j++;
        }          
      }
  }
    void SPIFFS_deleteRecursive(fs::FS &fs, const String &path) {

#ifdef ESP8266
        File file = fs.open(path, "r");
        bool isDir = file.isDirectory();
        file.close();

        if (!isDir) {
            fs.remove(path);
            return;
        }

        Dir dir = fs.openDir(path);

        while (dir.next()) {
            SPIFFS_deleteRecursive(fs, path + '/' + dir.fileName());
            yield();
        }

        fs.rmdir(path);  
#endif
    }    
  #endif
  
  String ch_toString(const char * c) {
    return String((const __FlashStringHelper*) c);
  } 

  boolean c_str(char * & ptr, const String & str) {
    uint16_t length = str.length() + 1;

    if (ptr) delete ptr; 

    ptr = new char[length];
    if(!ptr) {
      return false;
    }
    strcpy(ptr, str.c_str());
    return true;
  }
  boolean c_str(char * & ptr, const char * & str) {
    if(!str) {
      return false;
    }
        
    uint16_t length = strlen(str) + 1;

    if (ptr) delete ptr; 

    ptr = new char[length];
    if(!ptr) {
      return false;
    }
    strcpy(ptr, str);
    return true;
  } 
  boolean c_str(char * & ptr, const char * const & str) {
    if(!str) {
      return false;
    }
        
    uint16_t length = strlen(str) + 1;

    if (ptr) delete ptr; 

    ptr = new char[length];
    if(!ptr) {
      return false;
    }
    strcpy(ptr, str);
    return true;
  }    
  char * c_str(const String & str) {
    uint16_t length = str.length() + 1;

    if (al_tools_cstrBuffer) delete al_tools_cstrBuffer; 

    al_tools_cstrBuffer = new char[length];
    if(!al_tools_cstrBuffer) {
      return nullptr;
    }
    strcpy(al_tools_cstrBuffer, str.c_str());
    return al_tools_cstrBuffer;
  }

  void millis2time_m(const uint64_t & s, char * time){
  uint32_t milliseconds   =       (s / 1000) % 1000;
  uint32_t seconds        = ((    (s / 1000) - milliseconds)/1000)%60;
  uint32_t minutes        = ((((  (s / 1000) - milliseconds)/1000) - seconds)/60) %60;
  uint32_t hours          = ((((( (s / 1000) - milliseconds)/1000) - seconds)/60) - minutes)/60;  
    sprintf(time,"%lu:%02lu:%02lu:%02lu", (unsigned long)hours, (unsigned long)minutes , (unsigned long)seconds, (unsigned long)milliseconds);

  }
  void millis2time_d(const uint32_t & s, char * time) {
    uint32_t seconds    = (s /   (1000)           ) % 60    ;
    uint32_t minutes    = (s /   (1000UL*60UL)    ) % 60    ;
    uint32_t hours      = (s /   (1000UL*3600UL)  ) % 24    ;
    uint32_t days       = (s /   (1000UL*3600UL*24UL)  )    ;
    sprintf(time,"%lu-%02lu:%02lu:%02lu", (unsigned long)days, (unsigned long)hours , (unsigned long)minutes, (unsigned long)seconds);
  }
  void millis2time_h(unsigned long s, char * time) {
    uint32_t seconds    = (s /   (1000)           ) % 60    ;
    uint32_t minutes    = (s /   (1000UL*60UL)    ) % 60    ;
    uint32_t hours      = (s /   (1000UL*3600UL)  ) % 24    ;
    sprintf(time,"%02lu:%02lu:%02lu", (unsigned long)hours , (unsigned long)minutes, (unsigned long)seconds);
  }

  void on_time_h(uint32_t time, String & result) {
     char t[12];
     millis2time_h(time, t);
     result = String(t);
  }  
  void on_time_h(String & result) {
     char t[12];
     millis2time_h(millis(), t);
     result = String(t);
  }
  void on_time_m(String & result) {
     char t[12];
     millis2time_m(millis(), t);
     result = String(t);
  }
  void on_time_m(const uint64_t & s, String & result) {
     char t[12];
     millis2time_m(s, t);
     result = String(t);
  }  
  void on_time_d(String & result) {
     char t[14];
     millis2time_d(millis(), t);
     result = String(t);
  } 
  void on_time_d(uint32_t time, String & result) {
     char t[14];
     millis2time_d(millis(), t);
     result = String(t);
  } 
  /**
   * @brief      split string with sep
   *
   * @param[in]  s      source String
   * @param[in]  sep    separator to split
   * @param      rSize  size of array by ref
   *
   * @return     array of split values
   */
  void explode(const String & s, char sep, int & rSize, String * list) {
            rSize       = -1;
    String  t           = s + sep;
    int     str_index   = 0;
    int     list_index  = 0;
    int     j;
    String  sub;
    int     size        = t.length();

    str_index   =0;
    list_index  =0;
    while (str_index < size ) {

      j = t.indexOf(sep,str_index);
      if (j == -1) continue;

      sub = t.substring(str_index, j);
      if (sub == s) break;

      if (list) list[list_index]=sub;

      list_index++;
      str_index = j + 1;
    }

    if (list) list[list_index] = "";
    rSize = list_index;
  } 
  
  const char** explode(const String & s, char sep, int & rSize) {
            rSize       = -1;
    String  t           = s + sep;
    int     str_index   = 0;
    int     list_index  = 0;
    int     size        = t.length();
    int     j;
    String  sub;
     
    while (str_index < size ) {
      j = t.indexOf(sep,str_index);
      if (j == -1) continue;
      if (t.substring(str_index, j) == s) break;
      list_index++;
      str_index = j + 1;
    }
    if (list_index == 0) {
      rSize = -1;
      return nullptr;
    }

    const char** list = new const char*[list_index+1]; 
    str_index   = 0;
    list_index  = 0;

    while (str_index < size ) {

      j = t.indexOf(sep,str_index);
      if (j == -1) continue;

      sub = t.substring(str_index, j);

      char * buff = new char[sub.length()+1];
      strcpy(buff, sub.c_str());

      list[list_index]  = strdup(buff);
      str_index         = j + 1;
      list_index++;

      delete buff;
    }

    list[list_index]  = "";
    rSize             = list_index;

    return list;
  }       
}


uint32_t HeapStatu::pInitHeap(0);

HeapStatu::HeapStatu() {mod = true;};

void HeapStatu::update(){
  if (mod)  {if (pInitHeap  > ESP.getFreeHeap()) tused = (pInitHeap - ESP.getFreeHeap());}
  else      {if (initHeap   > ESP.getFreeHeap()) tused = (initHeap  - ESP.getFreeHeap());}

  tcnt++;
  if (tused > 0) ttot+=tused;  
}
void HeapStatu::print(String & ret){
  char buffer[120];
  uint32_t heap;
  if (mod)  heap = pInitHeap;
  else      heap = initHeap;
  sprintf(buffer, "heap-used:%7d [%-8d/%-4d|%-7d] init:%7d - free:%7d - mod: %d",
    tused,
    ttot,
    tcnt,
    ((ttot/tcnt)>0)?ttot/tcnt:0,
    heap,
    ESP.getFreeHeap(),
    mod
  );
  ret = String(buffer);   
} 


#ifdef FILESYSTEM
  bool AP_deserializeFile(DynamicJsonDocument& doc, const char* filepath){
      if (!filepath || !*filepath)
          return false;

      File jfile = FILESYSTEM.open(filepath, "r");
      DeserializationError error;
      if (jfile){
          error = deserializeJson(doc, jfile);
          jfile.close();
      } else {
          return false;
      }

      if (error) {
          return false;
      }
      return true;
  }  
#endif


/*  
  LList<SplitItem *>  _SplitItem;
  splitText("&c1:v1&c2:v2", "&",  ':', &_SplitItem);
  for(int i = 0; i < _SplitItem.size(); ++i) {
    SplitItem * ptr = _SplitItem.get(i);
    Serial.printf("[%d][c: %s][v: %s]", i, ptr->_cmd, ptr->_value);
  }
  while (_SplitItem.size()) {
    SplitItem *ptr = _SplitItem.shift();
    delete ptr;
  }
  _SplitItem.clear();     
*/
void splitText(const String & inputString, const char* const & arg,  char sep, LList<SplitItem * > * ptr) {
  char inputChar[inputString.length() + 1] ;
  inputString.toCharArray(inputChar, inputString.length() + 1);

  char * command  = strtok(inputChar, arg);
  while (command != 0){          
    char* separator  = strchr(command, sep);
    if (separator != 0) {
      *separator  = 0;            
      ++separator ;
      ptr->add( new SplitItem(String(command), String(separator)));
    }
    command = strtok(0, arg); 
  }
}
  
#ifdef ALT_DEBUG_TARCE

char * ALT_debugBuffer = nullptr;


uint32_t DebugPrintItem_maxlen_1 = 0;
uint32_t DebugPrintItem_maxlen_2 = 0;

boolean ALT_debugPrint(const char * _id) { 
  if (!_DebugPrintList.get_item(_id)) return false;
  if (_DebugPrintList.get_item(_id)->is_macro()) 
    return true; 
  else 
    return false; 
} 

void ALT_debugPrint(const String & msg, const String & file, const String & line, const String & func) {
  Serial.printf_P(PSTR("[%s:%s] %s\n"), file.c_str() , line.c_str() , func.c_str());
  if (msg!="") Serial.printf_P(PSTR("%s"), msg.c_str() ); 
}
void ALT_debugPrint(const String & msg, const String & file, const String & line, const String & func, const char * _id, ALT_DEBUGLVL_T mod) {
  if (_id == (const char *)"") return;

  DebugPrintItem * ptr = _DebugPrintList.get_item(_id);
  if (!ptr) return;
  
  if (!ptr->is_macro()) {/*Serial.printf_P(PSTR("%s"), pMsg.c_str());*/ return;}

  String pMsg = msg;

  if (pMsg!="" && pMsg.substring(0, 1) == "&"){
    LList<SplitItem *>  _SplitItem;
    splitText(pMsg, "&",  ':', &_SplitItem);
    if (_SplitItem.size()>0) {
      if (strcmp( _SplitItem[0]->_value, "1") == 0) {
        Serial.printf_P(PSTR("%s"), _SplitItem[1]->_value); 
        while (_SplitItem.size()) {
          SplitItem *ptr = _SplitItem.shift();
          delete ptr;
        }
        _SplitItem.clear();            
        return; 
      }
    }
  }
  char * b_heap = nullptr;
  if (ptr->is_heap()) {
    String heap = String(ESP.getFreeHeap());
    b_heap = new char[heap.length()+1];
    strcpy(b_heap, heap.c_str());
  }

  char * b_timeStamp = nullptr;
  if (ptr->is_timeStamp()) {
    char t[12];
    al_tools::millis2time_d(millis(), t);  
    b_timeStamp = new char[12];
    sprintf(b_timeStamp, "%11s", t);
    // strcpy(b_timeStamp, t);
  }

  char * b_file = nullptr;
  if (ptr->is_file()) {
    String  fileName = file;
    int     rSize = 0;
    const char** split = al_tools::explode(file, '/', rSize);
    if (split) {
      fileName = al_tools::ch_toString(split[rSize-1]);
      for(int i = 0; i < rSize; ++i) {
        delete split[i];
      }
      delete[] split;      
    }
    split = al_tools::explode(file, '\\', rSize);
    if (split) {
      fileName = al_tools::ch_toString(split[rSize-1]);
      for(int i = 0; i < rSize; ++i) {
        delete split[i];
      }
      delete[] split;      
    }    
    b_file = new char[fileName.length()+1];
    strcpy(b_file, fileName.c_str());
  }

  char * b_line = nullptr;
  if (ptr->is_line()) {
    String lineSpace = line;
    uint8_t lineSpaceSize = lineSpace.length();
    while (lineSpaceSize<3){lineSpace +=" "; lineSpaceSize = lineSpace.length();}  
    b_line = new char[lineSpace.length()+1];
    sprintf_P(b_line, PSTR("%s"), lineSpace.c_str());
  }  

  char * b_func = nullptr;
  if (ptr->is_func()) {

    String  funcName = "";
    int     rSize = 0;

    const char** split = al_tools::explode(func, '(', rSize);
    if (split) {

      funcName = al_tools::ch_toString(split[0]);
      for(int i = 0; i < rSize; ++i) {
        delete split[i];
      }
      delete[] split;
      split = nullptr;

      split = al_tools::explode(funcName, ' ', rSize);
      if (split) {
        if (rSize > 0) {
          funcName = split[1];
        } else if (rSize == 0) {
          funcName = split[0];
        }
        for(int i = 0; i < rSize; ++i) {
          delete split[i];
        }
        delete[] split;
      }

      b_func = new char[funcName.length()+1];
      sprintf_P(b_func, PSTR("%s"), funcName.c_str());      
    }
  }

  char * full = new char[1024];

  strcpy(full, "");

  if (b_timeStamp) {strcat(full, "["); strcat(full, b_timeStamp); strcat(full, "]"); strcat(full, " ");}

  if (b_heap) {strcat(full, "["); strcat(full, b_heap); strcat(full, "]"); strcat(full, " ");}

  if (b_line || b_file) strcat(full, "[");
  if (b_line) strcat(full, b_line);
  if (b_file && b_line) strcat(full, ":");
  if (b_file) strcat(full, b_file);
  if (b_line || b_file) {strcat(full, "]");strcat(full, " ");}

  if (b_file)       delete b_file;
  if (b_line)       delete b_line;
  if (b_timeStamp)  delete b_timeStamp;
  if (b_heap)       delete b_heap;

  
  if (pMsg=="") {
    strcat(full, "\n");
    Serial.printf_P(PSTR("%s"), full);
  } else {
    if (ptr->is_crmsg()) {
      Serial.printf_P(PSTR("%s\n%s"), full, pMsg.c_str());
    } else {
        String    s     = String(full);
        uint32_t  size  = s.length();
        if (DebugPrintItem_maxlen_1 < s.length() ) DebugPrintItem_maxlen_1 = s.length() ;
        while (size < DebugPrintItem_maxlen_1) {s += " "; size = s.length();}  
        if (b_func) {
          String s2 = String(b_func) + " ";
          size  = s2.length();
          if (DebugPrintItem_maxlen_2 < s2.length()+1 ) DebugPrintItem_maxlen_2 = s2.length()+1 ;
          while (size < DebugPrintItem_maxlen_2) {s2 += " "; size = s2.length();}  
          Serial.printf_P(PSTR("%s%s%s"), s.c_str(), s2.c_str(), pMsg.c_str());

        } else Serial.printf_P(PSTR("%s%s"), s.c_str(), pMsg.c_str()); 
    }      
  }
  if (full)   delete full;
  if (b_func) delete b_func;
}

ALT_DEBUGLVL_T ALT_DEBUGLVLARR_T[] = {
  ALT_DEBUGLVL_HIGHT,      
  ALT_DEBUGLVL_MEDIUM,
  ALT_DEBUGLVL_LOW,
  ALT_DEBUGLVL_NORMAL     

};

DebugPrintItem::DebugPrintItem(const char * id){
  if (!_id) _id = new char[strlen_P(id)+1];
  strcpy(_id,  id);
};
DebugPrintItem::~DebugPrintItem(){};

void DebugPrintItem::print(){ 
    Serial.printf_P(PSTR("[%18s][%7d][%7d][%7d][%7d][%7d][%7d][%7d][%7s]\n"), 
      _id,
      is_macro(), 
      is_timeStamp(), 
      is_heap(), 
      is_line(), 
      is_file(), 
      is_func(), 
      is_crmsg(),
      ALTPT_DEBUGREGIONML_ALL[_lvl]
      );
}

void DebugPrintItem::set_macro(boolean v1)      {_p_macro       = v1;}
void DebugPrintItem::set_timeStamp(boolean v1)  {_p_timeStamp   = v1;}
void DebugPrintItem::set_heap(boolean v1)       {_p_heap        = v1;}
void DebugPrintItem::set_line(boolean v1)       {_p_line        = v1;}
void DebugPrintItem::set_file(boolean v1)       {_p_file        = v1;}
void DebugPrintItem::set_func(boolean v1)       {_p_func        = v1;}
// void DebugPrintItem::set_arg(boolean v1)        {_p_arg         = v1;}
void DebugPrintItem::set_crmsg(boolean v1)      {_p_crmsg       = v1;}
void DebugPrintItem::set_lvl(uint8_t v1)        {_lvl           = ALT_DEBUGLVLARR_T[v1];}


boolean DebugPrintItem::is_macro()              {return _p_macro;}
boolean DebugPrintItem::is_timeStamp()          {return _p_timeStamp;} 
boolean DebugPrintItem::is_heap()               {return _p_heap;}
boolean DebugPrintItem::is_line()               {return _p_line;}
boolean DebugPrintItem::is_file()               {return _p_file;}
boolean DebugPrintItem::is_func()               {return _p_func;}
// boolean DebugPrintItem::is_arg()                {return _p_arg;}
boolean DebugPrintItem::is_crmsg()              {return _p_crmsg;}

ALT_DEBUGLVL_T DebugPrintItem::get_lvl()         {return _lvl;}


DebugPrintList::DebugPrintList(){};
DebugPrintList::~DebugPrintList(){};
DebugPrintItem * DebugPrintList::add(const char * const & id){ 
  _list.add(new DebugPrintItem(id));uint8_t pos = _list.size()-1;return _list[pos];
}
DebugPrintItem * DebugPrintList::get_item(const char *  v1){
  char buffId[strlen_P(v1)+1];
  strcpy(buffId,  v1);
  for(int i = 0; i < _list.size(); ++i) {
    DebugPrintItem * item = _list.get(i);
    if (strcmp(buffId, item->_id) == 0) {return item;}          
  }
  return nullptr;
} 

void DebugPrintList::ketboardPrintHeader(boolean pNbId){
  if (!pNbId)   Serial.printf_P(PSTR("[%18s]"), "_id");
  else          Serial.printf_P(PSTR("[%3s][%18s]"), "nb", "_id");
  uint8_t size = ARRAY_SIZE(APPT_DEBUGREGIONMS_ALL);
  for(uint8_t i = 0; i < size; ++i) {
    Serial.printf_P(PSTR("[%d %5s]"),i, APPT_DEBUGREGIONMS_ALL[i] );
  }   
  Serial.println();          
}
void DebugPrintList::ketboardPrint(){
  Serial.printf_P(PSTR(";&region_id=&setter_id:value\n"));
  Serial.printf_P(PSTR(";&region_id=&range:start,end,value\n"));
  Serial.printf_P(PSTR(";&range:start,end=&setter:value\n"));
  Serial.printf_P(PSTR(";&range:start,end=&range:start,end,value\n"));
  ketboardPrintHeader(true);
  for(uint8_t i = 0; i < _list.size(); ++i) {Serial.printf_P(PSTR("[%3d]"), i);DebugPrintItem * item = _list.get(i);item->print();}
  Serial.println();
}


/*

pattern 
&cmd=&value
&cmd=&value:value_v1
&cmd:cmd_v1,cmd_v2=&value:value_v1,value_v2,value_v3

----------CMD----------  -------------VALUE-------------
;&main                  =&0:0  
& _id                   = & postion : valeur
;&main                  =&range:0,7,0  
& _id                   = & range : start , end , valeur  

;&range:0,7             =&0:0
& range : start , end   = & postion : valeur     
;&range:0,7             =&range:0,7,0 
& range : start , end   = & range : start , end , valeur 

            !!! PAS DE CONTRA SUE LES RANGE DONC FIAR GAFFE AU VALEUR !!!

*/
void  DebugPrintList::keyboardSet(const String & cmd, const String & value){
  Serial.printf_P(PSTR("[keyboardSet] cmd[%s] value[%s]\n"), cmd.c_str(), value.c_str());

  String              sCmd  = cmd;
  int                 rSize = 0;
  LList<SplitItem *>  _SplitItem;

  splitText(sCmd, "&",  ':', &_SplitItem);
  for(int i = 0; i < _SplitItem.size(); ++i) {
    if (strcmp_P(_SplitItem[i]->_cmd, APPT_DEBUGREGIONMC_001) == 0) {

      const char** split = al_tools::explode(String(_SplitItem[i]->_value), ',', rSize);

      if (!split)     continue; 
      if (rSize < 2)  continue;

      ketboardPrintHeader();
      for(int j = atoi(split[0]); j < atoi(split[1]); ++j) {
        DebugPrintItem * item = _list.get(j);
        keyboardSet(item, value);
      }
      for(int i = 0; i < rSize; ++i) {
        delete split[i];
      }
      delete[] split;      
    }
  }

  if (_SplitItem.size()==0) sCmd = sCmd.substring(1, sCmd.length());

  while (_SplitItem.size()) {
    SplitItem *ptr = _SplitItem.shift();
    delete ptr;
  }
  _SplitItem.clear();   


  DebugPrintItem * item = get_item(sCmd.c_str());
  keyboardSet(item, value);

  DebugPrintItem_maxlen_1 = 0;
  DebugPrintItem_maxlen_2 = 0;
} 
void DebugPrintList::keyboardSet(DebugPrintItem * item, const String & value){
  if (!item) return;
  // Serial.printf_P(PSTR("[keyboardSet V2] value[%s]\n"), value.c_str());

  uint8_t aPos  = 0;
  uint8_t vPos  = 0;
  int     rSize = 0;

  LList<SplitItem *> _SplitItem;
  splitText(value, "&",  ':', &_SplitItem);

  for(int i = 0; i < _SplitItem.size(); ++i) {
    if (strcmp_P(_SplitItem[i]->_cmd, APPT_DEBUGREGIONMC_001) == 0) {

      const char** split = al_tools::explode(String(_SplitItem[i]->_value), ',', rSize);

      if (!split)     continue;
      if (rSize < 3)  continue;

      for(int j = atoi(split[0]); j < atoi(split[1]); ++j) {
        keyboardSet(item, j, atoi(split[2]));
      }
      for(int i = 0; i < rSize; ++i) {
        delete split[i];
      }
      delete[] split;   
    
    } else {
      aPos = atoi(_SplitItem[i]->_cmd);
      vPos = atoi(_SplitItem[i]->_value);
      keyboardSet(item, aPos, vPos);        
    }
  }

  while (_SplitItem.size()) {
    SplitItem *eff = _SplitItem.shift();
    delete eff;
  }
  _SplitItem.clear();

  item->print();       
}  
void DebugPrintList::keyboardSet(DebugPrintItem * item, uint8_t aPos, uint8_t vPos, boolean reset){ 
  switch (aPos) {
    case 0: item->set_macro(vPos);      break;
    case 1: item->set_timeStamp(vPos);  break;
    case 2: item->set_heap(vPos);       break;
    case 3: item->set_line(vPos);       break;
    case 4: item->set_file(vPos);       break;
    case 5: item->set_func(vPos);       break;
    case 6: item->set_crmsg(vPos);      break;
    case 7: item->set_lvl(vPos);        break;
    default:break;
  }
  if (reset) {
    DebugPrintItem_maxlen_1 = 0;
    DebugPrintItem_maxlen_2 = 0;    
  }
}


#endif


/*
 *
  Serial.printf("[size: %d]\n", _SplitItem.size());
  for(int i = 0; i < _SplitItem.size(); ++i) {
    SplitItem * ptr = _SplitItem.get(i);
    Serial.printf("[%d][c: %s][v: %s]\n", i, ptr->_cmd, ptr->_value);
  }
*/


/*
// fast 8-bit random number generator shamelessly borrowed from FastLED
uint16_t _rand16seed;
uint8_t random8() {
  _rand16seed = (_rand16seed * 2053) + 13849;
  return (uint8_t)((_rand16seed + (_rand16seed >> 8)) & 0xFF);
}

// note random8(lim) generates numbers in the range 0 to (lim -1)
uint8_t random8(uint8_t lim) {
  uint8_t r = random8();
  r = ((uint16_t)r * lim) >> 8;
  return r;
}

uint16_t random16() {
  return (uint16_t)random8() * 256 + random8();
}

// note random16(lim) generates numbers in the range 0 to (lim - 1)
uint16_t random16(uint16_t lim) {
  uint16_t r = random16();
  r = ((uint32_t)r * lim) >> 16;
  return r;
}
uint8_t random8(uint8_t min, uint8_t max) {
  return (random(min, max));
}

uint8_t map8(uint8_t val, uint8_t min, uint8_t max) {
  return map(val, 0, 255, min, max);
}
*/


// char * debug_printBuffer;

// void debugPrint () {
//   Serial.print(debug_printBuffer);
// }




/*
_LLlist::_LLlist () {
  this->m_head = NULL;
  _size = 0;
}

_LLlist::~_LLlist () {
  Node* n = this->m_head;
  while (n) {
    Node *aux = n;
    n = n->next;
    delete aux;
  }
}

void _LLlist::insert (const char * data) {
  Node *n = new Node();
  n->data = data;
  n->next = this->m_head;
  this->m_head = n;
  _size++;
}

void _LLlist::remove (const char * data) {
  Node *curr = this->m_head;
  Node *next = NULL;

  // node to delete is on HEAD
  if (curr && curr->data == data) {
    next = curr->next;
    this->m_head = next;
    delete curr;
    _size--;
    return ;
  }

  // node to delete is after head
  while (curr) {
    next = curr->next;
    if (next && next->data == data) {
      curr->next = next->next;
      delete next;
      _size--;
      return ;
    }
    curr = next;
  }

}

void _LLlist::reverse () {
  Node *curr = this->m_head;
  Node *next = NULL;
  Node *prev = NULL;

  while (curr) {
    next = curr->next;
    curr->next = prev;
    prev = curr;
    curr = next;
  }
  this->m_head = prev;
}

void _LLlist::print () {
  Node *n = this->m_head;
  while (n) {
    Serial.printf("%s\n", n->data);
    n = n->next;
  }
}
void _LLlist::print (uint8_t p, const char * & res) {
  Node *n = this->m_head;
  uint8_t i = 0;
  while (n) {
    if (i == p) {
      res = n->data;
      break;
    }
    i++;
    n = n->next;
  }
}

Node* _LLlist::search (const char * data) {
  Node *n = this->m_head;
  while (n) {
    n = n->next;
    if (n->data == data)
    break;
  }
  return n;
}

*/