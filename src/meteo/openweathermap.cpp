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
  #ifndef OPENWEATHMAP_ENABLED
    #define OPENWEATHMAP_ENABLED
  #endif
#endif
#ifdef OPENWEATHMAP_ENABLED

#include "../../include/meteo/openweathermap.h"
#include "../../include/altools.h"
#include "../../include/alhttptools.h"


#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

PROGMEM OpenWeatherMapData_t OpenWeatherMapData_items [] = { 
  {ALMOWM_KEY_LON,                    [](String & result, OpenWeatherMap_t * data){result = String(data->coord_lon); } },
  {ALMOWM_KEY_LAT,                    [](String & result, OpenWeatherMap_t * data){result = String(data->coord_lat); } },
  {ALMOWM_KEY_WEATHER_0_ID,           [](String & result, OpenWeatherMap_t * data){result = String(data->weather_0_id); } },
  {ALMOWM_KEY_WEATHER_0_MAIN,         [](String & result, OpenWeatherMap_t * data){result = String(data->weather_0_main); } },
  {ALMOWM_KEY_WEATHER_0_DESCRIPTION,  [](String & result, OpenWeatherMap_t * data){result = String(data->weather_0_description); } },
  {ALMOWM_KEY_WEATHER_0_ICON,         [](String & result, OpenWeatherMap_t * data){result = String(data->weather_0_icon); } },
  {ALMOWM_KEY_BASE,                   [](String & result, OpenWeatherMap_t * data){result = String(data->base); } },
  {ALMOWM_KEY_MAIN_TEMP,              [](String & result, OpenWeatherMap_t * data){result = String(data->main_temp); } },
  {ALMOWM_KEY_MAIN_FEELS_LIKE,        [](String & result, OpenWeatherMap_t * data){result = String(data->main_feels_like); } },
  {ALMOWM_KEY_MAIN_TEMP_MIN,          [](String & result, OpenWeatherMap_t * data){result = String(data->main_temp_min); } },
  {ALMOWM_KEY_MAIN_TEMP_MAX,          [](String & result, OpenWeatherMap_t * data){result = String(data->main_temp_max); } },
  {ALMOWM_KEY_MAIN_PRESSURE,          [](String & result, OpenWeatherMap_t * data){result = String(data->main_pressure); } },
  {ALMOWM_KEY_MAIN_HUMIDITY,          [](String & result, OpenWeatherMap_t * data){result = String(data->main_humidity); } },
  {ALMOWM_KEY_VISIBILITY,             [](String & result, OpenWeatherMap_t * data){result = String(data->visibility); } },
  {ALMOWM_KEY_WIND_SPEED,             [](String & result, OpenWeatherMap_t * data){result = String(data->wind_speed); } },
  {ALMOWM_KEY_WIND_DEG,               [](String & result, OpenWeatherMap_t * data){result = String(data->wind_deg); } },
  {ALMOWM_KEY_CLOUDS_ALL,             [](String & result, OpenWeatherMap_t * data){result = String(data->clouds_all); } },
  {ALMOWM_KEY_DT,                     [](String & result, OpenWeatherMap_t * data){result = String(data->dt); } },
  {ALMOWM_KEY_SYS_TYPE,               [](String & result, OpenWeatherMap_t * data){result = String(data->sys_type); } },
  {ALMOWM_KEY_SYS_ID,                 [](String & result, OpenWeatherMap_t * data){result = String(data->sys_id); } },
  {ALMOWM_KEY_SYS_COUNTRY,            [](String & result, OpenWeatherMap_t * data){result = String(data->sys_country); } },
  {ALMOWM_KEY_SYS_SUNRISE,            [](String & result, OpenWeatherMap_t * data){result = String(data->sys_sunrise); } },
  {ALMOWM_KEY_SYS_SUNSET,             [](String & result, OpenWeatherMap_t * data){result = String(data->sys_sunset); } },
  {ALMOWM_KEY_TIMEZONE,               [](String & result, OpenWeatherMap_t * data){result = String(data->timezone); } },
  {ALMOWM_KEY_ID,                     [](String & result, OpenWeatherMap_t * data){result = String(data->id); } },
  {ALMOWM_KEY_NAME,                   [](String & result, OpenWeatherMap_t * data){result = String(data->name); } },
  {ALMOWM_KEY_COD,                    [](String & result, OpenWeatherMap_t * data){result = String(data->cod); } }

//   {ALMOWM_KEY_MAIN_FEELS_LIKE,  }
};
uint8_t OpenWeatherMapData_itemsCount = ARRAY_SIZE(OpenWeatherMapData_items);
namespace al_openweathermap {
  void updataData(OpenWeatherMap_t * data, DynamicJsonDocument & doc){
      ALT_TRACEC("main", "-\n");
      data->coord_lon  = doc[F("coord")][F("lon")].as<float>();
      data->coord_lat  = doc[F("coord")][F("lat")].as<float>();

      data->weather_0_id           = doc[F("weather")][0][F("id")].as<uint8_t>();
      data->weather_0_main         = doc[F("weather")][0][F("main")].as<String>();
      data->weather_0_description  = doc[F("weather")][0][F("description")].as<String>();
      data->weather_0_icon         = doc[F("weather")][0][F("icon")].as<String>();

      data->base = doc[F("base")].as<String>();

      data->main_temp        = doc[F("main")][F("temp")].as<float>();
      data->main_feels_like  = doc[F("main")][F("feels_like")].as<float>();
      data->main_temp_min    = doc[F("main")][F("temp_min")].as<float>();
      data->main_temp_max    = doc[F("main")][F("temp_max")].as<float>();
      data->main_pressure    = doc[F("main")][F("pressure")].as<uint32_t>();
      data->main_humidity    = doc[F("main")][F("humidity")].as<uint8_t>();

      data->visibility = doc[F("visibility")].as<uint32_t>();

      data->wind_speed   = doc[F("wind")][F("speed")].as<float>();
      data->wind_deg     = doc[F("wind")][F("deg")].as<uint8_t>();

      data->clouds_all = doc[F("clouds")][F("all")].as<uint8_t>();

      data->dt = doc[F("dt")].as<long>();

      data->sys_type     = doc[F("sys")][F("type")].as<uint8_t>();
      data->sys_id       = doc[F("sys")][F("id")].as<uint32_t>();
      data->sys_country  = doc[F("sys")][F("country")].as<String>();
      data->sys_sunrise  = doc[F("sys")][F("sunrise")].as<long>();
      data->sys_sunset   = doc[F("sys")][F("sunset")].as<long>();

      data->timezone = doc[F("timezone")].as<uint32_t>();
      data->id       = doc[F("id")].as<long>();
      data->name     = doc[F("name")].as<String>();
      data->cod      = doc[F("cod")].as<uint8_t>(); 
      ALT_TRACEC("main", "--\n");
  }    
  void getJson(OpenWeatherMap_t * data, boolean isMetric, const String & appId, const String & locationId, const String & language) {
      String  units = isMetric ? "metric" : "imperial";
      String  url   = "http://api.openweathermap.org/data/2.5/weather?id=" + locationId + "&appid=" + appId + "&units=" + units + "&lang=" + language;

      String result = "";
      al_httptools::get_httpdata(result, url);
      DynamicJsonDocument doc(2000);
      deserializeJson(doc, result);
      updataData(data, doc);
  }
  void getValueByKey(OpenWeatherMap_t * data, const char * search, String & r){
    r = "";
    for(int i = 0; i < OpenWeatherMapData_itemsCount; ++i) {
      if ( al_tools::ch_toString(search) == al_tools::ch_toString(OpenWeatherMapData_items[i]._name) ) {
        OpenWeatherMapData_items[i].get_data(r, data);      
        break;       
      }
    }  
  }
  void serialPrint(OpenWeatherMap_t * data){
      for(int i = 0; i < OpenWeatherMapData_itemsCount; ++i) {
        String r;
        OpenWeatherMapData_items[i].get_data(r, data);      
        Serial.printf("%22s %-20s\n", OpenWeatherMapData_items[i]._name, r.c_str());
      }
  }

}


#endif