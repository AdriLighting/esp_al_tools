/*
    Author: adrilighting
    original source: https://docs.thingpulse.com/guides/wifi-color-display-kit/
*/

#ifdef WEATHERBITFORECAST_ENABLED

#ifndef _WEATHERBITFORECAST_H
  #define _WEATHERBITFORECAST_H

  #include <ArduinoJson.h>

  #define MAX_FORECASTS 4


  struct weatherbitForecastList {
    const char * key;
    const char * desc;
    const char * root;
  } ;

  class weatherbitForecastData {
    public:
      String key[40];
      String data[40];
      weatherbitForecastData();
  };

  void weatherbitForecast_init();

  class weatherbitForecast {
    private:
      void buildUrl(String & result, const String & appId, const String & locationId, const String & language) ;

      boolean httpget(const String & url, String & r);
      boolean parse(const String & json);
    public:
      weatherbitForecast();
      boolean httpget_updateData(const String & appId, const String & locationId, const String & language);
      void print();
      void getKey(String & result, int cnt, const String & value);


  };
  extern weatherbitForecastData weatherbitForecastArray[];

#endif
#endif
