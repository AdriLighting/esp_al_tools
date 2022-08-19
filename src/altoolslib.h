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