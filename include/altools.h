/**
 * MIT License
 * 
 * Copyright (c) 2022 Adrien Grellard
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef ALTOOLS_H
  #define ALTOOLS_H
  #include <Arduino.h>
  #include <ArduinoJson.h>
  #include <LinkedList.h>

  #ifndef ALT_DEFINE_DEFAULT
    #ifndef FSOK
      #define FSOK
    #endif
    #ifndef USE_LITTLEF
      #define USE_LITTLEF
    #endif
    #ifndef FILESYSTEM
      #if defined(ESP8266)
        #include <LittleFS.h> 
        #define FILESYSTEM LittleFS  
      #elif defined(ESP32)
        #include <FS.h>
        #include <LITTLEFS.h>
        #define FILESYSTEM LittleFS
      #endif
    #endif
    #ifndef ALT_DEBUG_TARCE
        #define ALT_DEBUG_TARCE
    #endif
    #ifndef ALSI_ENABLED
      #define ALSI_ENABLED
    #endif
  #endif

  #ifdef FILESYSTEM 
    #if defined USE_LITTLEFS
      #if defined(ESP8266)
        #include <LittleFS.h> 
      #elif defined(ESP32)
        #include <FS.h>
        #include <LITTLEFS.h>
      #endif
    #elif defined USE_SPIFFS
      #include <FS.h>
    #endif
  #endif  


    class FilePathList {
    public:
      String _folder;
      String _file;
      size_t _size = 0;
      FilePathList(const String & fo, const String & fi) : _folder(fo), _file(fi) {};
      FilePathList(const String & fo, const String & fi, size_t s) : _folder(fo), _file(fi), _size(s) {};
      ~FilePathList(){};
    }; 

  namespace al_tools {
    /*
      int rSize = 0;
      AP_explode(func, '(', rSize, nullptr) ;
      String split[rSize+1];
      AP_explode(func, '(', rSize, split) ;
    */
    void explode(const String & s, char sep, int & rSize, String * list);  
    /*
      https://stackoverflow.com/questions/16982015/getting-const-char-array-from-function
      String split = "s1,s2,s3";
      int splitSize;
      const char** list = AP_explode(split, ',', splitSize);
      Serial.printf("\n");
      for(int i = 0; i < splitSize; ++i) {Serial.printf("[%d] %s\n", i, list[i]);}
      for(int i = 0; i < rSize; ++i) {
        delete list[i];
      }
      delete[] list;
    */  
    const char** explode(const String & s, char sep, int & rSize);

    #ifdef FILESYSTEM 
      void SPIFFS_printFiles(const String & path, JsonObject & obj, boolean display = false, boolean subFolder = true);
      void SPIFFS_PRINT(const String & path = "", boolean sPrint = false, boolean display = false, boolean subFolder = true);
      void SPIFFS_readFile(const String & path);
      void SPIFFS_deleteRecursive(fs::FS &fs, const String &path);
      void SPIFFS_listFiles(LList<FilePathList * > * ptr, const String & in, boolean subFolder);
    #endif

    String ch_toString(const char * c);
    boolean c_str(char * & ptr, const String & str);
    boolean c_str(char * & ptr, const char * & );
    boolean c_str(char * & ptr, const char * const & str);
    char * c_str(const String & str); 
    
    void on_time_h(uint32_t, String & result);
    void on_time_h(String & result);
    void on_time_d(String & result);  
    void on_time_d(uint32_t, String & result);  
    void on_time_m(const uint64_t & s, String & result);  
  } // al_tools


  bool AP_deserializeFile(DynamicJsonDocument& doc, const char* filepath);  

  #ifdef ALT_DEBUG_TARCE

    static const char ALDT_REGION_TASK       [] PROGMEM = "task";
    static const char ALDT_REGION_WEBSERVER  [] PROGMEM = "webserver";
  // static const char ALML_DEBUGREGION_TASK       [] PROGMEM = "alml task";
  // static const char ALML_DEBUGREGION_WEBSERVER  [] PROGMEM = "alml webserver";
    class DebugPrintItem;

    #ifndef ALTDEBUGTARCE_FUNCTION_NAME_IN_FLASH
      #if defined(ESP8266)
        #define ALTDEBUGTARCE_FUNCTION_NAME_IN_FLASH 1
      #else
        #define ALTDEBUGTARCE_FUNCTION_NAME_IN_FLASH 0
      #endif
    #endif

    #if ALTDEBUGTARCE_FUNCTION_NAME_IN_FLASH
      #define ALTDEBUGTARCE_FUNCTION_NAME \
        reinterpret_cast<const __FlashStringHelper *>(__PRETTY_FUNCTION__)
    #else
      #define ALTDEBUGTARCE_FUNCTION_NAME __PRETTY_FUNCTION__
    #endif

    extern char * ALT_debugBuffer;

    static const char ALTPT_DEBUGREGIONML_001   [] PROGMEM = "HIGHT";
    static const char ALTPT_DEBUGREGIONML_002   [] PROGMEM = "MEDIUM";
    static const char ALTPT_DEBUGREGIONML_003   [] PROGMEM = "LOW";
    static const char ALTPT_DEBUGREGIONML_004   [] PROGMEM = "NORMAL";
    static const char* const ALTPT_DEBUGREGIONML_ALL[] PROGMEM = {
      ALTPT_DEBUGREGIONML_001, ALTPT_DEBUGREGIONML_002, ALTPT_DEBUGREGIONML_003, ALTPT_DEBUGREGIONML_004
    };
    typedef enum : uint8_t  {
      ALT_DEBUGLVL_HIGHT = 0, 
      ALT_DEBUGLVL_MEDIUM, 
      ALT_DEBUGLVL_LOW, 
      ALT_DEBUGLVL_NORMAL
    } ALT_DEBUGLVL_T; 
    // extern ALT_DEBUGLVL_T ALT_DEBUGLVLARR_T[];

    void ALT_debugPrint(const String & msg, const String & file, const String & line, const String & func);
    void ALT_debugPrint(const String & msg, const String & file, const String & line, const String & func, const char * ptr  = "", ALT_DEBUGLVL_T mod = ALT_DEBUGLVL_NORMAL);

    #define ALT_TRACE(parm_a, ...) { \
        if (!ALT_debugBuffer) ALT_debugBuffer = new char[512];  \
        sprintf_P(ALT_debugBuffer, (PGM_P)PSTR(parm_a), ##__VA_ARGS__); \
        ALT_debugPrint(String(ALT_debugBuffer), String(__FILE__), String(__LINE__), String(ALTDEBUGTARCE_FUNCTION_NAME)); \
      } 

    #define ALT_TRACEC(ptr, parm_a, ...) { \
      if (!ALT_debugBuffer) ALT_debugBuffer = new char[512];  \
      sprintf_P(ALT_debugBuffer, (PGM_P)PSTR(parm_a), ##__VA_ARGS__); \
      ALT_debugPrint(String(ALT_debugBuffer), String(__FILE__), String(__LINE__), String(ALTDEBUGTARCE_FUNCTION_NAME), ptr); \
    } 


    #define ALT_TRACEM(ptr, mod, parm_a, ...) { \
      if (!ALT_debugBuffer) ALT_debugBuffer = new char[512];  \
      sprintf_P(ALT_debugBuffer, (PGM_P)PSTR(parm_a), ##__VA_ARGS__); \
      ALT_debugPrint(String(ALT_debugBuffer), String(__FILE__), String(__LINE__), String(ALTDEBUGTARCE_FUNCTION_NAME), ptr, mod); \
    } 


    static const char APPT_DEBUGREGIONMC_001   [] PROGMEM = "range";

    static const char APPT_DEBUGREGIONMS_001   [] PROGMEM = "macro";
    static const char APPT_DEBUGREGIONMS_002   [] PROGMEM = "time";
    static const char APPT_DEBUGREGIONMS_003   [] PROGMEM = "heap";
    static const char APPT_DEBUGREGIONMS_004   [] PROGMEM = "line";
    static const char APPT_DEBUGREGIONMS_005   [] PROGMEM = "file";
    static const char APPT_DEBUGREGIONMS_006   [] PROGMEM = "func";
    static const char APPT_DEBUGREGIONMS_007   [] PROGMEM = "crmsg";
    static const char APPT_DEBUGREGIONMS_008   [] PROGMEM = "lvl";
    static const char* const APPT_DEBUGREGIONMS_ALL[] PROGMEM = {
    APPT_DEBUGREGIONMS_001, APPT_DEBUGREGIONMS_002, APPT_DEBUGREGIONMS_003, APPT_DEBUGREGIONMS_004, APPT_DEBUGREGIONMS_005, APPT_DEBUGREGIONMS_006, APPT_DEBUGREGIONMS_007, APPT_DEBUGREGIONMS_008
    };


    class DebugPrintItem {
      boolean _p_macro      = true;
      boolean _p_timeStamp  = true;
      boolean _p_heap       = true;
      boolean _p_line       = true;
      boolean _p_file       = true;
      boolean _p_func       = true;
      boolean _p_crmsg      = false;
      // boolean _p_arg       = true;

      ALT_DEBUGLVL_T      _lvl = ALT_DEBUGLVL_NORMAL;
    public:
      // const char  * _id = "";
      char * _id = nullptr;
      
      DebugPrintItem(const char * id);
      ~DebugPrintItem();

      void print();

      void set_macro(boolean);
      void set_timeStamp(boolean);
      void set_heap(boolean);
      void set_line(boolean);
      void set_file(boolean);
      void set_func(boolean);
      void set_crmsg(boolean);
      void set_lvl(uint8_t p);

      boolean is_macro();
      boolean is_timeStamp();
      boolean is_heap();
      boolean is_line();
      boolean is_file();
      boolean is_func();
      boolean is_crmsg();  

      ALT_DEBUGLVL_T get_lvl();    
    };
    class DebugPrintList
    {
    public:
      LList<DebugPrintItem *>  _list;

      DebugPrintList();
      ~DebugPrintList();
      DebugPrintItem * add(const char * const & id);
      DebugPrintItem * get_item(const char * v1);
      void ketboardPrint();
      void ketboardPrintHeader(boolean pNbId = false);
      void keyboardSet(const String &, const String &);
      void keyboardSet(DebugPrintItem*, const String &);
      void keyboardSet(DebugPrintItem * item, uint8_t aPos, uint8_t vPos, boolean reset = false);
    };
    extern DebugPrintList _DebugPrintList;

  #else
      #define ALT_TRACE(parm_a, ...) 
      #ifdef DALT_DEBUG_TARCE_SIMPLE
        #define ALT_TRACEC(ptr, parm_a, ...) { \
          Serial.printf_P((PGM_P)PSTR(parm_a), ##__VA_ARGS__); \
        } 
      #else 
        #define ALT_TRACEC(ptr, parm_a, ...)
      #endif
    /*

    */
  #endif


  class HeapStatu {
    uint32_t initHeap;
    int tcnt    = 0;
    int ttot    = 0;
    int tused   = 0;    
    boolean mod = true;
  public:

    static uint32_t pInitHeap;
    static void setupHeap_v1() {pInitHeap = ESP.getFreeHeap();};
    static void get_initHeap(uint32_t & result) {result = pInitHeap;};

    void setupHeap_v2() {mod = false; initHeap = ESP.getFreeHeap();};

    HeapStatu();
    ~HeapStatu(){};
    void print(String & ret);
    void update();

  };

  class SplitItem {
  public:
    SplitItem(const String  & v1, const String  & v2) {
      if (_cmd)   {delete _cmd;_cmd = nullptr;}
      if (_value) {delete _value;_value = nullptr;}
      _cmd    = new char[v1.length()+1];
      _value  = new char[v2.length()+1];
      strcpy(_cmd,    v1.c_str());
      strcpy(_value,  v2.c_str());
    };
    ~SplitItem(){
      if (_cmd)   {delete _cmd;}
      if (_value) {delete _value;}      
    }
     char* _cmd   = nullptr;
     char* _value = nullptr;

  };
  void splitText(const String & inputString, const char* const & arg,  char sep, LList<SplitItem * > * ptr);
  // void splitText(const String & inputString, const char* const & arg,  char sep, LList<SplitItem * > * ptr);


  boolean ALT_debugPrint(const char * _id);

#endif // TOOLS_H