#include "al_serialmenu.h"

#include <al_tools.h>
// #include "../include/program.h"
// #include "../include/apapi.h"

namespace {
  void splitText(const String & inputString, const char* const & sep, String & cmd, String & value) {
    char    inputChar[inputString.length() + 1] ;
    inputString.toCharArray(inputChar, inputString.length() + 1);
    char    * Rcommand  = strtok(inputChar, sep);
    while (Rcommand != 0){          
      char* separator  = strchr(Rcommand, '=');
      if (separator != 0) {
        *separator  = 0;            
        ++separator ;
        if (String(Rcommand)!=""){
          value = String(separator);
          cmd   = String(Rcommand);
          break;
        }
      }
      Rcommand = strtok(0, sep); 
    }
  }

// boolean _APSR_heapMonitor = false;
// uint32_t _APSR_timer_heapMonitor = 0;  
// uint32_t _APSR_timerMax_heapMonitor = 3000;  
}

Sr_timer::Sr_timer(){
  _delay      = 100000;
  _last       = 0;  
  _enabled    = true; 
}
Sr_timer::Sr_timer(const uint32_t & max){
  _delay      = max;
  _last       = 0;  
  _enabled    = true; 
}
Sr_timer::~Sr_timer(){}
boolean Sr_timer::execute () {
  if (!_enabled)    return false;

  unsigned long now     = micros();
  unsigned long delta = (now >= _last) ? now - _last : (0xffffffff - _last) + now ;
  boolean done  = (delta >= _delay);

  if (done) {
    // char t[12];
    // _timeStamp(now, t);
    // LOG(printf_P, PSTR("[%-12s][millis: %8d][_delay: %8d][elapsed:%8d]\n"), t, (now/1000), _delay, delta);   
    _last = now;
    return true;
  }
  return false;
}
void Sr_timer::set_enabled(boolean v1)         {_enabled = v1;}
void Sr_timer::set_delay(const uint32_t & v1)  {_delay = v1;}
void Sr_timer::get_delay(uint32_t & v1)        {v1 = _delay;}
boolean Sr_timer::isEnabled()                  {return _enabled;}

Sr_menu _Sr_menu;

Sr_item::Sr_item(){
}
Sr_item::~Sr_item(){}

void Sr_item::set(const char* v1, const char* v2, sr_cb_v_f v3, SR_MM v4) {
  _name   = v1;
  _key    = v2;
  _cb_v   = std::move(v3);   
 _mod     = v4;  
}
void Sr_item::set(const char* v1, const char* v2, sr_cb_ss_f v3, SR_MM v4) {
  _name   = v1;
  _key    = v2;
  _cb_ss  = std::move(v3);   
  _mod    = v4;    
}

void Sr_item::print() {
  Serial.printf_P(PSTR("[name: %s][key: %s]\n"), _name, _key);
}

void Sr_item::get_key(const char * & v1)  {v1 = _key;}
void Sr_item::get_name(const char * & v1) {v1 = _name;}
void Sr_item::get_mod(SR_MM & v1)         {v1 = _mod;}

void Sr_item::get_callback(const String & v1, const String & v2){
  switch (_mod) {
    case SR_MM::SRMM_SIMPLE: break;
    case SR_MM::SRMM_KEY: if (_cb_ss) _cb_ss(v1, v2); break;
    default:break;
  }
}
void Sr_item::get_callback(){
  switch (_mod) {
    case SR_MM::SRMM_SIMPLE: if (_cb_v) _cb_v(); break;
    case SR_MM::SRMM_KEY: break;
    default:break;
  }
}

void Sr_menu::print(){
  const char  * key = "";
  const char  * name = "";
  SR_MM mod;
  for(int i = 0; i < _list.size(); ++i) {
    Sr_item * item = _list.get(i);
    item->get_key(key);
    item->get_name(name);
    item->get_mod(mod);
    Serial.printf_P(PSTR("[%d][key: %s][mod: %d][name: %s]\n"), i, key, mod, name);
  }
}
/*
  SRMM_KEY
    "KEY" ... = ...  -> a debug si la pattern ne comprte pas  "=" 
      -3000=
      @loop=
      #7=
      #8=10000
*/
Sr_menu::Sr_menu(){
    _timer_h.set_delay(3000000);
    _timer_i1.set_enabled(false);
    _Sr_menu.add("menu",          "a", []() { _Sr_menu.print(); });
    _Sr_menu.add("ESPreset",      "z", []() { ESP.restart();    });
    _Sr_menu.add("freeHeap",      "e", []() { String time; al_tools::on_time_d(time);Serial.printf_P(PSTR("time: %s\n"), time.c_str()); Serial.printf_P(PSTR("freeHeap: %d\n"), ESP.getFreeHeap()); });
    _Sr_menu.add("heapmonitor",   "-", [&](const String & v1, const String & v2) {
      _timer_h.set_delay(v1.toInt() * 1000);
      _timer_i1.set_enabled(v2.toInt());
    }, SR_MM::SRMM_KEY);
    #ifdef ALT_DEBUG_TARCE
    _Sr_menu.add("debugregion", "u", []() { _DebugPrintList.ketboardPrint(); });    
    _Sr_menu.add("debugset",    ";", [](const String & v1, const String & v2) { 
      _DebugPrintList.keyboardSet(v1,v2); }, SR_MM::SRMM_KEY);    
    #endif
}
Sr_menu::~Sr_menu(){}

void Sr_menu::add(const char* v1, const char* v2, sr_cb_v_f v3, SR_MM v4){
  const char  * key = "";
  for(int i = 0; i < _list.size(); ++i) {
    _list[i]->get_key(key);
    if (strcmp(v2, key) == 0) {Serial.printf_P(PSTR("[Sr_menu::add | cb void] key %s already registered!\n"), v2);return;}
  }
  Serial.printf_P(PSTR("[Sr_menu::add | cb void] adding key: %s\n"), v2);
  _list.add(new Sr_item());
  uint8_t pos = _list.size()-1;
  _list[pos]->set(v1, v2, v3, v4);
}
void Sr_menu::add(const char* v1, const char* v2, sr_cb_ss_f v3, SR_MM v4){
  const char  * key = "";
  for(int i = 0; i < _list.size(); ++i) {
    _list[i]->get_key(key);
    if (strcmp(v2, key) == 0) {Serial.printf_P(PSTR("[Sr_menu::add | cb ss] key %s already registered!\n"), v2);return;}
  }
  Serial.printf_P(PSTR("[Sr_menu::add | cb ss] adding key: %s\n"), v2);
  _list.add(new Sr_item());
  uint8_t pos = _list.size()-1;
  _list[pos]->set(v1, v2, v3, v4);
}

void Sr_menu::serialRead(){
  if (_timer_i1.isEnabled() && _timer_i1.execute()) {
    if (_timer_h.isEnabled() && _timer_h.execute()) {
      String time; 
      al_tools::on_time_d(time);
      Serial.printf_P(PSTR("%s - %d\n"), time.c_str(), ESP.getFreeHeap());      
    }
  } 
  if(!Serial.available()) return;
  String str = Serial.readStringUntil('\n');
  serialReadString(str);
}
void Sr_menu::serialReadString(const String & v1){
  if (v1 == "") return;
  char        * keyStr = new char[80];
  uint8_t     size  = _list.size();
  const char  * key = "";
  SR_MM       mod;
  strcpy(keyStr, String(v1[0]).c_str());
  for(int i = 0; i < size; ++i) {
    _list[i]->get_key(key);
    _list[i]->get_mod(mod);
    if (key == (const char*)"") continue; 
    switch (mod) {
      case SR_MM::SRMM_SIMPLE:
        if ( String((const __FlashStringHelper*) key) == v1 ) {_list[i]->get_callback(); } break;
      case SR_MM::SRMM_KEY: 
        if (strcmp(keyStr, key) == 0) {
          {
            String cmd    = "";
            String value  = "";
            splitText(v1, key, cmd, value);
            _list[i]->get_callback(cmd, value);
          }
        }
      break;
      default:break;
    }
  }
  delete keyStr;
  Serial.flush();
}
