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

#include "../include/alworldclock.h"
#include <ArduinoJson.h>

#include "../include/altools.h"
#include "../include/alhttptools.h"
#include "../include/alhttptimesync.h"

/*
{
    "items":
    [
        {
            "timezone": "Europe/Paris",
            "name": "Paris",
            "country": "France"
        },
        {
            "timezone": "Asia/Shanghai",
            "name": "Shanghai",
            "country": "Chine"
        },
        {
            "timezone": "Europe/Istanbul",
            "name": "Istanbul",
            "country": "Turquie"
        },
        {
            "timezone": "America/Argentina/Buenos_Aires",
            "name": "Buenos Aires",
            "country": "Argentine"
        },
        {
            "timezone": "America/Mexico_City",
            "name": "Mexico City",
            "country": "Mexique"
        },
        {
            "timezone": "Asia/Karachi",
            "name": "Karachi",
            "country": "Pakistan"
        },
        {
            "timezone": "Europe/Moscow",
            "name": "Moscou",
            "country": "Russie"
        },
        {
            "timezone": "Europe/Kiev",
            "name": "Kyiv",
            "country": "Ukraine"
        },
        {
            "timezone": "Asia/Urumqi",
            "name": "Asia/Urumqi",
            "country": "Asia/Urumqi"
        },
        {
            "timezone": "Asia/Dhaka",
            "name": "Dhaka",
            "country": "Bangladesh"
        },
        {
            "timezone": "Asia/Seoul",
            "name": "Seoul",
            "country": "Corée du Sud"
        },
        {
            "timezone": "America/Sao_Paulo",
            "name": "São Paulo",
            "country": "Brésil"
        },
        {
            "timezone": "Africa/Lagos",
            "name": "Lagos",
            "country": "Nigéria"
        },
        {
            "timezone": "Asia/Jakarta",
            "name": "Jakarta",
            "country": "Indonésie"
        },
        {
            "timezone": "Asia/Tokyo",
            "name": "Tokyo",
            "country": "Japon"
        },
        {
            "timezone": "America/Martinique",
            "name": "Martinique",
            "country": "Martinique"
        },
        {
            "timezone": "America/New_York",
            "name": "New York City",
            "country": "États-Unis (USA)"
        },
        {
            "timezone": "Asia/Hong_Kong",
            "name": "Hong Kong",
            "country": "Hong Kong"
        },
        {
            "timezone": "America/Lima",
            "name": "Lima",
            "country": "Pérou"
        },
        {
            "timezone": "Africa/Cairo",
            "name": "Caire",
            "country": "Égypte"
        },
        {
            "timezone": "America/Bogota",
            "name": "Bogotá",
            "country": "Colombie"
        },
        {
            "timezone": "Europe/London",
            "name": "London",
            "country": "Royaume-Uni"
        },
        {
            "timezone": "Asia/Baghdad",
            "name": "Baghdad",
            "country": "Irak"
        },
        {
            "timezone": "Asia/Tehran",
            "name": "Téhéran",
            "country": "Iran"
        },
        {
            "timezone": "Asia/Karachi",
            "name": "Karachi",
            "country": "Pakistan"
        },
        {
            "timezone": "America/Argentina/Rio_Gallegos",
            "name": "Río Gallegos",
            "country": "Argentine"
        },
        {
            "timezone": "Europe/Berlin",
            "name": "Berlin",
            "country": "Allemagne"
        },
        {
            "timezone": "Australia/Sydney",
            "name": "Sydney",
            "country": "Australie"
        },
        {
            "timezone": "America/Detroit",
            "name": "Detroit",
            "country": "États-Unis (USA)"
        },
        {
            "timezone": "Europe/Madrid",
            "name": "Madrid",
            "country": "Espagne"
        },
        {
            "timezone": "Europe/Amsterdam",
            "name": "Amsterdam",
            "country": "Pays-Bas"
        }
    ]
}
*/
const char ALTIME_COUNTRY[] PROGMEM = R"rawliteral({"items":[{"timezone":"Europe/Paris","name":"Paris","country":"France"},{"timezone":"Asia/Shanghai","name":"Shanghai","country":"Chine"},{"timezone":"Europe/Istanbul","name":"Istanbul","country":"Turquie"},{"timezone":"America/Argentina/Buenos_Aires","name":"Buenos Aires","country":"Argentine"},{"timezone":"America/Mexico_City","name":"Mexico City","country":"Mexique"},{"timezone":"Asia/Karachi","name":"Karachi","country":"Pakistan"},{"timezone":"Europe/Moscow","name":"Moscou","country":"Russie"},{"timezone":"Europe/Kiev","name":"Kyiv","country":"Ukraine"},{"timezone":"Asia/Urumqi","name":"Asia/Urumqi","country":"Asia/Urumqi"},{"timezone":"Asia/Dhaka","name":"Dhaka","country":"Bangladesh"},{"timezone":"Asia/Seoul","name":"Seoul","country":"Corée du Sud"},{"timezone":"America/Sao_Paulo","name":"São Paulo","country":"Brésil"},{"timezone":"Africa/Lagos","name":"Lagos","country":"Nigéria"},{"timezone":"Asia/Jakarta","name":"Jakarta","country":"Indonésie"},{"timezone":"Asia/Tokyo","name":"Tokyo","country":"Japon"},{"timezone":"America/Martinique","name":"Martinique","country":"Martinique"},{"timezone":"America/New_York","name":"New York City","country":"États-Unis (USA)"},{"timezone":"Asia/Hong_Kong","name":"Hong Kong","country":"Hong Kong"},{"timezone":"America/Lima","name":"Lima","country":"Pérou"},{"timezone":"Africa/Cairo","name":"Caire","country":"Égypte"},{"timezone":"America/Bogota","name":"Bogotá","country":"Colombie"},{"timezone":"Europe/London","name":"London","country":"Royaume-Uni"},{"timezone":"Asia/Baghdad","name":"Baghdad","country":"Irak"},{"timezone":"Asia/Tehran","name":"Téhéran","country":"Iran"},{"timezone":"Asia/Karachi","name":"Karachi","country":"Pakistan"},{"timezone":"America/Argentina/Rio_Gallegos","name":"Río Gallegos","country":"Argentine"},{"timezone":"Europe/Berlin","name":"Berlin","country":"Allemagne"},{"timezone":"Australia/Sydney","name":"Sydney","country":"Australie"},{"timezone":"America/Detroit","name":"Detroit","country":"États-Unis (USA)"},{"timezone": "Europe/Madrid","name": "Madrid","country": "Espagne"},{"timezone": "Europe/Amsterdam","name": "Amsterdam","country": "Pays-Bas"}]})rawliteral"; 

WorldClock _WorldClock;


/*
 *

boolean AL_httpTime::getUtc(const String & search, String & result) {
  String country = FPSTR(ALTIME_COUNTRY);
  DynamicJsonDocument doc(3072);
  DeserializationError error = deserializeJson(doc, country);
  if (!error) {
    JsonArray arr = doc[F("items")];
    for(size_t i = 0; i < arr.size(); ++i) {
      JsonObject obj      = arr[i];
      String name         = obj[F("name")];
      String timezone     = obj[F("utc_offset")];
      if (name == search) {
        result = "";
        if (get_timeHTTP(timezone, result)) return true;
      }
    }
  } else {Serial.println("ERROR deserializeJson: ");} 
  return false; 
}

*/
/*

*/
/*
  // Serial.println();
  // String country = FPSTR(ALTIME_COUNTRY);
  // Serial.printf("country\n%s\n", country.c_str());
  // DynamicJsonDocument doc(3072);

  // DeserializationError error = deserializeJson(doc, country);
  // if (!error) {
  //   JsonArray arr = doc[F("items")];
  //     Serial.printf_P(PSTR("[%-3d] %-35S %-18s %-20S\n"), 0, 
  //       // "day_of_week",
  //       // "day_of_year",
  //       "timezone",
  //       "name",
  //       "country"); 
  //   for(size_t i = 0; i < arr.size(); ++i) {
  //     JsonObject obj      = arr[i];
  //     // String day_of_week  = obj[F("day_of_week")];
  //     // String day_of_year  = obj[F("day_of_year")];
  //     String timezone     = obj[F("timezone")];
  //     String name         = obj[F("name")].as<String>();
  //     String country      = obj[F("country")];
  //     Serial.printf_P(PSTR("[%-3d] %-35S %-18s %-20S\n"), i,
  //       timezone.c_str(),
  //       name.c_str(),
  //       country.c_str());
  //   }
  // } else {Serial.println("ERROR deserializeJson: ");}

  // _WorldClock.setup();
  */

WorldClockItem::WorldClockItem(const char * name, const char * tz, const char * country, const char * utc){
  size_t len;
  if (!_name) {
    len = strlen(name);
    _name = new char[len + 1];
    strcpy(_name, name);
  }
  if (!_tz) {
    len = strlen(tz);
    _tz = new char[len + 1];
    strcpy(_tz, tz);
  }
  if (!_country) {
    len = strlen(country);
    _country = new char[len + 1];
    strcpy(_country, country);
  }
  if (!_utc_offset) {
    len = strlen(utc);
    _utc_offset = new char[len + 1];
    strcpy(_utc_offset, utc);
  }

}
WorldClockItem::~WorldClockItem(){
  if (_name)        delete _name;
  if (_tz)          delete _tz;
  if (_country)     delete _country;
  if (_utc_offset)  delete _utc_offset;
}
String WorldClockItem::get_name() {return al_tools::ch_toString(_name);}

void WorldClock::add(const char * name, const char * tz, const char * country, const char * utc) {
  _list.add(new WorldClockItem(name, tz, country, utc)); 
}
// _WorldClock.add(name.c_str(), timezone.c_str(), country.c_str());

boolean WorldClock::get_timeHTTP_TZDB(const String & timezone, String & ret) {
  if (!_timezoneDB_key) return false; 

  String result((char *)0);
  result.reserve(ALTIME_TIMEAPI_BUFSIZE);

  if(timezone.length()){
    String timezoneDB_key = "";
    _WorldClock.get_timezoneDB_key(timezoneDB_key);
    String url(FPSTR(ALTIME_PT_timezonedb_tz_url));
    url += "&key=";
    url += timezoneDB_key;
    url += "&format=json&by=zone";
    url += "&zone=" + timezone;
    al_httptools::get_httpdata(result, url);
    // Serial.printf_P(PSTR("[0] getHttpData\n\turl: %s\n\tresult >>>\n%s\n\tresult <<<\n"), url.c_str(), result.c_str());
  } else return false;

  if(!result.length()){
    return false; 
  }

  DynamicJsonDocument doc(ALTIME_TIMEAPI_BUFSIZE);
  DeserializationError error = deserializeJson(doc, result);
  result="";

  if (error) {
    Serial.printf_P( PSTR("[get_timeHTTP_TZDB] Time deserializeJson error: %s\n"), error.c_str());
    return false;
  }

  if ( doc[F("status")].as<String>() == "FAILED") return false;

  String utc_offset_2;
  String utc_offset = doc[F("gmtOffset")].as<String>(); 
  String utc_dir    = utc_offset.substring(0, 1);

  if (utc_dir=="-") {utc_offset_2 = utc_offset.substring(1,utc_offset.length());}
  else              {utc_offset_2 = utc_offset; utc_dir = "+";}

  int utc_offset_3 = utc_offset_2.toInt() / 3600;
  
  if (utc_offset_3 >= 10) ret = utc_dir + String(utc_offset_3) + ":00";
  else                    ret = utc_dir + "0" + String(utc_offset_3) + ":00";

  Serial.printf_P(PSTR("[get_timeHTTP_TZDB]\n\tgmtOffset_1: %s\n\tgmtOffset_2: %s\n\tdir: %s\n\tgmtOffset/3600: %d\n\tresult: %s\n"), 
    utc_offset.c_str(), utc_offset_2.c_str(), utc_dir.c_str(), utc_offset_3, ret.c_str());

  delay(1000);
  return true;
}

boolean WorldClock::get_timeHTTP(const String & timezone, String & ret) {
  String result((char *)0);
  result.reserve(ALTIME_TIMEAPI_BUFSIZE);

  if(timezone.length()){
    String url(FPSTR(ALTIME_PT_timeapi_tz_url));
    url+=timezone;
    if (al_httptools::get_httpdata(result, url) == HTTP_CODE_OK)
      Serial.printf_P(PSTR("[get_timeHTTP] getHttpData\n\turl: %s\n\tresult >>>\n%s\n\tresult <<<\n"), url.c_str(), result.c_str());
    else return false;
  } else return false;

  if(!result.length()){
    return false; 
  }

  DynamicJsonDocument doc(ALTIME_TIMEAPI_BUFSIZE);
  DeserializationError error = deserializeJson(doc, result);
  result="";

  if (error) {
    Serial.printf_P( PSTR("[get_timeHTTP] Time deserializeJson error: %s\n"), error.c_str());
    return false;
  }

  ret = doc[F("utc_offset")].as<String>();

  Serial.println(ret);

  return true;
}

void WorldClock::setup_default(){

  String country = FPSTR(ALTIME_COUNTRY);
  DynamicJsonDocument doc(3072);
  DeserializationError error = deserializeJson(doc, country);
  if (error) return;

  JsonArray arr     = doc[F("items")];
  String    list[5] = {"Shanghai", "New York City", "London", "Kyiv", "Martinique"};

  for(int i = 0; i < 5; ++i) {
    for(size_t j = 0; j < arr.size(); ++j) {
      JsonObject obj      = arr[j];
      String timezone     = obj[F("timezone")];
      String name         = obj[F("name")].as<String>();
      String country      = obj[F("country")];
      if (name == list[i]) {
        String utc = "";
        if (!get_timeHTTP(timezone, utc)) continue;
        _WorldClock.add(name.c_str(), timezone.c_str(), country.c_str(), utc.c_str());
      }
    }
  }
  Serial.println("---");
  _WorldClock.print_registeredTime();
  Serial.println("---");

}
void WorldClock::new_timeByCity(const String & search){
  String country = FPSTR(ALTIME_COUNTRY);
  DynamicJsonDocument doc(4096);

  DeserializationError error = deserializeJson(doc, country);
  if (error) return;

  JsonArray arr = doc[F("items")];  
  for(size_t j = 0; j < arr.size(); ++j) {

    JsonObject obj      = arr[j];
    String timezone     = obj[F("timezone")];
    String name         = obj[F("name")].as<String>();
    String country      = obj[F("country")];

    if (name == search) {
      String utc = "";
      
      if (!get_timeHTTP(timezone, utc)) {
         if (!get_timeHTTP_TZDB(timezone, utc)) break;
      }

      _WorldClock.add(name.c_str(), timezone.c_str(), country.c_str(), utc.c_str());

      break;

    }
  }
}
void WorldClock::rem_timeByCity(const char *search){
  int8_t pos = -1;
  for(int i = 0; i < _list.size(); ++i) {
    WorldClockItem * item = _list.get(i);
    if (strcmp(item->_name, search) == 0) {
      pos = i;
      Serial.printf_P(PSTR("[%-3d] %-35S %-18s %-20S %-7S\n"), i,
        item->_tz,
        item->_name,
        item->_country, 
        item->_utc_offset
        );    
      break;  
    }
    
  }
  if (pos>=0) delete _list.remove(pos);
}
void WorldClock::get_timeByCity(const char * search, time_t & result){
  for(int i = 0; i < _list.size(); ++i) {
    WorldClockItem * item = _list.get(i);
    if (strcmp(search, item->_name) != 0) continue;
    get_timeByCity(i, result);
  }
}
void WorldClock::get_timeByCity(uint8_t p, time_t & result){
  int utc_fr = 2;
  WorldClockItem * item = _list.get(p);
  String utc = al_tools::ch_toString(item->_utc_offset);
  int utc_i = utc.substring(0,3).toInt();
  if (utc_i == utc_fr) {
    time(&result);
  } else {
    time_t ts;
    utc_i -= utc_fr;
    AL_timeHelper::incrementCurrentTime(ts, 0, 0, utc_i, 0, 0);
    result = ts;
  }
}
void WorldClock::get_timeByCity(const char * search, String & result, boolean shortTime){
  time_t ts;
  get_timeByCity(search, ts);
  if (shortTime) AL_timeHelper::getDateTimeShortString(result, ts);
  else  AL_timeHelper::getDateTimeString(result, ts);
}

WorldClockItem * WorldClock::get_item(uint8_t i) {return _list.get(i);}
WorldClockItem * WorldClock::get_itemByCity(const char * search){
  for(int i = 0; i < _list.size(); ++i) {
    WorldClockItem * item = _list.get(i);
    if (strcmp(search, item->_name) != 0) continue;
    return item;
  }
  return nullptr;
}

void WorldClock::print_times(boolean shortTime){
  int utc_fr = 2;

  String ts_paris = "";

  String result = "";
  ALT_TRACEC("main", "[ERR] size of ALTIME_PT_timeapi_tz_url data: %d - try with ALTIME_PT_timeapi_ip_url\n", result.length());
  String url(FPSTR(ALTIME_PT_timeapi_ip_url));
  if(!al_httptools::get_httpdata(result, url)) {
    ALT_TRACEC("main", "[ERR] size of ALTIME_PT_timeapi_ip_url data: %d\n", result.length());
  }
  ALT_TRACEC("main", "\n\tresult >>>\n%s\n\tresult <<<\n", result.c_str());  
  DynamicJsonDocument doc(ALTIME_TIMEAPI_BUFSIZE);
  DeserializationError error = deserializeJson(doc, result);
  if (error) {
    ALT_TRACEC("main", "Time deserializeJson error: ");
    #ifdef ALT_DEBUG_TARCE
      Serial.println( error.code());  
    #endif
  } else {
    bool dst_offset = doc[F("dst")];
    // if (!dst_offset) utc_fr -= 1;    
  }

    Serial.printf("[**************] dst: %d\n", dst_offset)


  if (shortTime) AL_timeHelper::getDateTimeShortString(ts_paris, 0);
  else AL_timeHelper::getDateTimeString(ts_paris, 0);
  Serial.printf("%-15s", "Paris");
  for(int i = 0; i < _list.size(); ++i) {WorldClockItem * item = _list.get(i);Serial.printf("%-25s", item->_name);}
  Serial.printf("\n");  
  Serial.printf("%-15s", ts_paris.c_str());
  for(int i = 0; i < _list.size(); ++i) {
    WorldClockItem * item = _list.get(i);
    String utc = al_tools::ch_toString(item->_utc_offset);
    int utc_i = utc.substring(0,3).toInt();
    if (utc_i == utc_fr) {
      Serial.printf("%-25s", ts_paris.c_str());
    } else {
      String ts_country;
      time_t ts;
      utc_i -= utc_fr;
      AL_timeHelper::incrementCurrentTime(ts, 0, 0, utc_i, 0, 0);
      if (shortTime) AL_timeHelper::getDateTimeShortString(ts_country, ts);
      else AL_timeHelper::getDateTimeString(ts_country, ts);
      Serial.printf("%-25s", ts_country.c_str());
    }
  }
  Serial.printf("\n");
}


void WorldClock::print_registeredTime(){
  Serial.printf_P(PSTR("WorldClock current list\n"));
  Serial.printf_P(PSTR("[%-3d] %-35S %-18s %-20S %-7S\n"), 0, 
    "timezone",
    "name",
    "country",
    "utc"
    );   
  for(int i = 0; i < _list.size(); ++i) {
    WorldClockItem * item = _list.get(i);
    Serial.printf_P(PSTR("[%-3d] %-35S %-18s %-20S %-7S\n"), i,
      item->_tz,
      item->_name,
      item->_country, 
      item->_utc_offset
      );    
  }
}
void WorldClock::print_avaibleTime() {
  Serial.printf_P(PSTR("WorldClock full list\n"));
  String country = FPSTR(ALTIME_COUNTRY);
  DynamicJsonDocument doc(3072);
  DeserializationError error = deserializeJson(doc, country);
  if (error) return;
  Serial.printf_P(PSTR("[%-3d] %-35S %-18s %-20S\n"), 0, 
    "timezone",
    "name",
    "country"
  );   
  JsonArray arr     = doc[F("items")];
  for(size_t j = 0; j < arr.size(); ++j) {
    JsonObject obj      = arr[j];
    String timezone     = obj[F("timezone")];
    String name         = obj[F("name")].as<String>();
    String country      = obj[F("country")];
    Serial.printf_P(PSTR("[%-3d] %-35S %-18s %-20S\n"), j,
      timezone.c_str(),
      name.c_str(),
      country.c_str()
    );    
  }
}

void WorldClock::set_timezoneDB_key(const char * const & key) {
  al_tools::c_str(_timezoneDB_key, key);
}
void WorldClock::get_timezoneDB_key(String & res) {
  res = String(_timezoneDB_key);
}