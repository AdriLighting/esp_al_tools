/*
    Author: adrilighting
    from: https://docs.thingpulse.com/guides/wifi-color-display-kit/
*/

#ifdef WEATHERBIT_ENABLED

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include "../../include/meteo/weatherbit.h"
#include "../../include/altools.h"
#include "../../include/meteo/weatherapiid.h"

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

weatherbitList weatherbitList_key [] = { 
  {ALMWB_KEY_LAT,                             ALMWB_DESC_LAT                },
  {ALMWB_KEY_LON,                             ALMWB_DESC_LON                },
  {ALMWB_KEY_SUNRISE,                         ALMWB_DESC_SUNRISE                },
  {ALMWB_KEY_SUNSET,                          ALMWB_DESC_SUNSET               },
  {ALMWB_KEY_TIMEZONE,                        ALMWB_DESC_TIMEZONE               },
  {ALMWB_KEY_STATION,                         ALMWB_DESC_STATION                },
  {ALMWB_KEY_OB_TIME,                         ALMWB_DESC_OB_TIME                },
  {ALMWB_KEY_DATETIME,                        ALMWB_DESC_DATETIME               },
  {ALMWB_KEY_TS,                              ALMWB_DESC_TS               },
  {ALMWB_KEY_CITY_NAME,                       ALMWB_DESC_CITY_NAME                },
  {ALMWB_KEY_COUNTRY_CODE,                    ALMWB_DESC_COUNTRY_CODE               },
  {ALMWB_KEY_STATE_CODE,                      ALMWB_DESC_STATE_CODE               },
  {ALMWB_KEY_PRES,                            ALMWB_DESC_PRES               },
  {ALMWB_KEY_SLP,                             ALMWB_DESC_SLP                },
  {ALMWB_KEY_WIND_SPD,                        ALMWB_DESC_WIND_SPD               },
  {ALMWB_KEY_WIND_DIR,                        ALMWB_DESC_WIND_DIR               },
  {ALMWB_KEY_WIND_CDIR,                       ALMWB_DESC_WIND_CDIR                },
  {ALMWB_KEY_WIND_CDIR_FULL,                  ALMWB_DESC_WIND_CDIR_FULL               },
  {ALMWB_KEY_TEMP,                            ALMWB_DESC_TEMP               },
  {ALMWB_KEY_APP_TEMP,                        ALMWB_DESC_APP_TEMP               },
  {ALMWB_KEY_RH,                              ALMWB_DESC_RH               },
  {ALMWB_KEY_DEWPT,                           ALMWB_DESC_DEWPT                },
  {ALMWB_KEY_CLOUDS,                          ALMWB_DESC_CLOUDS               },
  {ALMWB_KEY_POD,                             ALMWB_DESC_POD                },
  {ALMWB_KEY_ICON,                            ALMWB_DESC_ICON               },
  {ALMWB_KEY_CODE,                            ALMWB_DESC_CODE               },
  {ALMWB_KEY_DESCRIPTION,                     ALMWB_DESC_DESCRIPTION                },
  {ALMWB_KEY_VIS,                             ALMWB_DESC_VIS                },
  {ALMWB_KEY_PRECIP,                          ALMWB_DESC_PRECIP               },
  {ALMWB_KEY_SNOW,                            ALMWB_DESC_SNOW               },
  {ALMWB_KEY_UV,                              ALMWB_DESC_UV               },
  {ALMWB_KEY_AQI,                             ALMWB_DESC_AQI                },
  {ALMWB_KEY_DHI,                             ALMWB_DESC_DHI                },
  {ALMWB_KEY_DNI,                             ALMWB_DESC_DNI                },
  {ALMWB_KEY_GHI,                             ALMWB_DESC_GHI                },
  {ALMWB_KEY_SOLAR_RAD,                       ALMWB_DESC_SOLAR_RAD                },
  {ALMWB_KEY_ELEV_ANGLE,                      ALMWB_DESC_ELEV_ANGLE               },
  {ALMWB_KEY_H_ANGLE,                         ALMWB_DESC_H_ANGLE                }
};
uint8_t weatherbitList_keyCount = ARRAY_SIZE(weatherbitList_key);

weatherbitCurrent::weatherbitCurrent() {

}

void weatherbitCurrent::buildUrl(String & result, const String & appId, const String & locationId, const String & language) {
  result = "http://api.weatherbit.io/v2.0/current?city_id=" + locationId + "&lang=" + language + "&key=" + appId;
}
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
void weatherbitCurrent::print() {
  for( int i = 0; i < weatherbitList_keyCount; i++) { 
    String s = weatherbitList_key[i].key;
    String v = weatherbitList_key[i].data;
    String d = weatherbitList_key[i].desc;
    Serial.printf("%25s - %22s - %s\n", s.c_str(), v.c_str(), d.c_str());
  }     
}
void weatherbitCurrent::getKey(String & result, const String & value) {
  result = "";
  for( int i = 0; i < weatherbitList_keyCount; i++) { 
    if(al_tools::ch_toString(weatherbitList_key[i].key) == value) {
      result = weatherbitList_key[i].data;
      break;
    }
  }   
}

void weatherbitCurrent::whitespace(char c) {
}

void weatherbitCurrent::startDocument() {
}

void weatherbitCurrent::key(String key) {
  currentKey = String(key);
}

void weatherbitCurrent::value(String value) {
  for( int i = 0; i < weatherbitList_keyCount; i++) { 
    if(al_tools::ch_toString(weatherbitList_key[i].key) == currentKey) {
      weatherbitList_key[i].data = value;
    }
  }   
}

void weatherbitCurrent::endArray() {
}

void weatherbitCurrent::endObject() {
}

void weatherbitCurrent::endDocument() {
}

void weatherbitCurrent::startArray() {
}

void weatherbitCurrent::startObject() {
}

#endif