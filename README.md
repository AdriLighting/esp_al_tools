# esp_al_tools


![image](https://user-images.githubusercontent.com/43286266/180238286-367e2623-d4b0-4ef1-8c3b-979067ea37c6.png)


## al_tools - altools.h
static instance
   - DebugPrintList _DebugPrintList;
 
variables globals
   - char * ALT_debugBuffer;

define
  - ALT_DEBUG_TRACE
  - ALT_TRACE(parm_a, ...) 
  - ALT_TRACEC(ptr, parm_a, ...)
  - ALT_TRACEM(ptr, mod, parm_a, ...) 

namespace al_tools

````c++
  const char** explode(const String & s, char sep, int & rSize);
  
  String ch_toString(const char * c);
  
  void on_time_h(uint32_t, String & result);
  void on_time_h(String & result);
  void on_time_d(String & result);  
  void on_time_d(uint32_t, String & result);  
  void on_time_m(const uint64_t & s, String & result);    

  #ifdef FILESYSTEM 
    void SPIFFS_printFiles(const String & path, JsonObject & obj, boolean display = false, boolean subFolder = true);
    void SPIFFS_PRINT(const String & path = "", boolean sPrint = false, boolean display = false, boolean subFolder = true);
    void SPIFFS_readFile(const String & path);
    void SPIFFS_deleteRecursive(fs::FS &fs, const String &path);
    void SPIFFS_listFiles(LList<FilePathList * > * ptr, const String & in, boolean subFolder);
  #endif
  
```` 

global
````c++
bool AP_deserializeFile(DynamicJsonDocument& doc, const char* filepath);  
void splitText(const String & inputString, const char* const & arg,  char sep, LList<SplitItem * > * ptr);
boolean ALT_debugPrint(const char * _id);
```` 

class
````c++
class DebugPrintItem
  public
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
class DebugPrintList
  public:
    DebugPrintItem * add(const char * const & id);
    DebugPrintItem * get_item(const char * v1);
    void ketboardPrint();
    void ketboardPrintHeader(boolean pNbId = false);
    void keyboardSet(const String &, const String &);
    void keyboardSet(DebugPrintItem*, const String &);
    void keyboardSet(DebugPrintItem * item, uint8_t aPos, uint8_t vPos, boolean reset = false);  
 
--------------------------------------------

class HeapStatu   
  public:

    static uint32_t pInitHeap;
    static void setupHeap_v1() {pInitHeap = ESP.getFreeHeap();};
    static void get_initHeap(uint32_t & result) {result = pInitHeap;};

    void setupHeap_v2() {mod = false; initHeap = ESP.getFreeHeap();};

    HeapStatu();
    ~HeapStatu(){};
    void print(String & ret);
    void update();

--------------------------------------------

  class SplitItem {
  public:
    SplitItem(const String  & v1, const String  & v2)
    ~SplitItem()
     char* _cmd   = nullptr;
     char* _value = nullptr;
```` 
