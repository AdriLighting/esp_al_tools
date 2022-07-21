#include <altoolslib.h>
#include <ArduinoJson.h>
#include <LinkedList.h>

/*
  TO ENABLED FILESYSTEM MANAGEMENT
    Set "FSOK" in your compiler’s preprocessor options  
    for SPIFFS
      Set "USE_SPIFFS" in your compiler’s preprocessor options  
    for LittleFS
      Set "USE_LITTLEFS" in your compiler’s preprocessor options 

  and u can use the instance FILESYSTEM
*/

// heap monitor and soft historic
HeapStatu _HeapStatu;

void setup()
{
  Serial.begin(115200);

  for(unsigned long const serialBeginTime = millis(); !Serial && (millis() - serialBeginTime > 5000); ) { }
  delay(1000);

  //
  // initialization of the basic memory capture to compare with the following
  HeapStatu::setupHeap_v1();
  //


  Serial.println();
  Serial.printf_P(PSTR("\n#############\nALT SYSTEM INFOS\n#############\n\n"));

  // setup filesystem
  #if defined(ESP8266)
    FILESYSTEM.begin();
  #elif defined(ESP32)
    FILESYSTEM.begin(true);
  #endif

  //
  // String split with delimeter
  String split = "s1,s2,s3";
  int splitSize;
  const char** list = al_tools::explode(split, ',', splitSize);
  Serial.printf("\n");
  for(int i = 0; i < splitSize; ++i) {Serial.printf("[%d] %s\n", i, list[i]);}
  for(int i = 0; i < splitSize; ++i) {delete list[i];}
  delete[] list;
  //
  
  
  //
  // String split with delimeter
  split = "&c1:v1&c2:v2";
  LList<SplitItem *> _SplitItem;
  splitText(split, "&",  ':', &_SplitItem);
  for(int j = 0; j < _SplitItem.size(); ++j) {Serial.printf("[%d] c: %s - v: %s\n", j, _SplitItem[j]->_cmd, _SplitItem[j]->_value);}
  while (_SplitItem.size()) {SplitItem *eff = _SplitItem.shift(); delete eff;}
  _SplitItem.clear();
  //


  //
  // Convertion const char * to String
  const char * cs = "const char *";
  String str = al_tools::ch_toString(cs); 
  //


  //
  uint32_t now = millis();
  String result = "";
  // convertion to day/hours/min/sec
  al_tools::on_time_d(now, result);
  Serial.printf_P(PSTR("time since board launch: %s\n"), result.c_str());
  // convertion to hours/min/sec
  al_tools::on_time_h(now, result);
  Serial.printf_P(PSTR("time since board launch: %s\n"), result.c_str());
  //
  
  File f ;
  DynamicJsonDocument doc(10000);
  JsonObject          root;
  JsonArray           arr;

  root = doc.to<JsonObject>();
  root = root.createNestedObject(F("test_1"));
  root[F("key_1")] = "key_1 v";
  root[F("key_2")] = "key_2 v";
  arr = root.createNestedArray(F("array"));
  arr.add("v1");
  arr.add("v2");
  arr.add("v3");

  FILESYSTEM.mkdir("/test");
  f = FILESYSTEM.open("/test/test_1.json", "w");
  serializeJson(doc, f);
  f.close();


  doc.clear();doc.garbageCollect();
  root = doc.to<JsonObject>();
  root = root.createNestedObject(F("test_2"));
  root[F("key_1")] = "key_1 v";
  root[F("key_2")] = "key_2 v";
  arr = root.createNestedArray(F("array"));
  arr.add("v1");
  arr.add("v2");
  arr.add("v3");  

  FILESYSTEM.mkdir("/test/test2");
  f = FILESYSTEM.open("/test/test2/test_2.json", "w");
  serializeJson(doc, f);
  f.close();

  /**
   * @brief      { function_description }
   *
   * @param[in]  in              root folder
   * @param[in]  SerializePrint  pretty print
   * @param[in]  display         print file content
   */                                
  al_tools::SPIFFS_PRINT("/", false, false); // display files and folder
  al_tools::SPIFFS_PRINT("/test", false, false, false); // display files and folder

  // al_tools::SPIFFS_readFile("/test/test_1.json"); // simpl read file  


  // make json with folder and files from filesystem
  doc.clear();doc.garbageCollect();
  root = doc.to<JsonObject>();
  al_tools::SPIFFS_printFiles("/test", root, false, false);
  serializeJsonPretty(doc, Serial);Serial.println();


  // deserialise json from file
  // doc.clear();
  // if (AP_deserializeFile(doc, "/test/test_1.json")) {serializeJsonPretty(doc, Serial);Serial.println();}

}


// heap monitor test

const char* const strHeap[] PROGMEM = {
  "Qui facere quaerat sit blanditiis tempore et necessitatibus voluptas eum minima voluptatem", 
  "Non dolores quasi ea animi itaque eum repudiandae ", 
  "temporibus ab inventore quidem in doloremque dolore aut quia quos", 
  "illo eum nobis deserunt est voluptate nulla in similique atque. Est rerum eligendi qui saepe rerum est perferendis mollitia ex dicta labore", 
  "Ea dolorem eaque aperiam repellat sit iusto praesentium aut officia dolorem aut voluptatem delectus aut sapiente saepe sed pariatur exercitationem",
  "rerum non totam corporis sed deleniti dolorem et velit impedit 33 iure voluptas ut voluptates asperiores est perspiciatis voluptatem. Sit delectus alias nam esse eius aut soluta reiciendis"
};  

LList<String> _listOverride;

uint32_t timerLast_2=0;

void loop()
{

  // if (ESP.getFreeHeap() < 10000) {
  //   Serial.println("del");
  //   while (_listOverride.size()) {_listOverride.shift();}
  // }
  // if (millis()-timerLast_2 > 1000) {
  //   for(int i = 0; i < 6; ++i) {_listOverride.add(FPSTR(strHeap[i]));}
  //   String heap_1;
  //   _HeapStatu.update();
  //   _HeapStatu.print(heap_1); 
  //   Serial.printf_P(PSTR("%s\n"), heap_1.c_str());
  //   timerLast_2 = millis();      
  // }

}