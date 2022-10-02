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

#ifndef _ALWORLDCLOCK_H
  #define _ALWORLDCLOCK_H
  
  #include <Arduino.h>
  #include <LinkedList.h>

  class WorldClockItem
  {
  public:   
    char * _name        = nullptr;
    char * _tz          = nullptr;
    char * _country     = nullptr;
    char * _utc_offset  = nullptr;


    WorldClockItem(const char * name, const char * tz, const char * country, const char * utc);
    ~WorldClockItem();
    String get_name();
  };
  class WorldClock
  {
    boolean get_timeHTTP(const String & timezone, String & ret);
    boolean get_timeHTTP_TZDB(const String & timezone, String & ret);
    void add(const char * name, const char * tz, const char * country, const char * utc);

    
  public:
    LList<WorldClockItem *> _list;
    WorldClock()  {};
    ~WorldClock() {};

    char * _timezoneDB_key  = nullptr;

    void set_timezoneDB_key(const char * const & key );
    void get_timezoneDB_key(String & ret );
    void setup_default();

    void print_times(boolean shortTime = true);
    void print_avaibleTime();
    void print_registeredTime();

    void new_timeByCity(const String & search);
    void rem_timeByCity(const char * search);

    void get_timeByCity(const char * search, time_t & result);
    void get_timeByCity(const char * search, String & result, boolean shortTime = true);
    void get_timeByCity(uint8_t p, time_t & result);

    WorldClockItem * get_item(uint8_t);
    WorldClockItem * get_itemByCity(const char * search);
  };

  extern WorldClock _WorldClock;
#endif // _ALWORLDCLOCK_H