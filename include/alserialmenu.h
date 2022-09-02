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

#ifdef DEBUG_KEYBOARD
#ifndef ALSERIALMENU_H
  #define ALSERIALMENU_H

  #include <Arduino.h>
  #include <LinkedList.h>
  #include <functional>

  typedef std::function<void()> sr_cb_v_f;
  typedef std::function<void(const String &, const String &)> sr_cb_ss_f;
  typedef enum sr_menumod { SRMM_SIMPLE, SRMM_KEYVAL/*, SRMM_KEY*/  } SR_MM;
  class Sr_timer
  {
    unsigned long _last =      0;
    uint32_t      _delay      = 0;
    boolean       _enabled    = false; 
  public:
    Sr_timer();
    Sr_timer(const uint32_t & max);
    ~Sr_timer();
    boolean execute();  
    void set_enabled(boolean);
    void set_delay(const uint32_t &);
    void get_delay(uint32_t &);
    boolean isEnabled();
  };
  class Sr_item
  {
    const char*   _name   = "";
    const char*   _key    = "";
    sr_cb_v_f     _cb_v   = nullptr;      
    sr_cb_ss_f    _cb_ss  = nullptr;    
    SR_MM         _mod    = SRMM_SIMPLE;  
  public:
    Sr_item();
    ~Sr_item();
    void set(const char*, const char*, sr_cb_v_f, SR_MM v4 = SRMM_SIMPLE);
    void set(const char*, const char*, sr_cb_ss_f, SR_MM v4 = SRMM_SIMPLE);
    void print();
    void get_key(const char * &);
    void get_name(const char * &);
    void get_mod(SR_MM&);
    void get_callback();
    void get_callback(const String &, const String&);
  };
  class Sr_menu
  {
    Sr_timer _timer_i1;
    Sr_timer _timer_h;    
    LList<Sr_item *> _list;
  public:
    Sr_menu();
    ~Sr_menu();
    void add(const char*, const char*, sr_cb_v_f, SR_MM v4 = SRMM_SIMPLE);
    void add(const char*, const char*, sr_cb_ss_f, SR_MM v4 = SRMM_SIMPLE);
    void serialRead();
    void serialReadString(const String &);
    void print();
  };
  extern Sr_menu _Sr_menu;
#endif // MENU_SERIAL_H  
#endif
