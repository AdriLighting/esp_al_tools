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
// This framework originaly based on JeeUI2 lib used under MIT License Copyright (c) 2019 Marsel Akhkamov
// then re-written and named by (c) 2020 Anton Zolotarev (obliterator) (https://github.com/anton-zolotarev)
// also many thanks to Vortigont (https://github.com/vortigont), kDn (https://github.com/DmytroKorniienko)
// and others people
// ported by adrilighting 13/04/2022 00:38:55


#ifndef _ALHTTPTIMESYNC_H
    #define _ALHTTPTIMESYNC_H
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

    #ifndef SECS_PER_MIN
    #define SECS_PER_MIN  ((time_t)(60UL))  
    #endif
    #ifndef SECS_PER_HOUR
    #define SECS_PER_HOUR ((time_t)(3600UL))  
    #endif
    #ifndef SECS_PER_DAY
    #define SECS_PER_DAY  ((time_t)(SECS_PER_HOUR * 24UL))  
    #endif
    #ifndef DAYS_PER_WEEK
    #define DAYS_PER_WEEK ((time_t)(7UL))  
    #endif
    #ifndef SECS_PER_WEEK
    #define SECS_PER_WEEK ((time_t)(SECS_PER_DAY * DAYS_PER_WEEK))  
    #endif
    #ifndef SECS_PER_YEAR
    #define SECS_PER_YEAR ((time_t)(SECS_PER_WEEK * 52UL))  
    #endif
    #ifndef SECS_YR_2000
    #define SECS_YR_2000  ((time_t)(946684800UL)) // the time at the start of y2k  
    #endif

    #define ALTIME_TIMEAPI_BUFSIZE  600

    static const char ALTIME_PT_timeapi_tz_url[] PROGMEM  = "http://worldtimeapi.org/api/timezone/";
    static const char ALTIME_PT_timeapi_ip_url[] PROGMEM  = "http://worldtimeapi.org/api/ip";


    typedef struct EzTask{
      bool      activate = false;
      uint32_t  time = 0;
    } EzTask_t;

    void showTime();


    typedef struct _RTC_DATA{
      uint64_t      timeAcc;
      unsigned long storage;
      uint32_t      timeBase;
      _RTC_DATA() {timeAcc = 0; storage = 0; timeBase = 0;}
    } RTC_DATA;

    class AL_httpTime {
        boolean _get_timeHTTP = false;

        String  tzone;
        String  ntp2;              // хранилище для ntp-сервера2 (резервный, задается с UI)
        uint8_t ntpcnt = 2;

        void set_time(String str, uint32_t lastUpdate);

        // unsigned int getHttpData(String &payload, const String &url);

        static time_t getUnixTime() {return time(nullptr); }
        void httprefreshtimer(const uint32_t delay, uint32_t&r);
        void tzsetup(const char* tz);

        EzTask ntpResync_task;
        void ntpResync_run();
        void ntpResync_start();
      public:
        AL_httpTime();
        ~AL_httpTime(){};

        // bool sntpIsSynced();

        void get_timeHTTP();

        // boolean get_timeHTTP(const String &, String&);
        // boolean getUtc(const String & search, String & result);

#ifdef ESP8266
        void onSTAGotIP(WiFiEventStationModeGotIP ipInfo);
        void onSTADisconnected(WiFiEventStationModeDisconnected event_info);    
#endif
        void handle();

    };

    class AL_timeHelper {
    public:
        AL_timeHelper(){};
        ~AL_timeHelper(){};
        static bool sntpIsSynced();
        static uint8_t get_hours(const time_t _tstamp = 0) ;
        static uint8_t get_minutes(const time_t _tstamp = 0) ;
        static uint8_t get_seconds(const time_t _tstamp = 0) ;
        static uint8_t get_mday(const time_t _tstamp = 0) ;      
        static uint8_t get_wday(const time_t _tstamp = 0) ;      
        static uint8_t get_yday(const time_t _tstamp = 0) ;      
        static uint8_t get_month(const time_t _tstamp = 0) ;      
        static uint32_t get_year(const time_t _tstamp = 0) ;      
        static bool get_dst(const time_t _tstamp = 0) ;      
        static void getDateTimeShortString(String &buf, const time_t _tstamp = 0);
        static void getDateTimeString(String &buf, const time_t _tstamp = 0);
        static void incrementCurrentTime(time_t & result, time_t timeStamp, int day, int hr, int min, int sec);
        static void incrementCurrentTime(struct tm * tm, time_t timeStamp, int day, int hr, int min, int sec);
        static const time_t* now();  
        static void getDateString(String &buf, const time_t _tstamp) ;  
    };


    class AL_getTime {
      template<class X>
      using callback_t = X(*)();    

      callback_t<uint8_t>   cb_getDay           = nullptr;
      callback_t<uint8_t>   cb_getHours         = nullptr;
      callback_t<uint8_t>   cb_getMinutes       = nullptr;
      callback_t<uint8_t>   cb_getSeconds       = nullptr;
      callback_t<uint32_t>  cb_getYear          = nullptr;
      callback_t<uint8_t>   cb_getMonth         = nullptr;
      callback_t<bool>      cb_isTimeSet        = nullptr;
      callback_t<String>    cb_getFormattedTime = nullptr;
    public:
      AL_getTime()          ;
      ~AL_getTime()         {};

      template<class X>
      void set_cb_time(callback_t<X> f, uint8_t pos) {
        switch (pos) {
          case 0: cb_getDay           = std::move(f); break;
          case 1: cb_getHours         = std::move(f); break;
          case 2: cb_getMinutes       = std::move(f); break;
          case 3: cb_getSeconds       = std::move(f); break;
          case 4: cb_getMonth         = std::move(f); break;

          default:break;
        }
      }
      template<class X>
      void set_cb_isTimeSet(callback_t<X> f) {cb_isTimeSet = std::move(f);}
      template<class X>
      void set_cb_getFormattedTime(callback_t<X> f) {cb_getFormattedTime = std::move(f);}
      template<class X>
      void set_cb_getYear(callback_t<X> f) {cb_getYear = std::move(f);}

      uint8_t getDay()            {if (cb_getDay)     return cb_getDay();     else return 0;};
      uint8_t getHours()          {if (cb_getHours)   return cb_getHours();   else return 0;};
      uint8_t getMinutes()        {if (cb_getMinutes) return cb_getMinutes(); else return 0;};
      uint8_t getSeconds()        {if (cb_getSeconds) return cb_getSeconds(); else return 0;};
      uint8_t getMonth()          {if (cb_getMonth)   return cb_getMonth();   else return 0;};
      uint32_t getYear()          {if (cb_getYear)    return cb_getYear();    else return 0;};
      bool    isTimeSet()         {if (cb_isTimeSet)  return cb_isTimeSet();  else return false;};
      String  getFormattedTime()  {if (cb_getFormattedTime)  return cb_getFormattedTime(); else return "";};
      
    };  
#endif // _ALNETTIME_H