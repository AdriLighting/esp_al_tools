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
  #ifndef WEATHERBIT_ENABLED
    #define WEATHERBIT_ENABLED
  #endif
#endif
#ifdef WEATHERBIT_ENABLED

  #if defined(ESP8266)
    extern "C" {
      #include "user_interface.h"
    }
    #include <ESP8266WiFi.h>
    #include <ESP8266mDNS.h>
    #include <ESP8266HTTPClient.h>
  #elif defined(ESP32)
    #include <ESPmDNS.h>
    #include <WiFi.h>
    #include <HTTPClient.h>
  #endif
#include <WiFiClient.h>

#include "../../include/meteo/weatherbit.h"
#include "../../include/altools.h"
#include "../../include/alhttptools.h"
#include "../../include/meteo/weatherapiid.h"

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

PROGMEM weatherbitList weatherbitList_key [] = { 
  {ALMWB_KEY_LAT,                             ALMWB_DESC_LAT,             (const char *)""        },
  {ALMWB_KEY_LON,                             ALMWB_DESC_LON,             (const char *)""        },
  {ALMWB_KEY_SUNRISE,                         ALMWB_DESC_SUNRISE,         (const char *)""        },
  {ALMWB_KEY_SUNSET,                          ALMWB_DESC_SUNSET,          (const char *)""        },
  {ALMWB_KEY_TIMEZONE,                        ALMWB_DESC_TIMEZONE,        (const char *)""        },
  {ALMWB_KEY_STATION,                         ALMWB_DESC_STATION,         (const char *)""        },
  {ALMWB_KEY_OB_TIME,                         ALMWB_DESC_OB_TIME,         (const char *)""        },
  {ALMWB_KEY_DATETIME,                        ALMWB_DESC_DATETIME,        (const char *)""        },
  {ALMWB_KEY_TS,                              ALMWB_DESC_TS,              (const char *)""        },
  {ALMWB_KEY_CITY_NAME,                       ALMWB_DESC_CITY_NAME,       (const char *)""        },
  {ALMWB_KEY_COUNTRY_CODE,                    ALMWB_DESC_COUNTRY_CODE,    (const char *)""        },
  {ALMWB_KEY_STATE_CODE,                      ALMWB_DESC_STATE_CODE,      (const char *)""        },
  {ALMWB_KEY_PRES,                            ALMWB_DESC_PRES,            (const char *)""        },
  {ALMWB_KEY_SLP,                             ALMWB_DESC_SLP,             (const char *)""        },
  {ALMWB_KEY_WIND_SPD,                        ALMWB_DESC_WIND_SPD,        (const char *)""        },
  {ALMWB_KEY_WIND_DIR,                        ALMWB_DESC_WIND_DIR,        (const char *)""        },
  {ALMWB_KEY_WIND_CDIR,                       ALMWB_DESC_WIND_CDIR,       (const char *)""        },
  {ALMWB_KEY_WIND_CDIR_FULL,                  ALMWB_DESC_WIND_CDIR_FULL,  (const char *)""        },
  {ALMWB_KEY_TEMP,                            ALMWB_DESC_TEMP,            (const char *)""        },
  {ALMWB_KEY_APP_TEMP,                        ALMWB_DESC_APP_TEMP,        (const char *)""        },
  {ALMWB_KEY_RH,                              ALMWB_DESC_RH,              (const char *)""        },
  {ALMWB_KEY_DEWPT,                           ALMWB_DESC_DEWPT,           (const char *)""        },
  {ALMWB_KEY_CLOUDS,                          ALMWB_DESC_CLOUDS,          (const char *)""        },
  {ALMWB_KEY_POD,                             ALMWB_DESC_POD,             (const char *)""        },
  {ALMWB_KEY_ICON,                            ALMWB_DESC_ICON,            (const char *)"weather" },
  {ALMWB_KEY_CODE,                            ALMWB_DESC_CODE,            (const char *)"weather" },
  {ALMWB_KEY_DESCRIPTION,                     ALMWB_DESC_DESCRIPTION,     (const char *)"weather" },
  {ALMWB_KEY_VIS,                             ALMWB_DESC_VIS,             (const char *)""        },
  {ALMWB_KEY_PRECIP,                          ALMWB_DESC_PRECIP,          (const char *)""        },
  {ALMWB_KEY_SNOW,                            ALMWB_DESC_SNOW,            (const char *)""        },
  {ALMWB_KEY_UV,                              ALMWB_DESC_UV,              (const char *)""        },
  {ALMWB_KEY_AQI,                             ALMWB_DESC_AQI,             (const char *)""        },
  {ALMWB_KEY_DHI,                             ALMWB_DESC_DHI,             (const char *)""        },
  {ALMWB_KEY_DNI,                             ALMWB_DESC_DNI,             (const char *)""        },
  {ALMWB_KEY_GHI,                             ALMWB_DESC_GHI,             (const char *)""        },
  {ALMWB_KEY_SOLAR_RAD,                       ALMWB_DESC_SOLAR_RAD,       (const char *)""        },
  {ALMWB_KEY_ELEV_ANGLE,                      ALMWB_DESC_ELEV_ANGLE,      (const char *)""        },
  {ALMWB_KEY_H_ANGLE,                         ALMWB_DESC_H_ANGLE,         (const char *)""        }
};
uint8_t weatherbitList_keyCount = ARRAY_SIZE(weatherbitList_key);


LList<weatherbitData *> _weatherbitData;

weatherbitData::weatherbitData(const char *  search){
  for( int j = 0; j < weatherbitList_keyCount; j++) { 

    String key = al_tools::ch_toString(weatherbitList_key[j].key);

    if ( key == al_tools::ch_toString(search) ) _keyPos = j;
  }   
}

weatherbitCurrent::weatherbitCurrent() {

}

void weatherbitCurrent::buildUrl(String & result, const String & appId, const String & locationId, const String & language) {
  result = "http://api.weatherbit.io/v2.0/current?city_id=" + locationId + "&lang=" + language + "&key=" + appId;
}


boolean weatherbitCurrent::httpget_updateData(const String & appId, const String & locationId, const String & language) {
  String url, result;

  buildUrl(url, appId, locationId, language);
  if (!httpget(url, result)) return false;
  delay(2);
  return parse(result);
}

boolean weatherbitCurrent::httpget(const String & url, String & result) {
  int code = al_httptools::get_httpdata(result, url);
  // Serial.printf_P(PSTR("[weatherbitCurrent::httpget] get_httpdata code: %d\n"), code);
  if ( code != 200) return false;
  return true;
}

boolean weatherbitCurrent::parse(const String & json){
  DynamicJsonDocument doc(2048);
  DeserializationError error =  deserializeJson(doc, json);
  ALT_TRACEC("main", "deserializeJson error: %s\n", error.c_str());
  // if (error) return false;
  JsonArray array = doc[F("data")];
  for(size_t i = 0; i < array.size(); ++i) {

      for(int j = 0; j < _weatherbitData.size(); ++j) {

        uint8_t pos = _weatherbitData[j]->_keyPos; 

        String key  = al_tools::ch_toString(weatherbitList_key[pos].key);
        String root = al_tools::ch_toString(weatherbitList_key[pos].root);

        if (root == "") {
          // weatherbitList_key[j].data = array[pos][key].as<String>();
          al_tools::c_str(_weatherbitData[j]->_data, array[i][key].as<String>());
        } else {
          // weatherbitList_key[j].data = array[pos][root][key].as<String>();
          al_tools::c_str(_weatherbitData[j]->_data, array[i][root][key].as<String>());
        }
      }

    // for( int j = 0; j < weatherbitList_keyCount; j++) { 
    //   String key  = al_tools::ch_toString(weatherbitList_key[j].key);
    //   String root = al_tools::ch_toString(weatherbitList_key[j].root);
    //   if (root == "")
    //     weatherbitList_key[j].data = array[i][key].as<String>();
    //   else {
    //     weatherbitList_key[j].data = array[i][root][key].as<String>();
    //   }
    // }

  }  
  return true;
}

/*

void weatherbitCurrent::httpget_updateData(const String & appId, const String & locationId, const String & language) {
  String url;
  buildUrl(url, appId, locationId, language);
  httpget_updateData(url);
}
void weatherbitCurrent::httpget_updateData(const String & url) {

  unsigned long       lostTest  = 10000UL;
  unsigned long       lost_do   = millis();
  WiFiClient          _client;
  HTTPClient          http;
  bool                isBody    = false;
  char                c;
  JsonStreamingParser parser;

  parser.setListener(this);

  Serial.printf_P(PSTR("Getting url: %s\n"), url.c_str());
  http.begin(_client, url);
  Serial.printf_P(PSTR("[HTTP] GET...\n"));
  int httpCode = http.GET();
  Serial.printf_P(PSTR("[HTTP] GET... code: %d\n"), httpCode);

  if(httpCode > 0) {
    WiFiClient * client = http.getStreamPtr();
    while (client->connected() || client->available()) {
      while (client->available()) {
        if ((millis() - lost_do) > lostTest) {
          Serial.printf_P(PSTR("lost in client with a timeout\n"));
          client->stop();
          ESP.restart();
        }
        c = client->read();
        if (c == '{' || c == '[') {
          isBody = true;
        }
        if (isBody) {
          parser.parse(c);
        }
        yield();
        delay(1);
      }
      client->stop();
    }
  }
}

*/


void weatherbitCurrent::print() {
  for( int i = 0; i < weatherbitList_keyCount; i++) { 
    String s = weatherbitList_key[i].key;
    String v;
    String d = weatherbitList_key[i].desc;
    // Serial.printf("%25s - %22s - %s\n", s.c_str(), v.c_str(), d.c_str());
    for(int j = 0; j < _weatherbitData.size(); ++j) {
      uint8_t pos = _weatherbitData[j]->_keyPos;    
      if (i==pos) {
        v = al_tools::ch_toString(_weatherbitData[j]->_data);
      }
    }
    Serial.printf("%25s - %22s - %s\n", s.c_str(), v.c_str(), d.c_str());    
  }     
}
void weatherbitCurrent::getKey(String & result, const String & value) {
  result = "error";
  for( int i = 0; i < weatherbitList_keyCount; i++) { 
    if(al_tools::ch_toString(weatherbitList_key[i].key) == value) {
      // result = weatherbitList_key[i].data;
      

      for(int j = 0; j < _weatherbitData.size(); ++j) {
        uint8_t pos = _weatherbitData[j]->_keyPos;    
        if (i==pos) {
          result = al_tools::ch_toString(_weatherbitData[j]->_data);
        }
        // Serial.printf("[%d] %d = %s, %s\n", i, pos, weatherbitList_key[pos].key, _weatherbitData[i]->_data); 
      }

      break;
    }
  }  
  if (result == "") result = "error"; 
}

// void weatherbitCurrent::whitespace(char c) {
// }

// void weatherbitCurrent::startDocument() {
// }

// void weatherbitCurrent::key(String key) {
//   currentKey = String(key);
// }

// void weatherbitCurrent::value(String value) {
//   for( int i = 0; i < weatherbitList_keyCount; i++) { 
//     if(al_tools::ch_toString(weatherbitList_key[i].key) == currentKey) {
//       weatherbitList_key[i].data = value;
//     }
//   }   
// }

// void weatherbitCurrent::endArray() {
// }

// void weatherbitCurrent::endObject() {
// }

// void weatherbitCurrent::endDocument() {
// }

// void weatherbitCurrent::startArray() {
// }

// void weatherbitCurrent::startObject() {
// }

#endif