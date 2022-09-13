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

#ifndef ALT_DEFINE_DEFAULT
  #ifndef DEBUG_KEYBOARD
    #define DEBUG_KEYBOARD
  #endif
#endif
#ifdef DEBUG_KEYBOARD
#include "../include/alserialmenu.h"
#include "../include/altools.h"
#if defined(ALSI_ENABLED) && defined(DEBUG_KEYBOARD)
#include "../include/alsysinfo.h"
#endif

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
  HeapStatu * _ALHeapStatu = nullptr;
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
    case SR_MM::SRMM_KEYVAL: if (_cb_ss) _cb_ss(v1, v2); break;
    default:break;
  }
}
void Sr_item::get_callback(){
  switch (_mod) {
    case SR_MM::SRMM_SIMPLE: if (_cb_v) _cb_v(); break;
    case SR_MM::SRMM_KEYVAL: break;
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
  SRMM_KEYVAL
    "KEY" ... = ...  -> a debug si la pattern ne comprte pas  "=" 
      -3000=
      @loop=
      #7=
      #8=10000
*/
Sr_menu::Sr_menu(){
    _timer_h.set_delay(3000000);
    _timer_i1.set_enabled(false);
    _Sr_menu.add((const char *)"menu",          (const char *)"a", []() { _Sr_menu.print(); });
    _Sr_menu.add((const char *)"ESPreset",      (const char *)"z", []() { ESP.restart();    });
    _Sr_menu.add((const char *)"freeHeap",      (const char *)"e", []() { 
      String time; 
      al_tools::on_time_d(time);
      String heap;
      uint32_t freeHeap = 0;
      HeapStatu::get_initHeap(freeHeap);
      if (freeHeap==0) {
        if (!_ALHeapStatu) {
          _ALHeapStatu = new HeapStatu();
          _ALHeapStatu->setupHeap_v2();
        }
      } else {
        if (!_ALHeapStatu) _ALHeapStatu = new HeapStatu();     
      }
      _ALHeapStatu->update();
      _ALHeapStatu->print(heap);       
      Serial.printf_P(PSTR("%12s - %s\n"), time.c_str(), heap.c_str());   
    });
    _Sr_menu.add((const char *)"heapmonitor", (const char *)"-", [&](const String & v1, const String & v2) {
      _timer_h.set_delay(v1.toInt() * 1000);
      _timer_i1.set_enabled(v2.toInt());
    }, SR_MM::SRMM_KEYVAL);
    #if defined(ALSI_ENABLED) && defined(DEBUG_KEYBOARD)
    _Sr_menu.add((const char *)"sysinfo_value", (const char *)"t", []() { ALSYSINFO_print(); });  
    _Sr_menu.add((const char *)"sysinfo",       (const char *)"i", []() {
      Serial.printf_P(PSTR("@&ALSI:0,Network=\n"));
      Serial.printf_P(PSTR("@&ALSI:0,Network&WC:0,server=\n"));
      for(int i = 0; i < ALSI_CATEGORYSIZE; ++i) {
        Serial.printf_P(PSTR("[%-3d] %s\n"), i, ALSI_CATEGORY[i]);
        for(int j = 0; j < ALSI_ITEMSSIZE; ++j) {
          if (ALSI_items[j].GRP == ALSI_CATEGORY[i]) {
            Serial.printf_P(PSTR("[%-3d]\t%s\n"), j, ALSI_items[j].NAME);  
          }
        }   
      }   
    }); 
    _Sr_menu.add("asysinfo_keyb", "@", [](const String & v1, const String & v2) {  
      Serial.printf("CMD: %s - VAL: %s\n", v1.c_str(), v2.c_str());
      ALSYSINFO_keyboard_getter(v1);    
    }, SR_MM::SRMM_KEYVAL);       
    #endif    
    #ifdef ALT_DEBUG_TARCE
    _Sr_menu.add((const char *)"debugregion", (const char *)"u", []() { _DebugPrintList.ketboardPrint(); });    
    _Sr_menu.add((const char *)"debugset",    (const char *)";", [](const String & v1, const String & v2) { 
      _DebugPrintList.keyboardSet(v1,v2); }, SR_MM::SRMM_KEYVAL);    
    #endif
}
Sr_menu::~Sr_menu(){}

void Sr_menu::add(const char* v1, const char* v2, sr_cb_v_f v3, SR_MM v4){
  const char  * key = "";
  for(int i = 0; i < _list.size(); ++i) {
    _list[i]->get_key(key);
    if (strcmp(v2, key) == 0) {/*Serial.printf_P(PSTR("[Sr_menu::add | cb void] key %s already registered!\n"), v2);*/return;}
  }
  // Serial.printf_P(PSTR("[Sr_menu::add | cb void] adding key: %s - %s\n"), v2, v1);
  _list.add(new Sr_item());
  uint8_t pos = _list.size()-1;
  _list[pos]->set(v1, v2, v3, v4);
}
void Sr_menu::add(const char* v1, const char* v2, sr_cb_ss_f v3, SR_MM v4){
  const char  * key = "";
  for(int i = 0; i < _list.size(); ++i) {
    _list[i]->get_key(key);
    if (strcmp(v2, key) == 0) {/*Serial.printf_P(PSTR("[Sr_menu::add | cb ss] key %s already registered!\n"), v2);*/return;}
  }
  // Serial.printf_P(PSTR("[Sr_menu::add | cb ss] adding key: %s - %s\n"), v2, v1);
  _list.add(new Sr_item());
  uint8_t pos = _list.size()-1;
  _list[pos]->set(v1, v2, v3, v4);
}

void Sr_menu::serialRead(){
  if (_timer_i1.isEnabled() && _timer_i1.execute()) {
    if (_timer_h.isEnabled() && _timer_h.execute()) {
      String time; 
      al_tools::on_time_d(time);
      String heap;
      uint32_t freeHeap = 0;
      HeapStatu::get_initHeap(freeHeap);
      if (freeHeap==0) {
        if (!_ALHeapStatu) {
          _ALHeapStatu = new HeapStatu();
          _ALHeapStatu->setupHeap_v2();
        }
      } else {
        if (!_ALHeapStatu) _ALHeapStatu = new HeapStatu();     
      }
      _ALHeapStatu->update();
      _ALHeapStatu->print(heap);       
      Serial.printf_P(PSTR("%12s - %s\n"), time.c_str(), heap.c_str());      
    }
  } 

  if(!Serial.available()) return;

  String  str = Serial.readStringUntil('\n');
  String  ret;
  byte    len = str.length();
  char    * buffer = new char[len+1];
  char    ch[len+1]; 

  strcpy(ch, str.c_str());
  strcpy(buffer,"");

  for(int i=0; i < len; i++) {if (ch[i]=='\r' || ch[i]=='\n') break; strcat(buffer, String(ch[i]).c_str());}

  ret = al_tools::ch_toString(buffer);

  delete buffer;  

  serialReadString(ret);
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
      case SR_MM::SRMM_KEYVAL: 
        if (strcmp(keyStr, key) == 0) {
          {
            String cmd    = "";
            String value  = "";
            splitText(v1, key, cmd, value);

            // byte  len = cmd.length();
            // char  * buffer = new char[len];
            // char  ch[len+1]; 
            // strcpy(ch, cmd.c_str());
            // strcpy(buffer,"");
            // for(int i=1; i < len; i++) {strcat(buffer, String(ch[i]).c_str());}
            // _list[i]->get_callback(al_tools::ch_toString(buffer), value);
            // delete buffer;
            _list[i]->get_callback(cmd, value);
          }
        }
      break;/*
      case SR_MM::SRMM_KEY: 
        if (strcmp(keyStr, key) == 0) {
          {
            String cmd    = "";
            String value  = "";
            splitText(v1, key, cmd, value);
            _list[i]->get_callback(cmd, value);
            int rSize = 0;
            const char** split = al_tools::explode(v1, '&', rSize);
            for(int i = 0; i < rSize; ++i) {

            }
            for(int i = 0; i < rSize; ++i) {
              delete split[i];
            }
            delete[] split;              
          }
        }
      break;  */    
      default:break;
    }
  }
  delete keyStr;
  Serial.flush();
}
  
#endif