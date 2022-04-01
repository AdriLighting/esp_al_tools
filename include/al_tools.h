#ifndef _AL_TOOLS_H
  #define _AL_TOOLS_H
  #include <Arduino.h>
  #include <ArduinoJson.h>
  #include <LinkedList.h>

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
      void SPIFFS_printFiles(const String & path, JsonObject & obj);
      void SPIFFS_PRINT(boolean sPrint = false);
    #endif

    String ch_toString(const char * c);
    
    void on_time_h(String & result);
    void on_time_d(String & result);  
  } // al_tools


  bool AP_deserializeFile(DynamicJsonDocument& doc, const char* filepath);  

  #ifdef ALT_DEBUG_TARCE

    class DebugPrintItem;

    #ifndef ARDUINOTRACE_FUNCTION_NAME_IN_FLASH
      #if defined(ESP8266)
        #define ARDUINOTRACE_FUNCTION_NAME_IN_FLASH 1
      #else
        #define ARDUINOTRACE_FUNCTION_NAME_IN_FLASH 0
      #endif
    #endif

    #if ARDUINOTRACE_FUNCTION_NAME_IN_FLASH
      #define ARDUINOTRACE_FUNCTION_NAME \
        reinterpret_cast<const __FlashStringHelper *>(__PRETTY_FUNCTION__)
    #else
      #define ARDUINOTRACE_FUNCTION_NAME __PRETTY_FUNCTION__
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
      if (ALT_debugBuffer) { \
        sprintf_P(ALT_debugBuffer, (PGM_P)PSTR(parm_a), ##__VA_ARGS__); \
        ALT_debugPrint(String(ALT_debugBuffer), String(__FILE__), String(__LINE__), String(ARDUINOTRACE_FUNCTION_NAME));}  \
      } 

    #define ALT_TRACEC(ptr, parm_a, ...) { \
      if (ALT_debugBuffer) { \
        sprintf_P(ALT_debugBuffer, (PGM_P)PSTR(parm_a), ##__VA_ARGS__); \
        ALT_debugPrint(String(ALT_debugBuffer), String(__FILE__), String(__LINE__), String(ARDUINOTRACE_FUNCTION_NAME), ptr);} \
    } 


    #define ALT_TRACEM(ptr, mod, parm_a, ...) { \
      if (ALT_debugBuffer){ \
        sprintf_P(ALT_debugBuffer, (PGM_P)PSTR(parm_a), ##__VA_ARGS__); \
        ALT_debugPrint(String(ALT_debugBuffer), String(__FILE__), String(__LINE__), String(ARDUINOTRACE_FUNCTION_NAME), ptr, mod);} \
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
      void keyboardSet(DebugPrintItem * item, uint8_t aPos, uint8_t vPos);
    };
    extern DebugPrintList _DebugPrintList;

  #else
      #define ALT_TRACE(parm_a, ...) 
      #define ALT_TRACEC(ptr, parm_a, ...) 
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
  // void splitText(const String & inputString, const char* const & arg,  char sep, LList<SplitItem * > * ptr);




#endif // TOOLS_H