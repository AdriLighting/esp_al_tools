#include <altoolslib.h>
#include <ArduinoJson.h>
// Set "ALSI_ENABLED" in your compiler’s preprocessor options


#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

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

  doc.clear();

  ALSYSINFO_getterByCat(doc, ALSI_NETWORK, 2);

  serializeJsonPretty(doc, Serial);Serial.println();   


}

void loop()
{
  
}