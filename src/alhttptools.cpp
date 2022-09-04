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


#include "../include/alhttptools.h"
#include "../include/altools.h"

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <HTTPClient.h>
#endif
#ifdef FILESYSTEM
  #if defined USE_SPIFFS
    #include <FS.h>
  #elif defined USE_LITTLEFS
    #include <LittleFS.h> 
  #endif
#endif

typedef void (*ProgressCallback)(const String &fileName, int16_t bytesDownloaded, int16_t bytesTotal);


namespace al_httptools {
  int get_httpdata(String & payload, const String &url) {

    WiFiClient client;
    HTTPClient http;

    ALT_TRACEC("main", "[get_httpdata]\n\turl: %s\n", url.c_str());  

    http.begin(client, url);

    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK){
      payload = http.getString(); 
    }
    http.end();

    ALT_TRACEC("main", "&c:1&s:\tHTTPCode: %d\n", httpCode);  

    return httpCode;
  }
  int post_httpdata(String & payload, const String &url, const String &data) {

    WiFiClient client;
    HTTPClient http;

    ALT_TRACEC("main", "[get_httpdata]\n\turl: %s\n", url.c_str());  

    http.begin(client, url);
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(data);
    if (httpCode == HTTP_CODE_OK){
      payload = http.getString(); 
    }
    http.end();
    
    ALT_TRACEC("main", "&c:1&s:\tHTTPCode: %d\n", httpCode);  

    return httpCode;
    
  }

int downloadFile(const String &url, const String &filename, ProgressCallback progressCallback) {
  #ifdef FILESYSTEM
    ALT_TRACEC("main", "[downloadFile][START]\n") ;
    ALT_TRACEC("main", "&c:1&s:\tDownloading %s and saving as %s\n", url.c_str(), filename.c_str()) ;  

    HTTPClient http;
    WiFiClient client;
    
    #ifdef ALT_DEBUG_TARCE
      Serial.print(F("\t[HTTP] begin...\n"));  
    #endif

    // configure server and url
    http.begin(client, url);

    ALT_TRACEC("main", "&c:1&s:\t[HTTP] GET...\n");  

    // start connection and send HTTP header
    int httpCode = http.GET();
    if(httpCode > 0) {
        //FILESYSTEM.remove(filename);
        File f = FILESYSTEM.open(filename, "w");
        if (!f) {

            ALT_TRACEC("main", "&c:1&s:\t[FAIL]\n");
            ALT_TRACEC("main", "&c:1&s:t\tfile open failed\n");  

            return -1;
        }

        // HTTP header has been send and Server response header has been handled
        ALT_TRACEC("main", "&c:1&s:\t[HTTP] GET... code: %d\n", httpCode);  

        // file found at server
        if(httpCode == HTTP_CODE_OK) {

            // get lenght of document (is -1 when Server sends no Content-Length header)
            int total = http.getSize();
            int len = total;
            // progressCallback(filename, 0,total);
            // create buffer for read
            uint8_t buff[128] = { 0 };

            // get tcp stream
            WiFiClient * stream = http.getStreamPtr();

            uint32_t watchDog_timer = millis();
            // read all data from server
            while(http.connected() && (len > 0 || len == -1)) {

                if (( millis() - watchDog_timer ) > 45000) {
                  LT_TRACEC("main", "&c:1&s:\t[HTTP] WATCHDOG OVER 45000MS -> LEAV WHILE\n", httpCode);  
                  break;}

                // get available data size
                size_t size = stream->available();

                if(size) {
                    // read up to 128 byte
                    int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

                    // write it to Serial
                    f.write(buff, c);

                    if(len > 0) {
                        len -= c;
                    }
                    // progressCallback(filename, total - len,total);
                }
                delay(1);
            }

            ALT_TRACEC("main", "&c:1&s:\t[HTTP] connection closed or file end.\n");  

        }
        f.close();
    } else {
      ALT_TRACEC("main", "&c:1&s:\t[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());  
    }
    
    http.end();
    return httpCode;  
  #else
    return -1;  
  #endif
}

int downloadFile(const String &url, const String &filename) {
  return downloadFile(url, filename, nullptr);
}

}