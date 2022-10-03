#include <altoolslib.h>
#include <ArduinoJson.h>
#include <LinkedList.h>

/*

*/



void setup()
{
  Serial.begin(115200);

  for(unsigned long const serialBeginTime = millis(); !Serial && (millis() - serialBeginTime > 5000); ) { }
  delay(1000);



  Serial.println();
  Serial.printf_P(PSTR("\n#############\nALT SYSTEM INFOS\n#############\n\n"));



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

Serial.println(ESP.getFreeHeap());
{
  alstring _alstring;
  double value = 22.22;
  _alstring.add_str(0, 0,   al_tools::c_str(String(value)));
  _alstring.add_str(0, 0,   "line 1 col 0");
  _alstring.add_str(1, 0,   "line 1 col 1");
  _alstring.add_str(0, 0,   "line 2 col 0");
  _alstring.add_str(1, 0,   "l2 c1");
  _alstring.add_str(2, 0,   "l2 c2");
  _alstring.add_str(4, 0,   al_tools::c_str(String("1234567891")));
  _alstring.add_str(1, -1,  "yxxxxxxxxxxxxxxxxxy");
  _alstring.add_str(2, -1,  "yxxxxxxxxxxxxxxxxxy");
  _alstring.add_str(0, 0,   "dsdqd");
  _alstring.add_str(1, 0,   "");
  _alstring.add_str(2, 0,   "ssqds");  
  _alstring.add_str(3, 0,   "3");  
  _alstring.add_str(4, 0,   "4");  
  _alstring.add_str(5, 0,   "5");  
  _alstring.add_str(3, -1,  "3");
  _alstring.setup();
  _alstring.set_col_size(3,10);

  // uint8_t cols = _alstring.get_col_nb();
  // for(int i = 0; i < cols; i++) {
  //   uint8_t sizeMax = _alstring.get_col_size(i);
  //   String  sep;
  //   String  str_pad = String(i) + " ";
  //   uint8_t size    = str_pad.length();
  //   _alstring.get_col_seperator(sep);
  //   while (size < sizeMax) {str_pad += " "; size = str_pad.length();}    
  //   if (i==0) Serial.print(str_pad) ;
  //   else Serial.print(sep + str_pad) ; 
  // }
  // Serial.println();
  _alstring.print();

}
Serial.println(ESP.getFreeHeap());
}




void loop()
{


}