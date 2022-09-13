#include <altoolslib.h>

// Set "ALT_DEBUG_TARCE" in your compiler’s preprocessor options
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
    #ifdef ALT_DEBUG_TARCE
    _Sr_menu.add("debugregion", "u", []() { _DebugPrintList.ketboardPrint(); });    
    _Sr_menu.add("debugset",    ";", [](const String & v1, const String & v2) { 
      _DebugPrintList.keyboardSet(v1,v2); }, SR_MM::SRMM_KEYVAL);    
    #endif
*/

// definition of regions
const char DEBUGREGION_MAIN [] PROGMEM = "main";
const char DEBUGREGION_SERVER [] PROGMEM = "server";

void petitefunction(){ALT_TRACEC(DEBUGREGION_MAIN, "my little chain of character\n");}
void petitefunctionavecunestring(){const char * s_1 = "is beautiful";ALT_TRACEC(DEBUGREGION_MAIN, "my little chain of character %s\n", s_1);}
void petitefunctionavecunegrandestring(){const char * s_1 = "is really beautiful";ALT_TRACEC(DEBUGREGION_MAIN, "my little chain of character %s\n", s_1);}
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


  ALT_TRACEC(DEBUGREGION_MAIN, "string sans region, ne s'affiche pas\n");

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
  delay(2000);Serial.println();

  _Sr_menu.add("simple callback", "y", &print);
  _Sr_menu.add("simple callback", "t", [](){ALT_TRACEC(DEBUGREGION_SERVER, "debrug from server region\n");});
  _Sr_menu.add("simple callback", "r", [](){ALT_TRACEC(DEBUGREGION_MAIN, "debrug from main region\n");});
  _Sr_menu.print();
}


/*
KEYBOARD COMMAND
  [4][key: u][mod: 0][name: debugregion]
  [5][key: ;][mod: 1][name: debugset]

    pattern 
    &cmd=&value
    &cmd=&value:value_v1
    &cmd:cmd_v1,cmd_v2=&value:value_v1,value_v2,value_v3

    ----------CMD----------  -------------VALUE-------------
    ;&_id                   =&postion:valeur
    ;&main                  =&0:0  

    ;&_id                   =&range:start,end,valeur  
    ;&main                  =&range:0,7,0  

    ;&range:start,end       =&postion:valeur    
    ;&range:0,7             =&0:0

    ;&range:start,end       =&range:start,end,valeur 
    ;&range:0,7             =&range:0,7,0 

                !!! PAS DE CONTRA SUE LES RANGE DONC FIAR GAFFE AU VALEUR !!!
OUTPUT:
[SENT] ;range:0,2=&range:0,6,1
[keyboardSet] cmd[range:0,2] value[&range:0,6,1]
[               _id][0 macro][1  time][2  heap][3  line][4  file][5  func][6 crmsg][7   lvl]
[              main][      1][      1][      1][      1][      1][      1][      0][ NORMAL]
[            server][      1][      1][      1][      1][      1][      1][      0][ NORMAL]


*/

void loop()
{
  _Sr_menu.serialRead();   
}