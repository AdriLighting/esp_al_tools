#ifndef ALDATESTRING_H
#define ALDATESTRING_H

	#include <Arduino.h>
	#ifdef ALTIME_USE_TIMELIB
	    #include <TimeLib.h>    
	#endif
	#if defined(ESP8266)
	    #include <ESP8266WiFi.h>
	    #include <TZ.h>                        // TZ declarations https://github.com/esp8266/Arduino/blob/master/cores/esp8266/TZ.h
	    #include <sntp.h>
	    #include <ESP8266HTTPClient.h>
	#elif defined(ESP32)
	  #include <WiFi.h>
	  #include <HTTPClient.h>
	#endif
	#ifdef ESP32
	 #include <time.h>
	 #include <lwip/apps/sntp.h>
	 #include <HTTPClient.h>
	 #include "TZ32.h"
	#endif

	struct al_dateString_days {
	  const char			* en;
	  const char			* fr;
	  const char			* shorten;
	  const char			* shortfr;
	  // #ifdef ALTIME_USE_TIMELIB
	  // 	timeDayOfWeek_t	dow;	
	  // #endif
	} ;
  struct al_dateString_month {
      const char * en;
      const char * fr;
      const char * shorten;
      const char * shortfr;
  } ;
  extern const al_dateString_days al_dateString_days_t [] PROGMEM;
  extern const al_dateString_month al_dateString_month_t [] PROGMEM;

namespace al_datestr {
	int8_t get_dow(const String & country, const String & day);
	int8_t get_dow(const String & country, const char * day);
	void get_dowStr(const String & country, const String & day, String & result);
	int8_t get_month(const String & country, const String & search);
	void get_monyhStr(const String & country, const String & search, String & result);
	int8_t get_month(const String & country, const char * search);
}



#endif // ALDATESTRING_H