#ifndef _ALSYSINFO_H
	#define _ALSYSINFO_H
	#include <Arduino.h>
	#if defined(ESP8266)
		// extern "C" {
		//   #include "user_interface.h"
		// }
	#include <ESP8266WiFi.h>
	#elif defined(ESP32)
		#include <WiFi.h>
		#include <esp_wifi.h>
	#endif
	#include <ArduinoJson.h>

  typedef enum : uint8_t {
    ALSI_M_FREEHEAP = 0
  } alsi_mod_t;

// static const char ALSI_KEY_001 [] PROGMEM = "FREEHEAP";
// static const char ALSI_KEY_001 [] PROGMEM = "INITHEAP";
// static const char ALSI_KEY_001 [] PROGMEM = "FSTOTALBYTES";
// static const char ALSI_KEY_001 [] PROGMEM = "FSUSEDBYTES";
// static const char ALSI_KEY_001 [] PROGMEM = "FSFREEBYTES";
// static const char ALSI_KEY_001 [] PROGMEM = "FLASHSIZE";
// static const char ALSI_KEY_001 [] PROGMEM = "RFLASHSIZE";
// static const char ALSI_KEY_001 [] PROGMEM = "PSRAMSIZE";
// static const char ALSI_KEY_001 [] PROGMEM = "FREESKETCHSIZE";
// static const char ALSI_KEY_001 [] PROGMEM = "SKETCHSIZE";
// static const char ALSI_KEY_001 [] PROGMEM = "FLASHCHIPID";
// static const char ALSI_KEY_001 [] PROGMEM = "CHIPID";
// static const char ALSI_KEY_001 [] PROGMEM = "STAMAC";
// static const char ALSI_KEY_001 [] PROGMEM = "APAMAC";
// static const char ALSI_KEY_001 [] PROGMEM = "SDKVERSION";
// static const char ALSI_KEY_001 [] PROGMEM = "COREVERSION";
// static const char ALSI_KEY_001 [] PROGMEM = "BOOTVERSION";
// 
// static const char ALSI_KEY_001 [] PROGMEM = "FREEHEAP";
// static const char ALSI_KEY_002 [] PROGMEM = "INITHEAP";
// static const char ALSI_KEY_003 [] PROGMEM = "FSTOTALBYTES";
// static const char ALSI_KEY_004 [] PROGMEM = "FSUSEDBYTES";
// static const char ALSI_KEY_005 [] PROGMEM = "FSFREEBYTES";
// static const char ALSI_KEY_006 [] PROGMEM = "FLASHSIZE";
// static const char ALSI_KEY_007 [] PROGMEM = "RFLASHSIZE";
// static const char ALSI_KEY_008 [] PROGMEM = "PSRAMSIZE";
// static const char ALSI_KEY_009 [] PROGMEM = "FREESKETCHSIZE";
// static const char ALSI_KEY_010 [] PROGMEM = "SKETCHSIZE";
// static const char ALSI_KEY_011 [] PROGMEM = "FLASHCHIPID";
// static const char ALSI_KEY_012 [] PROGMEM = "CHIPID";
// static const char ALSI_KEY_013 [] PROGMEM = "STAMAC";
// static const char ALSI_KEY_014 [] PROGMEM = "APAMAC";
// static const char ALSI_KEY_015 [] PROGMEM = "SDKVERSION";
// static const char ALSI_KEY_016 [] PROGMEM = "COREVERSION";
// static const char ALSI_KEY_017 [] PROGMEM = "BOOTVERSION";

		struct ALSI_LIST {
			const char 	* NAME;
			void				(* DATA	) (const char *&);
			const char 	* GRP;

		} ;
		extern PROGMEM ALSI_LIST ALSI_items []; 
		extern const char* const ALSI_CATEGORY [] PROGMEM; 
		extern const uint8_t ALSI_ITEMSSIZE;
		extern const uint8_t ALSI_CATEGORYSIZE;

		void ALSYSINFO_print();
		void ALSYSINFO_getterByCat(DynamicJsonDocument & doc, const char * key);
		void ALSYSINFO_getterByKey(DynamicJsonDocument & doc, const char * key, bool keyStr = true);
		void ALSYSINFO_keyboard_getter(const String & v1); 
		void ALSYSINFO_keyboard_print(); 		
#endif // _ALSYSINFO_H