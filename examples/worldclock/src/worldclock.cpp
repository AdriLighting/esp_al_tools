/*
  COMPILATOR PREPOCESSOR 
  	ALTIMESYNC_ENABLED
    ALWORLDCLOCK_ENABLED
*/

#define MYTZ PSTR("CET-1CEST,M3.5.0,M10.5.0/3")
#include <altoolslib.h>
#include <wificonnectevo.h>
#include <adriiot_DS18B20.h>

AsyncWebServer  webserver(80);
DNSServer       dnsServer;
WCEVO_manager   _WCEVO_manager("ex_httptime", "alml1234", &dnsServer, &webserver);  
AL_httpTime     _AL_httpTime;
AL_getTime      _Event_time;
#ifdef ESP8266
  WiFiEventHandler e1, e2;  
#endif


bool cb_timeIsSet(){return AL_timeHelper::sntpIsSynced();}

void setup()
{
  Serial.begin(115200);

  for(unsigned long const serialBeginTime = millis(); !Serial && (millis() - serialBeginTime > 5000); ) { }
  delay(1000);

  Serial.println();
  Serial.println("STARTUP");

  
  #ifdef ALT_DEBUG_TARCE
    ALT_debugBuffer = new char[1024];  
    _DebugPrintList.add("main");  
    _DebugPrintList.add(WCEVO_DEBUGREGION_WCEVO);  
    _DebugPrintList.add(WCEVO_DEBUGREGION_AP);  
    _DebugPrintList.add(WCEVO_DEBUGREGION_STA);  
  #endif 

  WCEVO_managerPtrGet()->set_credential("free-3C3786-EXT", "SSIDPASS");
  _WCEVO_manager.set_cm(WCEVO_CM_STA);
  _WCEVO_manager.set_cmFail(WCEVO_CF_RESET);
  _WCEVO_manager.start();
  _WCEVO_manager.print();

  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);

  // configTime(MYTZ, "fr.pool.ntp.org"); 
  configTzTime(MYTZ, "fr.pool.ntp.org", "pool.ntp.org");
  sntp_stop(); 

  #ifdef ESP8266
    e1 = WiFi.onStationModeGotIP(std::bind(&AL_httpTime::onSTAGotIP, &_AL_httpTime, std::placeholders::_1));
    e2 = WiFi.onStationModeDisconnected(std::bind(&AL_httpTime::onSTADisconnected, &_AL_httpTime, std::placeholders::_1));
  #endif

    _WorldClock.print_avaibleTime();   
}

uint8_t loopMod = 0;
void loop()
{
  _WCEVO_manager.handleConnection();	
  _AL_httpTime.handle();
  if (!WCEVO_CONNECTED) return;
  switch (loopMod) {
    case 0:
      {
      time_t tstampBadUTC;
        AL_timeHelper::incrementCurrentTime(tstampBadUTC, 0, 0, 10, 0, 0);
        String buf = "";
        AL_timeHelper::getDateTimeString(buf, tstampBadUTC);
        Serial.printf("current+10hr: %s\n", buf.c_str()); 
      }
      _WorldClock.new_timeByCity("Berlin");
      _WorldClock.new_timeByCity("New York City");
      _WorldClock.new_timeByCity("London");
      _WorldClock.print_registeredTime();    
      loopMod = 1;
    break;
    case 1:
      {      
        time_t t;
        _WorldClock.get_timeByCity("Berlin", t);
        Serial.printf("\nBerlin : %02d:%02d:%02d - %02d/%02d/%d\n", 
          AL_timeHelper::get_hours(t),
          AL_timeHelper::get_minutes(t),
          AL_timeHelper::get_seconds(t),
          AL_timeHelper::get_mday(t),
          AL_timeHelper::get_month(t),
          AL_timeHelper::get_year(t)
        );
      }

      {
        String ts;
        _WorldClock.get_timeByCity("New York City", ts);
        Serial.printf_P(PSTR("New York City: %s\n"), ts.c_str());
        ts="";
        _WorldClock.get_timeByCity("New York City", ts, false);
        Serial.printf_P(PSTR("New York City: %s\n"), ts.c_str());
      }
      _WorldClock.print_times();
      loopMod = 2;
    break;
    default:
    break;
  }  
  delay(2);
}