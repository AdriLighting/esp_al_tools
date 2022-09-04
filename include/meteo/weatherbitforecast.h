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
  #ifndef WEATHERBITFORECAST_ENABLED
    #define WEATHERBITFORECAST_ENABLED
  #endif
#endif
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
