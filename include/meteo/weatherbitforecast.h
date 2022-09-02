#ifdef WEATHERBITFORECAST_ENABLED

#ifndef _WEATHERBITFORECAST_H
  #define _WEATHERBITFORECAST_H

  #include <ArduinoJson.h>
  #include <LinkedList.h>

  #define MAX_FORECASTS 4


  struct weatherbitForecastList {
    const char * key;
    const char * desc;
    const char * root;
  } ;

  class weatherbitForecastData {
    public:
      String * key  = nullptr;
      String * data = nullptr;
      weatherbitForecastData(){};
      ~weatherbitForecastData();

      void setup(uint8_t cnt);
  };

  class weatherbitForecastListSet {
    public:
      uint8_t   _keyPos   = 0;
      char      * _data   = nullptr;
      weatherbitForecastListSet(const char *  key);
  };
  extern LList<weatherbitForecastListSet *> _weatherbitForecastListSet;


  void weatherbitForecast_init();

  class weatherbitForecast {
    private:
      void buildUrl(String & result, const String & appId, const String & locationId, const String & language) ;

      boolean httpget(const String & url, String & r);
      boolean parse(const String & json);
    public:
      weatherbitForecast(uint8_t size);
      ~weatherbitForecast();
      boolean httpget_updateData(const String & appId, const String & locationId, const String & language);
      void print();
      void getKey(String & result, int cnt, const String & value);


  };
  extern weatherbitForecastData ** weatherbitForecastArray;

#endif
#endif
