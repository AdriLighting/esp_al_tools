/*
  -DALTIMESYNC_ENABLED
  -DALTIME_USE_TIMELIB
  https://github.com/AdriLighting/espwificonnect_evo.git
*/

#include <altoolslib.h>
#include <wificonnectevo.h>

AsyncWebServer  webserver(80);
DNSServer       dnsServer;
WCEVO_manager   _WCEVO_manager("httptime", "alml1234", &dnsServer, &webserver);  
AL_httpTime     _AL_httpTime;

void setup()
{
  Serial.begin(115200);

  for(unsigned long const serialBeginTime = millis(); !Serial && (millis() - serialBeginTime > 5000); ) { }
  delay(1000);

  Serial.println();
  Serial.println("STARTUP");

  WCEVO_managerPtrGet()->set_credential("free-3C3786-EXT", "phcaadax");
  _WCEVO_manager.set_cm(WCEVO_CM_STAAP);
  _WCEVO_manager.set_cmFail(WCEVO_CF_RESET);
  _WCEVO_manager.start();
  _WCEVO_manager.print();

  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  
  AL_httpTime_getPtr()->set_tz("fr.pool.ntp.org", "pool.ntp.org");
  configTzTime("CET-1CEST,M3.5.0,M10.5.0/3", "fr.pool.ntp.org", "pool.ntp.org");
  sntp_stop(); 

/*
  EXTERNAL FUNCTIONS
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
*/    

}


uint8_t loopMod=0;
void loop()
{

  // winfi connect 
  _WCEVO_manager.handleConnection(); 

  // timeSync 
  _AL_httpTime.handle();

  // when time is sync
  if (AL_timeHelper::sntpIsSynced() ) {
    if (loopMod==0) {
      loopMod=1;
      
      uint8_t month = AL_timeHelper::get_month();
      uint8_t wday  = AL_timeHelper::get_wday();

      // print date fr && en
      Serial.println();
      Serial.println(month);
      Serial.println(al_dateString_month_t[month-1].fr);
      Serial.println(al_dateString_month_t[month-1].en);
      Serial.println(al_dateString_month_t[month-1].shortfr);
      Serial.println(al_dateString_month_t[month-1].shorten);
      Serial.println(wday);
      Serial.println(al_dateString_days_t[wday].fr);
      Serial.println(al_dateString_days_t[wday].en);
      Serial.println(al_dateString_days_t[wday].shortfr);
      Serial.println(al_dateString_days_t[wday].shorten);
      Serial.println();

      // print current time  
      char tmpStr[12];
      sprintf(tmpStr, "%02d:%02d:%02d", AL_timeHelper::get_hours(), AL_timeHelper::get_minutes(), AL_timeHelper::get_seconds());      
      Serial.printf_P(PSTR("\n%s: %s\n"), getenv("TZ") ? : "(none)", tmpStr);
      String result;
      AL_timeHelper::getDateTimeShortString(result);
      Serial.printf_P(PSTR("\n%s\n"), result.c_str());
      result = "";
      AL_timeHelper::getDateTimeString(result);
      Serial.printf_P(PSTR("\n%s\n"), result.c_str());
      result = "";
      AL_timeHelper::getDateString(result, time(nullptr));
      Serial.printf_P(PSTR("\n%s\n"), result.c_str());

      // increment time
      time_t incrementTime;
      //                                                  time base day hr  min   sec
      AL_timeHelper::incrementCurrentTime(incrementTime, 0,        0,  0,  10,   0);
      result = "";
      AL_timeHelper::getDateTimeString(result, incrementTime);   
      Serial.printf_P(PSTR("\n%s\n"), result.c_str());   
    }
  }  
  delay(0);
}