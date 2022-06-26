#include <altoolslib.h>
#include <ArduinoJson.h>
// Set "ALSI_ENABLED" in your compiler’s preprocessor options
// Set "DEBUG_KEYBOARD" in your compiler’s preprocessor options

// predifine static class DebugPrintList _DebugPrintList
// predifine static class Sr_menu _Sr_menu
// 
/* initialization from constructor
    _Sr_menu.add("menu",          "a", []() { _Sr_menu.print(); });
    _Sr_menu.add("ESPreset",      "z", []() { ESP.restart();    });
    _Sr_menu.add("freeHeap",      "e", []() { String time; al_tools::on_time_d(time);Serial.printf_P(PSTR("time: %s\n"), time.c_str()); Serial.printf_P(PSTR("freeHeap: %d\n"), ESP.getFreeHeap()); });
    _Sr_menu.add("heapmonitor",   "-", [&](const String & v1, const String & v2) {
      _timer_h.set_delay(v1.toInt() * 1000);
      _timer_i1.set_enabled(v2.toInt());
    }, SR_MM::SRMM_KEYVAL);
    #if defined(ALSI_ENABLED) && defined(DEBUG_KEYBOARD)
    _Sr_menu.add("sysinfo", "t", []() { ALSYSINFO_print(); });  
    #endif    
    #ifdef ALT_DEBUG_TARCE
    _Sr_menu.add("debugregion", "u", []() { _DebugPrintList.ketboardPrint(); });    
    _Sr_menu.add("debugset",    ";", [](const String & v1, const String & v2) { 
      _DebugPrintList.keyboardSet(v1,v2); }, SR_MM::SRMM_KEYVAL);    
    #endif
*/

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

/*
 EXTERN 
  extern PROGMEM ALSI_LIST ALSI_items []; 
  extern const char* const ALSI_CATEGORY [] PROGMEM; 
  extern const uint8_t ALSI_ITEMSSIZE;
  extern const uint8_t ALSI_CATEGORYSIZE;
*/

void setup()
{
  Serial.begin(115200);

  for(unsigned long const serialBeginTime = millis(); !Serial && (millis() - serialBeginTime > 5000); ) { }
  delay(1000);

  Serial.println();
  Serial.printf_P(PSTR("\n#############\nALT SYSTEM INFOS\n#############\n\n"));


  // DISPLAY ALL AVAIBLE KEY AND CONTEGORY
  ALSYSINFO_print();

  const char * result = "";
  ALSYSINFO_getValByKey(ALSI_FREEHEAP, result);
  Serial.printf_P("KEY: %s - RESULT: %s\n", ALSI_FREEHEAP, result);

  DynamicJsonDocument doc(1024);
  ALSYSINFO_getterByCat(doc, ALSI_BOARD);

  serializeJsonPretty(doc, Serial);Serial.println();

  ALSYSINFO_getterByCat(doc, ALSI_FLASHCHIP);

  serializeJsonPretty(doc, Serial);Serial.println();

  doc.clear();

  ALSYSINFO_getterByCat(doc, "0", 1);

  serializeJsonPretty(doc, Serial);Serial.println(); 

  // doc.clear();

  ALSYSINFO_getterByCat(doc, ALSI_NETWORK, 2);

  serializeJsonPretty(doc, Serial);Serial.println();   
  Serial.println();   
  _Sr_menu.print();
}

void loop()
{
  _Sr_menu.serialRead();   
}