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
  #ifndef WEATHERBIT_ENABLED
    #define WEATHERBIT_ENABLED
  #endif
  #ifndef WEATHERBITFORECAST_ENABLED
    #define WEATHERBITFORECAST_ENABLED
  #endif
  #ifndef ALTIMESYNC_ENABLED
    #define ALTIMESYNC_ENABLED
  #endif
  #ifndef ALWORLDCLOCK_ENABLED
    #define ALWORLDCLOCK_ENABLED
  #endif

  #ifndef FSOK
    #define FSOK
  #endif
  #ifndef USE_LITTLEF
    #define USE_LITTLEF
  #endif
  #ifndef FILESYSTEM
    #if defined(ESP8266)
      #include <LittleFS.h> 
      #define FILESYSTEM LittleFS  
    #elif defined(ESP32)
      #include <FS.h>
      #include <LITTLEFS.h>
      #define FILESYSTEM LittleFS
    #endif
  #endif
  #ifndef ALT_DEBUG_TARCE
      #define ALT_DEBUG_TARCE
  #endif

  #ifndef ALSI_ENABLED
    #define ALSI_ENABLED
  #endif

  #ifndef DEBUG_KEYBOARD
    #define DEBUG_KEYBOARD
  #endif

  #ifndef ALWC_WS_OTA
    #define ALWC_WS_OTA
  #endif
  #ifndef ALWC_WS_API
    #define ALWC_WS_API
  #endif
#endif



  #if defined(ESP8266)
    extern "C" {
      #include "user_interface.h"
    }
    #include <ESP8266WiFi.h>
  #elif defined(ESP32)
    #include <WiFi.h>
  #endif

#ifdef OPENWEATHMAP_ENABLED
    #include "../include/meteo/openweathermap.h"    
#endif     
#ifdef WEATHERBIT_ENABLED
    #include "../include/meteo/weatherbit.h"    
#endif 
#ifdef WEATHERBITFORECAST_ENABLED
    #include "../include/meteo/weatherbitforecast.h"    
#endif  
#if defined(WEATHERBIT_ENABLED) || defined(WEATHERBITFORECAST_ENABLED) || defined(OPENWEATHMAP_ENABLED)
    #include "../include/meteo/weatherapiid.h"    
#endif
 
#ifdef DEBUG_KEYBOARD
    #include "../include/alserialmenu.h"    
#endif
#include "../include/altools.h"
#include "../include/alhttptools.h"
#include "../include/alstring.h"
#ifdef ALSI_ENABLED
    #include "../include/alsysinfo.h"
#endif
#include "../include/aldatestring.h"      
#ifdef ALTIMESYNC_ENABLED
    #include "../include/alhttptimesync.h"  
#endif
#ifdef ALWORLDCLOCK_ENABLED
    #include "../include/alworldclock.h"
#endif


    