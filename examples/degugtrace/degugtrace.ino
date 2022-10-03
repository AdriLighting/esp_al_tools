#include <altoolslib.h>

// Set "ALT_DEBUG_TARCE" in your compiler’s preprocessor options

// predifine static class DebugPrintList _DebugPrintList

// definition of regions
const char DEBUGREGION_MAIN [] PROGMEM = "main";
const char DEBUGREGION_SERVER [] PROGMEM = "server";

void petitefunction(){ALT_TRACEC(DEBUGREGION_MAIN, "ma petite string\n");}
void petitefunctionavecunestring(){const char * s_1 = "est belle";ALT_TRACEC(DEBUGREGION_MAIN, "ma petite string %s\n", s_1);}
void petitefunctionavecunegrandestring(){const char * s_1 = "est vraiment belle";ALT_TRACEC(DEBUGREGION_MAIN, "ma petite string %s\n", s_1);}
void print(){
    petitefunction();
    petitefunctionavecunestring();
    petitefunctionavecunegrandestring();
    petitefunction();    
}
void setup()
{
  Serial.begin(115200);

  for(unsigned long const serialBeginTime = millis(); !Serial && (millis() - serialBeginTime > 5000); ) { }
  delay(1000);

  Serial.println();
  Serial.printf_P(PSTR("\n#############\nALT DEBUG\n#############\n\n"));

  // Mandatory buffer initialization
  ALT_debugBuffer = new char[1024];   

  ALT_TRACEC(DEBUGREGION_SERVER, "string sans region, ne s'affiche pas\n");

  _DebugPrintList.add(DEBUGREGION_MAIN); 
  _DebugPrintList.add(DEBUGREGION_SERVER); 

  ALT_TRACEC(DEBUGREGION_MAIN, "string avec region, s'affiche\n");
  delay(2000);Serial.println();

  _DebugPrintList.ketboardPrint();
  delay(2000);Serial.println();

  DebugPrintItem * item = _DebugPrintList.get_item(DEBUGREGION_MAIN);
  // case 0: item->set_macro(vPos);        enable/disable the region 
  // case 1: item->set_timeStamp(vPos);  
  // case 2: item->set_heap(vPos);       
  // case 3: item->set_line(vPos);       
  // case 4: item->set_file(vPos);       
  // case 5: item->set_func(vPos);       
  // case 6: item->set_crmsg(vPos);      
  // case 7: item->set_lvl(vPos);        
  _DebugPrintList.keyboardSet( item, 0, 0, true);
  item->print();
  print();
  delay(2000);Serial.println();

  _DebugPrintList.keyboardSet( item, 0, 1, true);
  item->print();
  print();
  delay(2000);Serial.println();

  _DebugPrintList.keyboardSet( item, 1, 0, true);
  item->print();
  print();
  delay(2000);Serial.println();

  _DebugPrintList.keyboardSet( item, 2, 0, true);
  item->print();
  print();
  delay(2000);Serial.println();

  _DebugPrintList.keyboardSet( item, 3, 0, true);
  item->print();
  print();
  delay(2000);Serial.println();

  _DebugPrintList.keyboardSet( item, 4, 0, true);
  item->print();
  print();
  delay(2000);Serial.println();

  _DebugPrintList.keyboardSet( item, 5, 0, true);
  item->print();
  print();
  delay(2000);Serial.println();

  _DebugPrintList.keyboardSet( item, 3, 1, true);
  _DebugPrintList.keyboardSet( item, 4, 1, true);
  _DebugPrintList.keyboardSet( item, 5, 1, true);
  item->print();
  print();
  delay(2000);Serial.println();

  ALT_TRACEC(DEBUGREGION_MAIN, ""); 
  for(uint8_t i = 0; i < 5; ++i) {ALT_TRACEC(DEBUGREGION_MAIN, "&c:1&s:\t%d\n", i); }
  for(uint8_t i = 0; i < 5; ++i) {ALT_TRACEC(DEBUGREGION_MAIN, "&c:1&s:%d ", i); }
  Serial.println();
}

void loop()
{
    
}