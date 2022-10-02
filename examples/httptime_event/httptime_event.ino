/*
  -DALTIMESYNC_ENABLED
  -DALTIME_USE_TIMELIB

*/

#define MYTZ PSTR("CET-1CEST,M3.5.0,M10.5.0/3")

#include <altoolslib.h>
#include <wificonnectevo.h>
#include <esp8266_events.h>


AsyncWebServer  webserver(80);
DNSServer       dnsServer;
WCEVO_manager   _WCEVO_manager("ex_httptime", "alml1234", &dnsServer, &webserver);  
AL_httpTime     _AL_httpTime;
AL_getTime      _Event_time;
#ifdef ESP8266
  WiFiEventHandler e1, e2;  
#endif
EventManager    * _EventManager;

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

  WCEVO_managerPtrGet()->set_credential("free-3C3786-EXT", "phcaadax");
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

  _Event_time.set_cb_isTimeSet( &cb_timeIsSet );
  _EventManager = new EventManager(&_Event_time, 1);
  _EventManager->set_activate(0, false); 
}
time_t prevTime;
time_t displatTime_compare(){   
  uint32_t result = 0;
  result+= _Event_time.getHours()    * SECS_PER_HOUR;
  result+= _Event_time.getMinutes()  * SECS_PER_MIN;
  result+= _Event_time.getSeconds();
  return (time_t)result; 
}

void event_1(){
  Serial.printf_P(PSTR("event_1\n"));
  time_t triggerTimer;
  AL_timeHelper::incrementCurrentTime(triggerTimer, 0, 0, 0, 1, 0);
  _EventManager->set_time(0, triggerTimer);
}
uint8_t loopMod = 0;
void loop()
{
  _WCEVO_manager.handleConnection();  
  _AL_httpTime.handle();
  if (AL_timeHelper::sntpIsSynced()) {
    switch (loopMod) {
      case 0:
      {
        uint8_t diff = 60 - AL_timeHelper::get_minutes();
        time_t triggerTimer;
        AL_timeHelper::incrementCurrentTime(triggerTimer, 0, 0, 0, 1, 0);
        _EventManager->set_triggerFunc(0, &event_1);
        _EventManager->set_time(0, triggerTimer);
        _EventManager->set_activate(0, true);
        loopMod = 1;
      }    
      break;
      case 1:
        _EventManager->loop();
      break;
      default:
      break;
    }    
  }


  delay(0);
}
