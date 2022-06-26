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
  

  /**
   * @brief      { function_description }
   *
   * @param[in]  in              root folder
   * @param[in]  SerializePrint  pretty print
   * @param[in]  display         print file content
   */                                
  al_tools::SPIFFS_PRINT("/", false, false); // display files and folder
  al_tools::SPIFFS_PRINT("/eff", false, false); // display files and folder

  al_tools::SPIFFS_readFile("/outputs/lamp.txt"); // simpl read file  


  // make json with folder and files from filesystem
  DynamicJsonDocument doc(10000);
  JsonObject root = doc.to<JsonObject>();
  al_tools::SPIFFS_printFiles("/eff", root);
  serializeJsonPretty(doc, Serial);Serial.println();

/*
  String returnText = "";
  returnText += "<table><tr><th align='left'>Folder</th><th align='left'>Name</th><th align='left'>Size</th><th></th><th></th></tr>";
  DynamicJsonDocument doc_fsWebpage(10000);
  JsonObject root_fsWebpage = doc_fsWebpage.to<JsonObject>();
  al_tools::SPIFFS_printFiles("/eff", root_fsWebpage);
  JsonArray arr = doc_fsWebpage[F("folders")].as<JsonArray>();
  for (size_t i = 0; i < arr.size(); i++) {
    String path = arr[i].as<String>();
    JsonArray oPath;
    if (path == "/")  oPath = doc_fsWebpage[path][F("items")].as<JsonArray>();
    else              oPath = doc_fsWebpage[F("/")][path][F("items")].as<JsonArray>();
    for (size_t j = 0; j < oPath.size(); j++) {
      String file = oPath[j][F("file")].as<String>();
      size_t size = oPath[j][F("size")].as<size_t>();
      returnText += "<tr align='left'><td>" + path+ "</td><td>" + file+ "</td><td>" + "humanReadableSize(size)" + "</td>";
      String fullpath = (path=="/")?path+file:"/"+path+"/"+file;
      returnText += "<td><button onclick=\"downloadDeleteButton(\'" + fullpath + "\', \'download\')\">Download</button>";
      returnText += "<td><button onclick=\"downloadDeleteButton(\'" + fullpath + "\', \'delete\')\">Delete</button></tr>";
    }
  }
  returnText += "</table>";
  serializeJsonPretty(root_fsWebpage,Serial);Serial.println();
*/

  // spiff delete recursive file folder from target
  // SPIFFS_deleteRecursive(fs::FS &fs, const String &path);


  // deserialise json from file
  doc.clear();
  if (AP_deserializeFile(doc, "/eff/STRIP_0/eff/0000.json")) {serializeJsonPretty(doc, Serial);Serial.println();}

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