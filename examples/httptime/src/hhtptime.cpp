/*
  -DALTIMESYNC_ENABLED
  -DALTIME_USE_TIMELIB

*/

#define MYTZ PSTR("CET-1CEST,M3.5.0,M10.5.0/3")

#include <altoolslib.h>
#include <wificonnectevo.h>

AsyncWebServer  webserver(80);
DNSServer       dnsServer;
WCEVO_manager   _WCEVO_manager("ex_ds18b20", "alml1234", &dnsServer, &webserver);  
AL_httpTime     _AL_httpTime;
#ifdef ESP8266
  WiFiEventHandler e1, e2;  
#endif

#include <AL_sensors.h>
// typedef MyTemplate< int > MyInstantiatedTemplate;  
// template class SensorManagement< dht22Class >;
// class dht22Class;
Sensor<DS18B20Class>  _sensor;
// DS18B20Managment  _sensor;
uint8_t         _sensorId;


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
   // dht22Managment<String> * _sensor = new dht22Managment<String>(); 
  _sensor.create(D2, _sensorId);


}
time_t prevTime;
time_t displatTime_compare(){   
  uint32_t result = 0;
  result+= AL_timeHelper::get_hours()    * SECS_PER_HOUR;
  result+= AL_timeHelper::get_minutes()  * SECS_PER_MIN;
  result+= AL_timeHelper::get_seconds();
  return (time_t)result; 
}
void loop()
{
  _WCEVO_manager.handleConnection();  
  _AL_httpTime.handle();

  if (AL_timeHelper::sntpIsSynced() && prevTime != displatTime_compare( )) {
    char tmpStr[12];
    sprintf(tmpStr, "%02d:%02d:%02d", AL_timeHelper::get_hours(), AL_timeHelper::get_minutes(), AL_timeHelper::get_seconds());
    prevTime  = displatTime_compare( );
    Serial.println(tmpStr);

    DynamicJsonDocument doc(1024);
    JsonObject obj = doc.createNestedObject(F("dht22"));
    _sensor.get_json(_sensorId, obj);
    serializeJsonPretty(doc, Serial);Serial.println();


  }  

  delay(0);
}