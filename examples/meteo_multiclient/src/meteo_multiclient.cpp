#include <altoolslib.h>
#include <wificonnectevo.h>

AsyncWebServer  webserver(80);
DNSServer       dnsServer;
WCEVO_manager   _WCEVO_manager("httptime", "alml1234", &dnsServer, &webserver);  

void setup() {
  Serial.begin(115200);

  for(unsigned long const serialBeginTime = millis(); !Serial && (millis() - serialBeginTime > 5000); ) { }
  delay(1000);

  Serial.println();
  Serial.printf_P(PSTR("\n#############\nALT DEBUG\n#############\n\n"));


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
}

uint8_t modLoop = 0;
void loop() {
  _WCEVO_manager.handleConnection();  

  if (WCEVO_CONNECTED && modLoop == 0) {

    String searchValue = "";

#ifdef WEATHERBIT_ENABLED
    weatherbitCurrent weatherbitCurrentClient;
    weatherbitCurrentClient.httpget_updateData(FPSTR(WEATHERBIT_APPID), FPSTR(WEATHER_LOCATIONID), FPSTR(WEATHER_LANGUAGE));
    weatherbitCurrentClient.print(); 
    Serial.println();
    weatherbitCurrentClient.getKey(searchValue, FPSTR(ALMWB_KEY_TEMP));
    Serial.println(searchValue);

    delay(2000);  
#endif

#ifdef WEATHERBITFORECAST_ENABLED
    weatherbitForecast _weatherbitForecast;
    _weatherbitForecast.httpget_updateData(FPSTR(WEATHERBIT_APPID), FPSTR(WEATHER_LOCATIONID), FPSTR(WEATHER_LANGUAGE));
    _weatherbitForecast.print(); 
    Serial.println();
    _weatherbitForecast.getKey(searchValue, 0, FPSTR(ALMWB_KEY_TEMP));
    Serial.println(searchValue);  

    delay(2000);
#endif

#ifdef OPENWEATHMAP_ENABLED
    OpenWeatherMap_t currentWeather;
    al_openweathermap::getJson(&currentWeather, true, FPSTR(OPENWEATHERMAP_APPID), FPSTR(WEATHER_LOCATIONID), FPSTR(WEATHER_LANGUAGE));
    al_openweathermap::serialPrint(&currentWeather);
    Serial.println();
    al_openweathermap::getValueByKey(&currentWeather, ALMOWM_KEY_SYS_SUNSET, searchValue);
    Serial.println(searchValue);  
    struct tm t;
    tm *tm=&t;    
    time_t tnow = searchValue.toInt();
    localtime_r(&tnow, tm);
   
        Serial.printf("tm_mday %d - tm_hour %d - tm_min %d - tm_sec %d\n", tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec) ;   
#endif

    modLoop = 1;

  }
}


/*
String getMeteoIcon(const String & icon) {
  // clear sky
  // 01d
  if (icon == "01d")  {
    return "B";
  }
  // 01n
  if (icon == "01n")  {
    return "C";
  }
  // few clouds
  // 02d
  if (icon == "02d")  {
    return "H";
  }
  // 02n
  if (icon == "02n")  {
    return "4";
  }
  // scattered clouds
  // 03d
  if (icon == "03d")  {
    return "N";
  }
  // 03n
  if (icon == "03n")  {
    return "5";
  }
  // broken clouds
  // 04d
  if (icon == "04d")  {
    return "Y";
  }
  // 04n
  if (icon == "04n")  {
    return "%";
  }
  // shower rain
  // 09d
  if (icon == "09d")  {
    return "R";
  }
  // 09n
  if (icon == "09n")  {
    return "8";
  }
  // rain
  // 10d
  if (icon == "10d")  {
    return "Q";
  }
  // 10n
  if (icon == "10n")  {
    return "7";
  }
  // thunderstorm
  // 11d
  if (icon == "11d")  {
    return "P";
  }
  // 11n
  if (icon == "11n")  {
    return "6";
  }
  // snow
  // 13d
  if (icon == "13d")  {
    return "W";
  }
  // 13n
  if (icon == "13n")  {
    return "#";
  }
  // mist
  // 50d
  if (icon == "50d")  {
    return "M";
  }
  // 50n
  if (icon == "50n")  {
    return "M";
  }
  // Nothing matched: N/A
  return ")";

}

// http://api.openweathermap.org/data/2.5/weather?id=2972214&appid=603ef62761eb23b76ac8c8af7711776f&units=metric&lang=fr
// http://api.openweathermap.org/data/2.5/weather?2972214&appid=603ef62761eb23b76ac8c8af7711776f&units=metric&lang=fr

*/