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


#include "../include/alhttptimesync.h"

#ifdef ESP8266
#include <coredecls.h>                 // settimeofday_cb()
#include <TZ.h>                        // TZ declarations https://github.com/esp8266/Arduino/blob/master/cores/esp8266/TZ.h
#include <sntp.h>

#ifdef __NEWLIB__ 
#if __NEWLIB__ >= 4
    extern "C" {
        #include <sys/_tz_structs.h>
    };
#endif
#endif
#endif

#ifdef ESP32
 #include <time.h>
 #include <lwip/apps/sntp.h>
 #include "TZ32.h"
#endif

#include <ArduinoJson.h>

#include "../include/altools.h"
#include "../include/alhttptools.h"
#include "../include/aldatestring.h"

    #define ALTIME_CONNECTED (WiFi.localIP()[0] != 0 && WiFi.status() == WL_CONNECTED)


#define ALTIME_NTP1ADDRESS      "fr.pool.ntp.org"
#define ALTIME_NTP2ADDRESS      "pool.ntp.org"
#define ALTIME_TIMEAPI_BUFSIZE  600
#define ALTIME_DATETIME_STRLEN  (19U)   // buffer for data/time string "YYYY-MM-DDThh:mm:ss"
#define ALTIME_TM_BASE_YEAR     1900
#define ALTIME_HTTP_REFRESH_HRS 3     // время суток для обновления часовой зоны
#define ALTIME_HTTP_REFRESH_MIN 3
#define ALTIME_DAYSECONDS       (86400U)
#define ALTIME_TASK_SECOND      1000UL
#define MYTZ PSTR("CET-1CEST,M3.5.0,M10.5.0/3")
// #define MYTZ PSTR("TZ_Etc_GMT")
/*
{
    "abbreviation": "CEST",
    "client_ip": "82.64.31.31",
    "datetime": "2022-04-12T21:03:03.699127+02:00",
    "day_of_week": 2,
    "day_of_year": 102,
    "dst": true,
    "dst_from": "2022-03-27T01:00:00+00:00",
    "dst_offset": 3600,
    "dst_until": "2022-10-30T01:00:00+00:00",
    "raw_offset": 3600,
    "timezone": "Europe/Paris",
    "unixtime": 1649790183,
    "utc_datetime": "2022-04-12T19:03:03.699127+00:00",
    "utc_offset": "+02:00",
    "week_number": 15
}
*/
static const char P_LOC[] PROGMEM = "LOC";


extern "C" int clock_gettime(clockid_t unused, struct timespec *_tp);

#define PTM(w) \
  Serial.print(" " #w "="); \
  Serial.print(tm->tm_##w);


void printTm(const char* what, const tm* tm) {
  Serial.printf_P(PSTR("%s"), what);
  PTM(isdst); PTM(yday); PTM(wday);
  PTM(year);  PTM(mon);  PTM(mday);
  PTM(hour);  PTM(min);  PTM(sec);
}

// framework-arduinoespressif8266\libraries\esp8266\examples\NTP-TZ-DST\NTP-TZ-DST.ino
/*
void showTime() {
  timeval _tv;
  timespec _tp;
  time_t _now;
  uint32_t _now_ms, _now_us;

  gettimeofday(&_tv, nullptr);
  clock_gettime(0, &_tp);
  _now = time(nullptr);
  _now_ms = millis();
  _now_us = micros();

  Serial.println();
  printTm("localtime:", localtime(&_now));
  Serial.println();
  printTm("gmtime:   ", gmtime(&_now));
  Serial.println();

  // time from boot
  Serial.print("clock:     ");
  Serial.print((uint32_t)_tp.tv_sec);
  Serial.print("s + ");
  Serial.print((uint32_t)_tp.tv_nsec);
  Serial.println("ns");

  // time from boot
  Serial.print("millis:    ");
  Serial.println(_now_ms);
  Serial.print("micros:    ");
  Serial.println(_now_us);

  // EPOCH+tz+dst
  Serial.print("gtod:      ");
  Serial.print((uint32_t)_tv.tv_sec);
  Serial.print("s + ");
  Serial.print((uint32_t)_tv.tv_usec);
  Serial.println("us");

  // EPOCH+tz+dst
  Serial.print("time:      ");
  Serial.println((uint32_t)_now);

  // timezone and demo in the future
  Serial.printf("timezone:  %s\n", getenv("TZ") ? : "(none)");

  // human readable
  Serial.print("ctime:     ");
  Serial.print(ctime(&_now));

  // lwIP v2 is able to list more details about the currently configured SNTP servers
  #if defined(ESP8266)
  for (int i = 0; i < SNTP_MAX_SERVERS; i++) {
    IPAddress sntp = *sntp_getserver(i);  
    const char* name = sntp_getservername(i);
    if (sntp.isSet()) {
      Serial.printf("sntp%d:     ", i);
      if (name) {
        Serial.printf("%s (%s) ", name, sntp.toString().c_str());
      } else {
        Serial.printf("%s ", sntp.toString().c_str());
      }
      Serial.printf("- IPv6: %s - Reachability: %o\n",
                    sntp.isV6() ? "Yes" : "No",
                    sntp_getreachability(i));
    }
  }
  #endif
  Serial.println();
}
*/




#ifdef ESP8266
bool getLocalTime(struct tm * info, uint32_t ms) {
  uint32_t count = ms / 10;
  time_t now;

  time(&now);
  localtime_r(&now, info);

  if (info->tm_year > (2016 - 1900)) {
    return true;
  }

  while (count--) {
    delay(10);
    time(&now);
    localtime_r(&now, info);
    if (info->tm_year > (2016 - 1900)) {
      return true;
    }
  }
  return false;
}
#endif

#ifndef ESP8266
namespace RTC_MEM_32 {
    RTC_DATA_ATTR RTC_DATA rtcTime;
};

using namespace RTC_MEM_32;
#endif

unsigned long RTC_Worker(unsigned long _storage=0){
  ALT_TRACEC("main", "[RTC_Worker] -\n");
  #ifdef ESP8266
    RTC_DATA rtcTime;
    uint32_t rtc_time = system_get_rtc_time();
    if(rtc_time<500000){
        rtcTime.timeBase = rtc_time;
        rtcTime.timeAcc = 0;
        rtcTime.storage = 0;
        // Serial.printf_P(PSTR("%d - %d - %lld - %d\n"), rtc_time, rtcTime.timeBase, rtcTime.timeAcc, (rtcTime.timeAcc / 1000000) / 1000);
        //ESP.rtcUserMemoryWrite(128-sizeof(RTC_DATA), (uint32_t*)&rtcTime, sizeof(RTC_DATA));
        system_rtc_mem_write(192-sizeof(RTC_DATA), &rtcTime, sizeof(RTC_DATA));
    } else {
        //ESP.rtcUserMemoryRead(128-sizeof(RTC_DATA), (uint32_t*)&rtcTime, sizeof(RTC_DATA));
        system_rtc_mem_read(192-sizeof(RTC_DATA), &rtcTime, sizeof(RTC_DATA));
        rtc_time = system_get_rtc_time();
        uint32_t cal = system_rtc_clock_cali_proc();
        rtcTime.timeAcc += ((uint64_t)(rtc_time - rtcTime.timeBase) * (((uint64_t)cal * 1000) >> 12));
        // Serial.printf_P(PSTR("%d - %d - %lld - %d\n"), rtc_time, rtcTime.timeBase, rtcTime.timeAcc, (rtcTime.timeAcc / 1000000) / 1000);
        rtcTime.timeBase = rtc_time;
        if(_storage)
            rtcTime.storage = _storage - (rtcTime.timeAcc / 1000000) / 1000;
        //ESP.rtcUserMemoryWrite(128-sizeof(RTC_DATA), (uint32_t*)&rtcTime, sizeof(RTC_DATA));
        system_rtc_mem_write(192-sizeof(RTC_DATA), &rtcTime, sizeof(RTC_DATA));
    }
    ALT_TRACEC("main", "[RTC_Worker]\n\ttimeAcc: %u \n\tstorage: %u\n\treturn: %u\n", 
      (uint32_t)(rtcTime.timeAcc / 1000000) / 1000, 
      rtcTime.storage,
      rtcTime.storage+(rtcTime.timeAcc / 1000000) / 1000);
    return rtcTime.storage+(rtcTime.timeAcc / 1000000) / 1000;
  #else
    struct timeval tv = { .tv_sec = 0, .tv_usec = 0 };   /* btw settimeofday() is helpfull here too*/
    // uint64_t sec, us;
    gettimeofday(&tv, NULL);
    // (sec) = tv.tv_sec;
    // (us) = tv.tv_usec;
    uint32_t rtc_time = tv.tv_sec;

    if(rtc_time<500000){
        rtcTime.timeBase = rtc_time;
        rtcTime.timeAcc = 0;
        rtcTime.storage = 0;
        //LOG(printf_P, PSTR("%d - %d - %lld\n"), rtc_time, rtcTime.timeBase, rtcTime.timeAcc);
    } else {
        rtcTime.timeAcc += (uint64_t)(rtc_time - rtcTime.timeBase);
        //LOG(printf_P, PSTR("%d - %d - %lld\n"), rtc_time, rtcTime.timeBase, rtcTime.timeAcc);
        rtcTime.timeBase = rtc_time;
        if(_storage)
            rtcTime.storage = _storage - rtcTime.timeAcc;
    }
    ALT_TRACEC("main", "[RTC_Worker]\n\ttimeAcc: %u \n\tstorage: %u\n\treturn: %u\n", 
      rtcTime.timeAcc, 
      rtcTime.storage,
      rtcTime.storage+rtcTime.timeAcc);
    return rtcTime.storage+rtcTime.timeAcc;
  #endif
}


AL_httpTime * AL_httpTimePtr = nullptr;
AL_httpTime::AL_httpTime(){AL_httpTimePtr = this;}

/**
 * установки системной временной зоны/правил сезонного времени.
 * по сути дублирует системную функцию setTZ, но работает сразу
 * со строкой из памяти, а не из PROGMEM
 * Может использоваться для задания настроек зоны/правил налету из
 * браузера/апи вместо статического задания Зоны на этапе компиляции
 * @param tz - указатель на строку в формате TZSET(3)
 * набор отформатированных строк зон под прогмем лежит тут
 * https://github.com/esp8266/Arduino/blob/master/cores/esp8266/TZ.h
 */
void AL_httpTime::tzsetup(const char* tz){
  // https://stackoverflow.com/questions/56412864/esp8266-timezone-issues
  ALT_TRACEC("main", "tz: %s\n", tz);
  if (!tz || !*tz)
      return;

  /*
   * newlib has issues with TZ strings with quoted <+-nn> names 
   * this has been fixed in https://github.com/esp8266/Arduino/pull/7702 for esp8266 (still not in stable as of Nov 16 2020)
   * but it also affects ESP32 and who knows when to expect a fix there
   * So let's fix such zones in-place untill core support for both platforms available
   */
  if (tz[0] == 0x3C){     // check if first char is '<'
    String _tz(tz);
    String _tzfix((char *)0);
    _tzfix.reserve(sizeof(tz)) ;
    _tzfix += FPSTR(P_LOC);
    if (_tz.indexOf('<',1) > 0){  // there might be two <> quotes
    //LOG(print, "2nd pos: "); LOG(println, _tz.indexOf('<',1)); 
      _tzfix += _tz.substring(_tz.indexOf('>')+1, _tz.indexOf('<',1));
      _tzfix += FPSTR(P_LOC);
    }
    _tzfix += _tz.substring(_tz.lastIndexOf('>')+1, _tz.length());
    setenv("TZ", _tzfix.c_str(), 1/*overwrite*/);
    ALT_TRACEC("main", "fix applied: %s\n", _tzfix.c_str());
  } else {
    setenv("TZ", tz, 1/*overwrite*/);
  }

  tzset();
  tzone = ""; // сбрасываем костыльную зону
  // tpData.usehttpzone = false;  // запрещаем использование http
  ALT_TRACEC("main", "rules changed from: %s to: %s\n", MYTZ, tz);

  unsigned long shift = RTC_Worker();
  //time_t _time = shift;
  //timeval tv = { _time, 0 };
  //settimeofday(&tv, NULL);
  String dt; AL_timeHelper::getDateTimeString(dt, shift);
  ALT_TRACEC("main", "reboot time (%lu)-> %s\n", (unsigned long)shift, dt.c_str());
}
boolean AL_httpTime_handle = false;
void AL_httpTime::handle(){
  if (ntpResync_task.activate) {
    if (( millis() - ntpResync_task.time ) > 10000) {
      ntpResync_run();
      ntpResync_task.activate = false;
    }
  }
  if (ALTIME_CONNECTED && !AL_timeHelper::sntpIsSynced()) {get_timeHTTP();_get_timeHTTP=true;}
  if (ALTIME_CONNECTED && !_get_timeHTTP) {get_timeHTTP();_get_timeHTTP=true;}
}

void AL_httpTime::ntpResync_start(){
  if(!AL_timeHelper::sntpIsSynced() ){
    #ifndef ESP32
      sntp_setoperatingmode(SNTP_OPMODE_POLL);
    #endif 
    ntpResync_task.time = millis();
    ntpResync_task.activate = true; 
  }

}
void AL_httpTime::ntpResync_run(){
  ALT_TRACEC("main", "-\n");
  if((!AL_timeHelper::sntpIsSynced() ) && ntpcnt){
    const char *to;
    switch(ntpcnt){
      case 1:   to = ALTIME_NTP1ADDRESS;  break;
      case 2:   to = ALTIME_NTP2ADDRESS;  break;
      case 3:   to = ntp2.c_str();        break;
      default:  to = ALTIME_NTP1ADDRESS;  break;
    }
    ALT_TRACEC("main", "switching NTP[%d] server from %s to %s\n", ntpcnt, String(sntp_getservername(0)).c_str(), to); // странное преобразование, но почему-то без него бывают ребуты...
    sntp_stop();
    sntp_setservername(0, (char *)to);
    sntp_init();
    ntpcnt++; ntpcnt%=4;
    // ts.getCurrentTask()->restartDelayed(TASK_SECOND*10);
    return;
  } else {
    if(!ntpcnt){
      unsigned long shift = RTC_Worker();
      time_t _time = shift;
      timeval tv = { _time, 0 };
      settimeofday(&tv, NULL);
      String dt; AL_timeHelper::getDateTimeString(dt);
      ALT_TRACEC("main", "Get time from RTC (%lu)-> %s\n", (unsigned long)shift, dt.c_str());
    }
  }
}

/**
 * обратный вызов при подключении к WiFi точке доступа
 * запускает синхронизацию времени
 */
#ifdef ESP8266
void AL_httpTime::onSTAGotIP(WiFiEventStationModeGotIP ipInfo)
{
      Serial.printf_P(PSTR(">>>>>>>>>>>>>>>>>> onSTAGotIP UI WiFi: IP: %s\n"), ipInfo.ip.toString().c_str());

    if(AL_timeHelper::sntpIsSynced() ) return;

    sntp_init();
    ntpResync_start();
    // if (ALT_debugPrint("main")) showTime();
}
void AL_httpTime::onSTADisconnected(WiFiEventStationModeDisconnected ipInfo)
{
   Serial.printf(">>>>>>>>>>>>>>>>>> onSTADisconnected\n");

    sntp_stop();
    if(ntpResync_task.activate){
      ntpResync_task.activate = false;
    }
}
#endif  //ESP8266



void AL_httpTime::set_time(String str, uint32_t lastUpdate) {
  ALT_TRACEC("main", "-\n");
  int   s_year  = str.substring(0,4).toInt();
  int   s_mon   = str.substring(5,7).toInt();
  int   s_day   = str.substring(8,10).toInt();
  int   s_hr    = str.substring(11,13).toInt();
  int   s_min   = str.substring(14,16).toInt();
  int   s_sec   = str.substring(17,19).toInt();

  // char buff[120];
  // sprintf(buff, "timeFromEsp_set: %d/%d/%d %d:%d:%d\n", s_year, s_mon, s_day, s_hr, s_min, s_sec);
  // Serial.println(buff);

  #ifdef ALTIME_USE_TIMELIB
    TimeElements myTimeElements;
    myTimeElements.Year   = ( s_year-1970);
    myTimeElements.Month  = s_mon;
    myTimeElements.Day    = s_day;
    myTimeElements.Hour   = s_hr;
    myTimeElements.Minute = s_min;
    myTimeElements.Second = s_sec;
    time_t t = makeTime(myTimeElements);  
  #endif

  struct tm t2;
  tm *tm      = &t2;
  tm->tm_year = s_year - 1900;
  tm->tm_mon  = s_mon - 1;
  tm->tm_mday = s_day;
  tm->tm_hour = s_hr;
  tm->tm_min  = s_min;
  tm->tm_sec  = s_sec;
  timeval tv  = { mktime(tm), 0 };

  ALT_TRACEC("main", ">>> settimeofday\n");
  settimeofday(&tv, NULL);


  #ifdef ALTIME_USE_TIMELIB
    ALT_TRACEC("main", ">>> TimeLib.h setTime+adjustTime]\n");
    setTime(t);  
    adjustTime(( (millis() - lastUpdate) / 1000 ) );   
  #endif 

  RTC_Worker(tv.tv_sec);    
}

void AL_httpTime::get_timeHTTP() {
  yield();
  ALT_TRACEC("main", "-\n");
  String    buf       = "";
  uint32_t  lastcall  = millis();

  String result((char *)0);
  result.reserve(ALTIME_TIMEAPI_BUFSIZE);

  sntp_init();
  ntpResync_start();

  if(tzone.length()){
    ALT_TRACEC("main", "sntp is sync\n");
    String url(FPSTR(ALTIME_PT_timeapi_tz_url));
    url+=tzone;
    al_httptools::get_httpdata(result, url);
    ALT_TRACEC("main", "\n\tresult >>>\n%s\n\tresult <<<\n", result.c_str());
  } else ALT_TRACEC("main", "sntp isnt sync\n");

  if(!result.length()){
    ALT_TRACEC("main", "[ERR] size of ALTIME_PT_timeapi_tz_url data: %d - try with ALTIME_PT_timeapi_ip_url\n", result.length());
    String url(FPSTR(ALTIME_PT_timeapi_ip_url));
    if(!al_httptools::get_httpdata(result, url)) {
      ALT_TRACEC("main", "[ERR] size of ALTIME_PT_timeapi_ip_url data: %d\n", result.length());
      return;
    }
    ALT_TRACEC("main", "\n\tresult >>>\n%s\n\tresult <<<\n", result.c_str());  
  }

  DynamicJsonDocument doc(ALTIME_TIMEAPI_BUFSIZE);
  DeserializationError error = deserializeJson(doc, result);
  result="";

  if (error) {
    ALT_TRACEC("main", "Time deserializeJson error: ");
    #ifdef ALT_DEBUG_TARCE
      Serial.println( error.code());  
    #endif
    return;
  }



  String datetime = doc[F("datetime")];

  int raw_offset, dst_offset = 0;

  raw_offset=doc[F("raw_offset")];    // по сути ничего кроме текущего смещения от UTC от сервиса не нужно
                                      // правила перехода сезонного времени в формате, воспринимаемом системной
                                      // либой он не выдает, нужно писать внешний парсер. Мнемонические определения
                                      // слишком объемные для контроллера чтобы держать и обрабатывать их на лету.
                                      // Вероятно проще будет их запихать в js веб-интерфейса
  dst_offset=doc[F("dst_offset")];

  // Save mnemonic time-zone (do not know why :) )
  if (!tzone.length()) {
    const char *tz = doc[F("timezone")];
    tzone+=tz;
  }

  ALT_TRACEC("main", "Reading JSON:\n\t[HTTP TimeZone: %s]\n\t[timezone: %d]\n\t[dst_offset: %d]\n\t[datetime %s]\n", 
    tzone.c_str(), raw_offset, dst_offset, datetime.c_str());

  AL_timeHelper::getDateTimeString(buf, 0);
  ALT_TRACEC("main", "get datetime from: time - localtime\n\t[timestamp: %s]\n\t[sntpIsSynced: %d]\n", buf.c_str(), AL_timeHelper::sntpIsSynced());

  ALT_TRACEC("main", ">>> set_timezone\n");
  // setenv("TZ", tzone.c_str(),1);
  // sntp_set_timezone_in_seconds(raw_offset+dst_offset);
  int val = raw_offset+dst_offset;
  #if defined(ESP8266)
    sntp_set_timezone_in_seconds(val);
  #elif defined(ESP32)
    //setTimeZone((long)val, 0);    // this breaks linker in some weird way
    //configTime((long)val, 0, ntp0.c_str(), ntp1.c_str(), "");
    configTime((long)val, 0, ALTIME_NTP1ADDRESS, ALTIME_NTP2ADDRESS, "");
  #endif

  set_time(datetime, lastcall);


  struct tm timeinfo;
  #if defined(ESP32)
    if(!getLocalTime(&timeinfo)){
      ALT_TRACEC("main", "!!! [FAILED TO OBTAIN TIME] !!!");
    }
  #elif defined(ESP8266)
    if(!getLocalTime(&timeinfo, 5000)){
      ALT_TRACEC("main", "!!! [FAILED TO OBTAIN TIME] !!!");
    }
  #endif

  buf = "";
  AL_timeHelper::getDateTimeString(buf, 0);
  ALT_TRACEC("main", "get datetime from: time - localtime\n\t[timestamp: %s]\n\t[sntpIsSynced: %d]\n", buf.c_str(), AL_timeHelper::sntpIsSynced());

  if (doc[F("dst_from")]!=nullptr){
    ALT_TRACEC("main", "Zone has DST, rescheduling refresh\n");
    uint32_t tDelay;
    httprefreshtimer(0, tDelay);
    String sDelay = "";
    al_tools::on_time_h(tDelay, sDelay);
    ALT_TRACEC("main", "[next request]\n\t%d\n\t%s\n", tDelay, sDelay.c_str());
  }
  
  yield();
  // showTime() ;
}

void AL_httpTime::httprefreshtimer(const uint32_t delay, uint32_t &result){
  // if (tpData.usehttpzone && _httpTask){
  //     _httpTask->disable();
  //     return;
  // }

  time_t timer;

  if (delay){
    timer = delay;
  } else {
    struct tm t;
    tm *tm=&t;
    time_t tnow = time(nullptr);
    localtime_r(&tnow, tm);

    tm->tm_mday++;                  // выставляем "завтра"
    tm->tm_hour= ALTIME_HTTP_REFRESH_HRS;
    tm->tm_min = ALTIME_HTTP_REFRESH_MIN;

    timer = (mktime(tm) - getUnixTime()) % ALTIME_DAYSECONDS;

    ALT_TRACEC("main", "Schedule TZ refresh in %ld\n", timer);
  }

  // if(!_httpTask){
  //     #if defined(PIO_FRAMEWORK_ARDUINO_MMU_CACHE16_IRAM48_SECHEAP_SHARED) && defined(ALTIME_DAYSECONDS)
  //         HeapSelectIram ephemeral;
  //     #endif
      // _httpTask = new Task(timer * TASK_SECOND, TASK_ONCE, nullptr, &ts, false, nullptr, [this](){get_timeHTTP(); TASK_RECYCLE; _httpTask=nullptr;});
  // }
  // _httpTask->restartDelayed();
  result = timer * ALTIME_TASK_SECOND; 
}



String _cb_getFormattedTime() {
  unsigned long hours = AL_timeHelper::get_hours();
  String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

  unsigned long minutes = AL_timeHelper::get_minutes();
  String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

  unsigned long seconds = AL_timeHelper::get_seconds();
  String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);

  return hoursStr + ":" + minuteStr + ":" + secondStr;
} 

AL_getTime::AL_getTime(){
  set_cb_time( +[](){return AL_timeHelper::get_mday();},    0);  
  set_cb_time( +[](){return AL_timeHelper::get_hours();},   1);
  set_cb_time( +[](){return AL_timeHelper::get_minutes();}, 2);
  set_cb_time( +[](){return AL_timeHelper::get_seconds();}, 3);
  set_cb_time( +[](){return AL_timeHelper::get_month();},   4);
  set_cb_getYear( +[](){return AL_timeHelper::get_year();} );  
  set_cb_getFormattedTime( &_cb_getFormattedTime );  
  set_cb_isTimeSet( +[](){return AL_timeHelper::sntpIsSynced();} );
}



const time_t* AL_timeHelper::now(){
  static time_t _now;
  time(&_now);
  return &_now;
}
bool AL_timeHelper::sntpIsSynced() {
    time_t now;
    tm *timeinfo;
    bool rc;

    time(&now);
    timeinfo = localtime(&now);
  //LOG(printf_P, PSTR("NTP: timeinfo->tm_year=%d\n"), timeinfo->tm_year);

  if (timeinfo->tm_year < (2000 - 1900)) {
    rc = false;
  } else {
    rc = true;
  }

  return rc;
}

bool AL_timeHelper::get_dst(const time_t _tstamp) {
  return localtime(_tstamp? &_tstamp : now())->tm_isdst;
}
uint8_t AL_timeHelper::get_hours(const time_t _tstamp) {
  return localtime(_tstamp? &_tstamp : now())->tm_hour;
}
uint8_t AL_timeHelper::get_minutes(const time_t _tstamp) {
  return localtime(_tstamp? &_tstamp : now())->tm_min;
}
uint8_t AL_timeHelper::get_seconds(const time_t _tstamp) {
  return localtime(_tstamp? &_tstamp : now())->tm_sec;
}
uint8_t AL_timeHelper::get_mday(const time_t _tstamp) {
  return localtime(_tstamp? &_tstamp : now())->tm_mday;
}
uint8_t AL_timeHelper::get_wday(const time_t _tstamp) {
  return localtime(_tstamp? &_tstamp : now())->tm_wday;
}
uint8_t AL_timeHelper::get_yday(const time_t _tstamp) {
  return localtime(_tstamp? &_tstamp : now())->tm_yday;
}
uint8_t AL_timeHelper::get_month(const time_t _tstamp) {
  return localtime(_tstamp? &_tstamp : now())->tm_mon+1;
}
uint32_t AL_timeHelper::get_year(const time_t _tstamp) {
  return localtime(_tstamp? &_tstamp : now())->tm_year+ ALTIME_TM_BASE_YEAR ;
}

void AL_timeHelper::incrementCurrentTime(time_t & result, const time_t timeStamp, const int day, const int hr, const int min, const int sec){
  struct tm t;
  tm *tm = &t;
  time_t tnow = timeStamp ? timeStamp : time(nullptr);
  incrementCurrentTime(tm, tnow, day, hr, min, sec);
  result = mktime(&t);
}
void AL_timeHelper::incrementCurrentTime(struct tm * tm, const time_t timeStamp, const int day, const int hr, const int min, const int sec){
  time_t tnow = timeStamp ? timeStamp : time(nullptr);
  localtime_r(&tnow, tm);  
  tm->tm_mday += day;     
  tm->tm_hour += hr;  
  tm->tm_min  += min;  
  tm->tm_sec  += sec;  
}

// https://github.com/DmytroKorniienko/EmbUI/blob/main/src/timeProcessor.cpp
/**
 * функция допечатывает в переданную строку передайнный таймстамп даты/времени в формате "9999-99-99T99:99"
 * @param _tstamp - преобразовать заданный таймстамп, если не задан используется текущее локальное время
 */
void AL_timeHelper::getDateTimeString(String &buf, const time_t _tstamp){
  char      tmpBuf[ALTIME_DATETIME_STRLEN+1];
  time_t    t     = time(nullptr);
  const tm  * tm  = localtime(  _tstamp ? &_tstamp : &t );
  sprintf_P(tmpBuf,PSTR("%04u-%02u-%02uT%02u:%02u:%02u"), tm->tm_year + ALTIME_TM_BASE_YEAR, tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
  buf.concat(tmpBuf);
}
void AL_timeHelper::getDateTimeShortString(String &buf, const time_t _tstamp){
  char      tmpBuf[ALTIME_DATETIME_STRLEN+1];
  time_t    t     = time(nullptr);
  const tm  * tm  = localtime(  _tstamp ? &_tstamp : &t );
  sprintf_P(tmpBuf,PSTR("%02u:%02u:%02u"), tm->tm_hour, tm->tm_min, tm->tm_sec);
  buf.concat(tmpBuf);
}
void AL_timeHelper::getDateString(String &buf, const time_t _tstamp){
  char      tmpBuf[512];
  time_t    t     = time(nullptr);
  const tm  * tm  = localtime(  _tstamp ? &_tstamp : &t );
  sprintf_P(tmpBuf,PSTR("%s %d %s"), al_dateString_days_t[tm->tm_wday].fr, tm->tm_mday, al_dateString_month_t[tm->tm_mon].fr);
  buf.concat(tmpBuf);
}















/*


void worldclock(){


int utc_fr = 2;

String list[5] = {"Shanghai", "New York City", "London", "Kyiv", "Martinique"};
String ts_paris = "";
AL_timeHelper::getDateTimeShortString(ts_paris, 0);
Serial.printf("%-15s", "Paris");
for(int i = 0; i < 5; ++i) Serial.printf("%-15s", list[i].c_str());
Serial.printf("\n");  
Serial.printf("%-15s", ts_paris.c_str());
for(int i = 0; i < 5; ++i) {
  String utc = "";
  // getUtc(list[i], utc);

  if (!AL_httpTimePtr->getUtc(list[i],utc)) continue;
  int utc_i = utc.substring(0,3).toInt();
  // Serial.printf("%s : %d\n", list[i].c_str(), utc_i);
  if (utc_i == utc_fr) {
    Serial.printf("%-15s", ts_paris.c_str());
  } else {
    String ts_country;
    time_t ts;
    utc_i -= utc_fr;
    // Serial.printf("\t%s : %d\n", list[i].c_str(), utc_i);
    AL_timeHelper::incrementCurrentTime(ts, 0, 0, utc_i, 0, 0);
    AL_timeHelper::getDateTimeShortString(ts_country, ts);
    Serial.printf("%-15s", ts_country.c_str());
  }

}
  Serial.printf("\n");

// time_t tstampBadUTC;
// AL_timeHelper::incrementCurrentTime(tstampBadUTC, 0, 0, -15, 0, 0);
// String buf = "";
// AL_timeHelper::getDateTimeString(buf, 0);
// Serial.printf("current: %s\n", buf.c_str());
// buf = "";
// AL_timeHelper::getDateTimeString(buf, tstampBadUTC);
// Serial.printf("current+10min: %s\n", buf.c_str());
// char tmpStr[100];
// sprintf(tmpStr, "%02d:%02d:%02d", _Event_time.getHours(), _Event_time.getMinutes(), _Event_time.getSeconds());
// Serial.write(27);       // ESC command
// Serial.print("[2J");    // clear screen command
// Serial.write(27);
// Serial.print("[H");      
// ALT_TRACEC("main", "%s\n", tmpStr);
}



*/