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
  public:
    LList<WorldClockItem *> _list;
    WorldClock()  {};
    ~WorldClock() {};
    void add(const char * name, const char * tz, const char * country, const char * utc);
    void print();
    void print_fullList();
    void setup_default();
    void display();
  };

  extern WorldClock _WorldClock;
#endif // _ALWORLDCLOCK_H