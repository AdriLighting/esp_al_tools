#include "alhttptools.h"

#if defined(ESP8266)
	#include <ESP8266WiFi.h>
	#include <ESP8266HTTPClient.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <HTTPClient.h>
#endif

namespace al_httptools {
	unsigned int get_httpdata(String & payload, const String &url) {
	  WiFiClient client;
	  HTTPClient http;
	  Serial.printf_P( PSTR("[getHttpData] [url: %s]\n"), url.c_str());
	  http.begin(client, url);

	  int httpCode = http.GET();
	  if (httpCode == HTTP_CODE_OK){
	    payload = http.getString(); 
	  } else {
	    Serial.printf_P( PSTR("[ERR] [getHttpData] [HTTPCode: %d]\n"), httpCode);
	  }
	  http.end();
	  return payload.length();
	}
}