#ifdef WEATHERBIT_ENABLED

#ifndef WEATHERBIT_H
  #define WEATHERBIT_H


  #include <Arduino.h>
  #include <LinkedList.h>


  struct weatherbitList {
      const char *  key;
      const char *  desc;
      const char *  root;
      // boolean enable;
      // String data;
      // void    (* setValue ) (String msg);
  } ;

  class weatherbitData {
    public:
      uint8_t   _keyPos = 0;
      char *    _data   = nullptr;
      weatherbitData(const char *  key);
  };
  extern LList<weatherbitData *> _weatherbitData;

  class weatherbitCurrent {
    private:
      String currentKey;
      String currentParent;

      void buildUrl(String & result, const String & appId, const String & locationId, const String & language) ;
      boolean httpget(const String & url, String & r);
      boolean parse(const String & json);

    public:
      weatherbitCurrent();

      boolean httpget_updateData(const String & appId, const String & locationId, const String & language);
      void print();
      void getKey(String & result, const String & value);
  };

  // extern PROGMEM weatherbitList weatherbitList_key [] ;
#endif
#endif
