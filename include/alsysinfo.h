#ifndef _ALSYSINFO_H
  #define _ALSYSINFO_H
  #include <Arduino.h>
  #include <ArduinoJson.h>


  static const char ALSI_FREEHEAP        [] PROGMEM = "Free Heap";         // Ram
  #ifdef ESP8266
  static const char ALSI_HEAPFRAG        [] PROGMEM = "Heap fragmentation";  // Ram  
  #endif
  static const char ALSI_HEAPBLOCK       [] PROGMEM = "Largest free heap block"; // Ram
  static const char ALSI_INITHEAP        [] PROGMEM = "Start Free Heap";   // Ram
  #ifdef ESP32
  static const char ALSI_PSRAMSIZE       [] PROGMEM = "PSRAM Size";        // Ram
  #endif

  #ifdef FILESYSTEM
  static const char ALSI_FSTOTALBYTES    [] PROGMEM = "Total Bytes";       // Filesystem
  static const char ALSI_FSUSEDBYTES     [] PROGMEM = "Used Bytes";        // Filesystem
  static const char ALSI_FSFREEBYTES     [] PROGMEM = "Free Bytes";        // Filesystem
  static const char ALSI_FSBLOCKSIZE     [] PROGMEM = "Block size";        // Filesystem
  static const char ALSI_FSMAXOPENFILES  [] PROGMEM = "Max nb files open simultaneously";  // Filesystem
  static const char ALSI_FSMAXPATH       [] PROGMEM = "Max file name length";  // Filesystem 
  #endif

  // ESP.getSketchSize()
  // ESP.getSketchSize()+ESP.getFreeSketchSpace()
  static const char ALSI_SKETCHSIZETOTAL [] PROGMEM = "Total sketch size";       // Flash chip info
  static const char ALSI_SKETCHSIZE      [] PROGMEM = "Sketch size";       // Flash chip info
  static const char ALSI_FREESKETCHSIZE  [] PROGMEM = "Free Sketch size";  // Flash chip info
                                                                          // 
  // float flashChipSize = (float)ESP.getFlashChipSize() / 1024.0 / 1024.0;
  // Serial.printf("    Sketch thinks Flash RAM is size: ");  Serial.print(flashChipSize); Serial.println(" MB");
  static const char ALSI_FLASHSIZE       [] PROGMEM = "Flash Chip Size";         // Flash chip info
  static const char ALSI_RFLASHSIZE      [] PROGMEM = "Real Flash Chip Size";  // Flash chip info 

  static const char ALSI_FLASHFREQUENCY  [] PROGMEM = "Flash Chip frequency";  // Flash chip info 
  static const char ALSI_FLASHMOD        [] PROGMEM = "Flash Chip mod";  // Flash chip info 

  #ifdef ESP8266
  static const char ALSI_FLASHSIZEMAP    [] PROGMEM = "Flash size map";  // Flash chip info 
  // Serial.printf("    Flash chip Id: %08X (for example: Id=001640E0  Manuf=E0, Device=4016 (swap bytes))\n", ESP.getFlashChipId());
  static const char ALSI_FLASHCHIPID     [] PROGMEM = "Flash Chip ID";     // Flash chip info
                                                                          // 
  // float realFlashChipSize = (float)ESP.getFlashChipRealSize() / 1024.0 / 1024.0;
  // Serial.print("    Actual size based on chip Id: "); Serial.print(realFlashChipSize); Serial.println(" MB ... given by (2^( \"Device\" - 1) / 8 / 1024");

  static const char ALSI_COREVERSION     [] PROGMEM = "Core Version";      // Firmware info
  static const char ALSI_BOOTVERSION     [] PROGMEM = "Boot Version";      // Firmware info
  #endif

  #ifdef ESP32
  static const char ALSI_CHIPEV          [] PROGMEM = "Chip Rev";
  static const char ALSI_TEMPERATURE     [] PROGMEM = "Temperature";
  #endif

  static const char ALSI_LASTRESET       [] PROGMEM = "Last reset reason"; // Firmware info  
  static const char ALSI_CHIPID          [] PROGMEM = "Chip ID";           // Firmware info
  static const char ALSI_STAMAC          [] PROGMEM = "Station MAC";           // Firmware info
  static const char ALSI_APAMAC          [] PROGMEM = "Access Point MAC";          // Firmware info
  static const char ALSI_SDKVERSION      [] PROGMEM = "SDK Version";       // Firmware info
  static const char ALSI_CPUFREQUENCY    [] PROGMEM = "CPU Frequency";     //
  static const char ALSI_MODEL           [] PROGMEM = "Chip: Model";
  static const char ALSI_FEATURES        [] PROGMEM = "Features";
  static const char ALSI_CORES           [] PROGMEM = "Cores";
  static const char ALSI_REVISION        [] PROGMEM = "Revision";
  static const char ALSI_ESPIDFVERS      [] PROGMEM = "ESP-IDF Version";
  static const char ALSI_EFUSEMAC        [] PROGMEM = "EFuse MAC";

  static const char ALSI_APIIP           [] PROGMEM = "Access Point IP";
  static const char ALSI_APSSID          [] PROGMEM = "Access Point SSID";
  static const char ALSI_APBSSID         [] PROGMEM = "Access Point BSSID";
  static const char ALSI_STAIP           [] PROGMEM = "Station IP";
  static const char ALSI_STASUBNET       [] PROGMEM = "Station Subnet";
  static const char ALSI_STAGATEWAY      [] PROGMEM = "Station Gateway";
  static const char ALSI_CONNECT         [] PROGMEM = "Connected";
  static const char ALSI_AUTOCONNECT     [] PROGMEM = "Autoconnect";
  static const char ALSI_DNSSERVER       [] PROGMEM = "DNS Server";
  static const char ALSI_HOSTNAME        [] PROGMEM = "Hostname";

  static const char ALSI_BUILDDATE       [] PROGMEM = "Build Date";
  static const char ALSI_UPTIME          [] PROGMEM = "Uptime";
  /*
  Memory
  Filesystem
  Board
  Network
  */
  static const char ALSI_HEAPSIZE        [] PROGMEM = "heap";
  // static const char ALSI_FIRMWARE       [] PROGMEM = "Firmware";
  static const char ALSI_FLASHCHIP       [] PROGMEM = "flash chip";
  static const char ALSI_SKETCH          [] PROGMEM = "sketch";
  static const char ALSI_FILESYSTEM      [] PROGMEM = "filesystem";
  static const char ALSI_BOARD           [] PROGMEM = "core";
  static const char ALSI_NETWORK         [] PROGMEM = "network";
  static const char ALSI_TIME            [] PROGMEM = "time";


  struct ALSI_LIST {
    const char  * NAME;
    void        (* DATA ) (const char *&);
    const char  * GRP;
  } ;
  extern PROGMEM ALSI_LIST ALSI_items []; 
  extern const char* const ALSI_CATEGORY [] PROGMEM; 
  extern const uint8_t ALSI_ITEMSSIZE;
  extern const uint8_t ALSI_CATEGORYSIZE;

  void ALSYSINFO_print();
  void ALSYSINFO_getterByCat(DynamicJsonDocument & doc, const char * key);
  void ALSYSINFO_getterByKey(DynamicJsonDocument & doc, const char * key, bool keyStr = true);
  void ALSYSINFO_keyboard_getter(const String & v1); 
  void ALSYSINFO_keyboard_print();  
  boolean ALSYSINFO_getValByKey(const char * key, const char *& ret) ;  
#endif // _ALSYSINFO_H