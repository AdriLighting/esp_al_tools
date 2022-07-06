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
    void add(const char * name, const char * tz, const char * country, const char * utc);

    
  public:
    LList<WorldClockItem *> _list;
    WorldClock()  {};
    ~WorldClock() {};


    void setup_default();

    void print_times(boolean shortTime = true);
    void print_avaibleTime();
    void print_registeredTime();

    void new_timeByCity(const String & search);
    void rem_timeByCity(const char * search);

    void get_timeByCity(const char * search, time_t & result);
    void get_timeByCity(const char * search, String & result, boolean shortTime = true);
    void get_timeByCity(uint8_t p, time_t & result);
  };

  extern WorldClock _WorldClock;
#endif // _ALWORLDCLOCK_H