/*
    Author: adrilighting
    original source: https://docs.thingpulse.com/guides/wifi-color-display-kit/
*/

#ifdef WEATHERBITFORECAST_ENABLED

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include "../../include/meteo/weatherbitforecast.h"
#include "../../include/altools.h"
#include "../../include/alhttptools.h"
#include "../../include/meteo/weatherapiid.h"


#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))





PROGMEM weatherbitForecastList weathebitForecastList_key [] = { 
  {ALMWBF_KEY_VALID_DATE,     ALMWBF_DESC_VALID_DATE,     ""},//
  {ALMWBF_KEY_TS,             ALMWBF_DESC_TS,             ""},
  {ALMWB_KEY_DATETIME,        ALMWB_DESC_DATETIME,        ""},
  {ALMWBF_KEY_WIND_GUST_SPD,  ALMWBF_DESC_WIND_GUST_SPD,  ""},
  {ALMWB_KEY_WIND_SPD,        ALMWB_DESC_WIND_SPD,        ""},//
  {ALMWB_KEY_WIND_DIR,        ALMWB_DESC_WIND_DIR,        ""},
  {ALMWB_KEY_WIND_CDIR,       ALMWB_DESC_WIND_CDIR,       ""},//
  {ALMWB_KEY_WIND_CDIR_FULL,  ALMWB_DESC_WIND_CDIR_FULL,  ""},
  {ALMWB_KEY_TEMP,            ALMWB_DESC_TEMP,            ""},//
  {ALMWBF_KEY_MAX_TEMP,       ALMWBF_DESC_MAX_TEMP,       ""},//
  {ALMWBF_KEY_MIN_TEMP,       ALMWBF_DESC_MIN_TEMP,       ""},//
  {ALMWBF_KEY_HIGH_TEMP,      ALMWBF_DESC_HIGH_TEMP,      ""},
  {ALMWBF_KEY_LOW_TEMP,       ALMWBF_DESC_LOW_TEMP,       ""},
  {ALMWBF_KEY_APP_MAX_TEMP,   ALMWBF_DESC_APP_MAX_TEMP,   ""},
  {ALMWBF_KEY_APP_MIN_TEMP,   ALMWBF_DESC_APP_MIN_TEMP,   ""},
  {ALMWBF_KEY_POP,            ALMWBF_DESC_POP,            ""},
  {ALMWB_KEY_PRECIP,          ALMWB_DESC_PRECIP,          ""},//
  {ALMWB_KEY_SNOW,            ALMWB_DESC_SNOW,            ""},
  {ALMWBF_KEY_SNOW_DEPTH,     ALMWBF_DESC_SNOW_DEPTH,     ""},
  {ALMWB_KEY_PRES,            ALMWB_DESC_PRES,            ""},//
  {ALMWB_KEY_SLP,             ALMWB_DESC_SLP,             ""},
  {ALMWB_KEY_DEWPT,           ALMWB_DESC_DEWPT,           ""},
  {ALMWB_KEY_RH,              ALMWB_DESC_RH,              ""},//
  {ALMWB_KEY_ICON,            ALMWB_DESC_ICON,            "weather"},//
  {ALMWB_KEY_CODE,            ALMWB_DESC_CODE,            ""},
  {ALMWB_KEY_DESCRIPTION,     ALMWB_DESC_DESCRIPTION,     "weather"},//
  {ALMWB_KEY_POD,             ALMWB_DESC_POD,             ""},
  {ALMWBF_KEY_CLOUDS_LOW,     ALMWBF_DESC_CLOUDS_LOW,     ""},
  {ALMWBF_KEY_CLOUDS_MID,     ALMWBF_DESC_CLOUDS_MID,     ""},
  {ALMWBF_KEY_CLOUDS_HI,      ALMWBF_DESC_CLOUDS_HI,      ""},
  {ALMWB_KEY_CLOUDS,          ALMWB_DESC_CLOUDS,          ""},//
  {ALMWB_KEY_VIS,             ALMWB_DESC_VIS,             ""},//
  {ALMWBF_KEY_MAX_DHI,        ALMWBF_DESC_MAX_DHI,        ""},
  {ALMWB_KEY_UV,              ALMWB_DESC_UV,              ""},
  {ALMWBF_KEY_OZONE,          ALMWBF_DESC_OZONE,          ""},
  {ALMWBF_KEY_MOON_PHASE,     ALMWBF_DESC_MOON_PHASE,     ""},
  {ALMWBF_KEY_MOONRISE_TS,    ALMWBF_DESC_MOONRISE_TS,    ""},
  {ALMWBF_KEY_MOONSET_TS,     ALMWBF_DESC_MOONSET_TS,     ""},
  {ALMWBF_KEY_SUNRISE_TS,     ALMWBF_DESC_SUNRISE_TS,     ""},
  {ALMWBF_KEY_SUNSET_TS,      ALMWBF_DESC_SUNSET_TS,      "" }
};
uint8_t weatherbitForecastList_keyCount = ARRAY_SIZE(weathebitForecastList_key);

    int forecastCnt = 0;


weatherbitForecastData weatherbitForecastArray[MAX_FORECASTS];
weatherbitForecastData::weatherbitForecastData() {
  weatherbitForecast_init();
}

weatherbitForecast::weatherbitForecast() {
  
}

void weatherbitForecast::buildUrl(String & r, const String & appId, const String & locationId, const String & language) {
  r = "http://api.weatherbit.io/v2.0/forecast/daily?city_id=" + locationId + "&days=" + MAX_FORECASTS + "&lang=" + language + "&key=" + appId;
}
boolean weatherbitForecast::httpget_updateData(const String & appId, const String & locationId, const String & language) {
  String url, result;

  buildUrl(url, appId, locationId, language);
  if (!httpget(url, result)) return false;
  delay(2);
  return parse(result);
}

boolean weatherbitForecast::httpget(const String & url, String & result) {
  int code = al_httptools::get_httpdata(result, url);
  Serial.printf_P(PSTR("[weatherbitForecast::httpget] get_httpdata code: %d\n"), code);
  if ( code != 200) return false;
  return true;
}
boolean weatherbitForecast::parse(const String & json){
  DynamicJsonDocument doc(3072);
  DeserializationError error =  deserializeJson(doc, json);
  Serial.printf_P(PSTR("[weatherbitForecast::parse] deserializeJson error: %s\n"), error.c_str());
  // if (error) return false;
  JsonArray array = doc[F("data")];
  for(size_t i = 0; i < array.size(); ++i) {
    for( int j = 0; j < weatherbitForecastList_keyCount; j++) { 
      String key = al_tools::ch_toString(weathebitForecastList_key[j].key);
      String root = al_tools::ch_toString(weathebitForecastList_key[j].root);
      if (root == "")
        weatherbitForecastArray[i].data[j] = array[i][key].as<String>();
      else {
        weatherbitForecastArray[i].data[j] = array[i][root][key].as<String>();
      }
    }       
  }  
  return true;
}
void weatherbitForecast_init() {
  String s;
  String v;
  String d;
  forecastCnt = 0;
  // Serial.printf("forecastCnt: %d\n", forecastCnt);
  // Serial.printf_P(PSTR("weatherbitForecastList_keyCount: %d\n"), weatherbitForecastList_keyCount);
    for( int i = 0; i < MAX_FORECASTS; i++) { 
      for( int j = 0; j < weatherbitForecastList_keyCount; j++) { 
        s = al_tools::ch_toString(weathebitForecastList_key[j].key);
        weatherbitForecastArray[i].key[j] = s;
        weatherbitForecastArray[i].data[j] = "";

      }     
    }     
}
void weatherbitForecast::print() {
  String s;
  String v;
  String d;
    for( int i = 0; i < MAX_FORECASTS; i++) { 
      Serial.printf_P(PSTR("\n[%d]\n"), i);
      for( int j = 0; j < weatherbitForecastList_keyCount; j++) { 
        s = weatherbitForecastArray[i].key[j];
        v = weatherbitForecastArray[i].data[j];
        d = al_tools::ch_toString(weathebitForecastList_key[j].desc);
        Serial.printf_P(PSTR("%25s - %22s - %s\n"), s.c_str(), v.c_str(), d.c_str());
      }     
    }       
}
void weatherbitForecast::getKey(String & ret, int cnt, const String & value) {
  ret = "";
  for( int i = 0; i < weatherbitForecastList_keyCount; i++) { 
    if(weatherbitForecastArray[cnt].key[i] == value) {
      ret = weatherbitForecastArray[cnt].data[i];
      break;
    }
  }   
}

/*
void weatherbitForecast::value(String value) {
  // if (forecastCnt<3) {
  //   for( int i = 0; i < weatherbitForecastList_keyCount; i++) { 
  //     if(weatherbitForecastArray[forecastCnt].key[i] == currentKey) {
  //       weatherbitForecastArray[forecastCnt].data[i] = value;
  //     }
  //   }       
  // }
  // if (currentKey == "clouds_low"){
  //   Serial.printf_P(PSTR("forecastCnt: %d\n"), forecastCnt);
  //   forecastCnt++;
  // }
}

*/
#endif