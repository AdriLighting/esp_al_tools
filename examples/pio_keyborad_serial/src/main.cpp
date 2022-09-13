#include <altoolslib.h>

// Set "DEBUG_KEYBOARD" in your compiler’s preprocessor options

// predifine static class Sr_menu _Sr_menu
/* initialization from constructor
    _Sr_menu.add("menu",          "a", []() { _Sr_menu.print(); });
    _Sr_menu.add("ESPreset",      "z", []() { ESP.restart();    });
    _Sr_menu.add("freeHeap",      "e", []() { String time; al_tools::on_time_d(time);Serial.printf_P(PSTR("time: %s\n"), time.c_str()); Serial.printf_P(PSTR("freeHeap: %d\n"), ESP.getFreeHeap()); });
    _Sr_menu.add("heapmonitor",   "-", [&](const String & v1, const String & v2) {
      _timer_h.set_delay(v1.toInt() * 1000);
      _timer_i1.set_enabled(v2.toInt());
    }, SR_MM::SRMM_KEYVAL);
*/

void setup()
{
  Serial.begin(115200);

  for(unsigned long const serialBeginTime = millis(); !Serial && (millis() - serialBeginTime > 5000); ) { }
  delay(1000);

  Serial.println();
  Serial.printf_P(PSTR("\n#############\nALT KEYBOARD\n#############\n\n"));

  _Sr_menu.print();
    
  /*
    mod 0
      callback type void(*callback)()
        call from a header character.
      example :
  */
//                                  header character  callback (lambda ou typedef std::function)
  _Sr_menu.add("simple callback",   "y",              []() { Serial.println(F("touche y")); });
  /*
    mod 1
      callback type void(*callback)(const String&, const String&) 
        call from a header character and a separation character.
        separation character by default is "="
      example :
  */
//                                header character  callback (lambda ou typedef std::function)
  _Sr_menu.add("callback param",  "!",              [](const String & v1, const String & v2) {  
    Serial.printf_P(PSTR("CMD: %s - VAL: %s\n"), v1.c_str(), v2.c_str());
  }, SR_MM::SRMM_KEYVAL);

  _Sr_menu.print();

/* OUTPUT

  [SENT] y
  touche y

  [SENT] !tata=toto
  CMD: tata - VAL: toto

*/     
}

void loop()
{
  _Sr_menu.serialRead();
}