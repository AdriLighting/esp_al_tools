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