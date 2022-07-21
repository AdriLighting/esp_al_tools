# esp_al_tools


def	def_varia	initialisation	
FSOK		FILESYSTEM.begin();	altoolslib.h
USE_LITTLEFS	USE_SPIFFS		
FILESYSTEM=LittleFS	FILESYSTEM=SPIFFS		
			
ALT_DEBUG_TARCE		ALT_debugBuffer = new char[1024]; 	altoolslib.h
		_DebugPrintList.add("main");	
			
DEBUG_KEYBOARD			altoolslib.h
		_Sr_menu.add("", "y", []() { ; });	
		 _Sr_menu.add("", "@", [](const String & v1, const String & v2) { ; }, SR_MM::SRMM_KEYVAL); 	
			
ALSI_ENABLED			altoolslib.h
			
ALTIMESYNC_ENABLED			altoolslib.h
			
ALWORLDCLOCK_ENABLED			altoolslib.h
			
WEATHERBIT_ENABLED			altoolslib.h
			
WEATHERBITFORECAST_ENABLED			altoolslib.h
			
OPENWEATHMAP_ENABLED			altoolslib.h
![image](https://user-images.githubusercontent.com/43286266/180238286-367e2623-d4b0-4ef1-8c3b-979067ea37c6.png)



altoolslib.h
	#ifdef DEBUG_KEYBOARD
		#include "../include/alserialmenu.h"	
	#endif
	#include "../include/altools.h"
	#ifdef ALSI_ENABLED
		#include "../include/alsysinfo.h"
	#endif
	#ifdef ALTIMESYNC_ENABLED
		#include "../include/alhttptimesync.h"	
	#endif
	#ifdef ALWORLDCLOCK_ENABLED
		#include "../include/alworldclock.h"
	#endif

tools
	-D ALT_DEBUG_TARCE
	-D DEBUG_KEYBOARD

	-D ALSI_ENABLED

	-D ALTIMESYNC_ENABLED
	-D ALTIME_USE_TIMELIB

	-D ALWORLDCLOCK_ENABLED

all lib
-D USE_LITTLEFS - USE_SPIFFS	
-D FILESYSTEM=LittleFS 
-D FILESYSTEM=LITTLEFS
-D FILESYSTEM=SPIFFS

arduino patternlist
  FSOK
	-DDEBUG_PROGRAM
	-DDEBUG_PLAYLIST	
	DEBUG_APAPI
	DEBUG_BASICLIST





#include "../include/altools.h"
	extern DebugPrintList _DebugPrintList;
	namespace al_tools {
		/*
		int rSize = 0;
		AP_explode(func, '(', rSize, nullptr) ;
		String split[rSize+1];
		AP_explode(func, '(', rSize, split) ;
		*/
		void explode(const String & s, char sep, int & rSize, String * list);  
		/*
		https://stackoverflow.com/questions/16982015/getting-const-char-array-from-function
		String split = "s1,s2,s3";
		int splitSize;
		const char** list = AP_explode(split, ',', splitSize);
		Serial.printf("\n");
		for(int i = 0; i < splitSize; ++i) {Serial.printf("[%d] %s\n", i, list[i]);}
		for(int i = 0; i < rSize; ++i) {
		delete list[i];
		}
		delete[] list;
		*/  
		const char** explode(const String & s, char sep, int & rSize);

		#ifdef FILESYSTEM 
		void SPIFFS_printFiles(const String & path, JsonObject & obj);
		void SPIFFS_PRINT(boolean sPrint = false);
		#endif

		String ch_toString(const char * c);

		void on_time_h(uint32_t, String & result);
		void on_time_h(String & result);
		void on_time_d(String & result);  

	bool AP_deserializeFile(DynamicJsonDocument& doc, const char* filepath);  

	#ifdef ALT_DEBUG_TARCE
	  Serial.printf_P(PSTR(";&region_id=&setter_id:value\n"));
	  Serial.printf_P(PSTR(";&region_id=&range:start,end,value\n"));
	  Serial.printf_P(PSTR(";&range:start,end=&setter:value\n"));
	  Serial.printf_P(PSTR(";&range:start,end=&range:start,end,value\n"));	
		char * ALT_debugBuffer
			new char[1024];
		#define ALT_TRACE(parm_a, ...) 
		#define ALT_TRACEC(ptr, parm_a, ...)
		#define ALT_TRACEM(ptr, mod, parm_a, ...) 

	 class HeapStatu {
		static void setupHeap_v1() {pInitHeap = ESP.getFreeHeap();};
			HeapStatu::setupHeap_v1();
		void setupHeap_v2() {mod = false; initHeap = ESP.getFreeHeap();};
		void update();
		void print(String & ret);
		  String heap, time;
		  al_tools::on_time_h(time);_HeapStatu_2.setupHeap_v2();_HeapStatu_2.update();_HeapStatu_2.print(heap);
		  Serial.printf_P(PSTR("[HEAP MONITOR]\n\t%-15s%s\n##########################\n"), time.c_str(), heap.c_str());

	/*  
	  LList<SplitItem *>  _SplitItem;
	  splitText("&c1:v1&c2:v2", "&",  ':', &_SplitItem);
	  for(int i = 0; i < _SplitItem.size(); ++i) {
	    SplitItem * ptr = _SplitItem.get(i);
	    Serial.printf("[%d][c: %s][v: %s]", i, ptr->_cmd, ptr->_value);
	  }
	  while (_SplitItem.size()) {
	    SplitItem *ptr = _SplitItem.shift();
	    delete ptr;
	  }
	  _SplitItem.clear();     
	*/
	void splitText(const String & inputString, const char* const & arg,  char sep, LList<SplitItem * > * ptr);

#ifdef ALSI_ENABLED
	#include "../include/alsysinfo.h"
#endif
	struct ALSI_LIST {
		const char 	* NAME;
		void		(* DATA	) (const char *&);
		const char 	* GRP;

	} ;
	extern PROGMEM ALSI_LIST ALSI_items []; 
	extern const char* const ALSI_CATEGORY [] PROGMEM; 
	extern const uint8_t ALSI_ITEMSSIZE;
	extern const uint8_t ALSI_CATEGORYSIZE;

	get all keys: pos/name/value
	void ALSYSINFO_print();

	get category
	void ALSYSINFO_keyboard_print(); 
	ALSI_CATEGORY
		[0  ] Heap
		[1  ] Filesystem
		[2  ] Flash chip
		[3  ] Sketch
		[4  ] Core
		[5  ] Time
		[6  ] Network
	void ALSYSINFO_getterByCat(DynamicJsonDocument & doc, const char * key); {"system_infos" : {"cat" : {"key":"value"}}}
	void ALSYSINFO_getterByKey(DynamicJsonDocument & doc, const char * key); {"system_infos" : {"cat" : {"key":"value"}}}

	@&ALSI:0,Network=
	@&ALSII:Free Heap,4=
	void ALSYSINFO_keyboard_getter(const String & v1); 

  _Sr_menu.add("sysinfo", "t", []() { ALSYSINFO_print(); });
  _Sr_menu.add("api", "r", [this]() { ALSYSINFO_keyboard_print(); });
  _Sr_menu.add("api getter", "@", [this](const String & v1, const String & v2) {  
    Serial.printf("CMD: %s - VAL: %s\n", v1.c_str(), v2.c_str());
    ALSYSINFO_keyboard_getter(v1);    
  }, SR_MM::SRMM_KEYVAL);   


#ifdef DEBUG_KEYBOARD
	#include "../include/alserialmenu.h"	
#endif

[0][key: a][mod: 0][name: menu]
[1][key: z][mod: 0][name: ESPreset]
[2][key: e][mod: 0][name: freeHeap]
[4][key: t][mod: 0][name: sysinfo]
[5][key: u][mod: 0][name: debugregion]
[7][key: r][mod: 0][name: wcevo api]
[9][key: o][mod: 0][name: setter list] 	arduinopattern
[10][key: p][mod: 0][name: getter list] arduinopattern

[3][key: -][mod: 1][name: heapmonitor]
[6][key: ;][mod: 1][name: debugset]
[8][key: @][mod: 1][name: wcevo api getter]

[11][key: $][mod: 1][name: api getter] arduinopattern
[12][key: #][mod: 1][name: api setter] arduinopattern


  extern Sr_menu _Sr_menu;
	Sr_menu::Sr_menu(){
	    _timer_h.set_delay(3000000);
	    _timer_i1.set_enabled(false);
	    _Sr_menu.add("menu",          "a", []() { _Sr_menu.print(); });
	    _Sr_menu.add("ESPreset",      "z", []() { ESP.restart();    });
	    _Sr_menu.add("freeHeap",      "e", []() { String time; al_tools::on_time_d(time);Serial.printf_P(PSTR("time: %s\n"), time.c_str()); Serial.printf_P(PSTR("freeHeap: %d\n"), ESP.getFreeHeap()); });
	    _Sr_menu.add("heapmonitor",   "-", [&](const String & v1, const String & v2) {
	      _timer_h.set_delay(v1.toInt() * 1000);
	      _timer_i1.set_enabled(v2.toInt());
	    }, SR_MM::SRMM_KEYVAL);
	    #if defined(ALSI_ENABLED) && defined(DEBUG_KEYBOARD)
	    _Sr_menu.add("sysinfo", "t", []() { ALSYSINFO_print(); });  
	    #endif    
	    #ifdef ALT_DEBUG_TARCE
	    _Sr_menu.add("debugregion", "u", []() { _DebugPrintList.ketboardPrint(); });    
	    _Sr_menu.add("debugset",    ";", [](const String & v1, const String & v2) { 
	      _DebugPrintList.keyboardSet(v1,v2); }, SR_MM::SRMM_KEYVAL);    
	    #endif
	}  

	void loop()
	{
	  #ifdef DEBUG_KEYBOARD
	    _Sr_menu.serialRead();  
	  #endif	


#ifdef ALTIMESYNC_ENABLED
	#include "../include/alhttptimesync.h"	
#endif	 
	class AL_timeHelper {
	public:
		AL_timeHelper(){};
		~AL_timeHelper(){};
		static bool sntpIsSynced();
		static int get_hours();
		static int get_minutes();
		static int get_seconds();
		static int get_mday();		
		static void getDateTimeShortString(String &buf, const time_t _tstamp = 0);
		static void getDateTimeString(String &buf, const time_t _tstamp = 0);
		static void incrementCurrentTime(time_t & result, time_t timeStamp, int day, int hr, int min, int sec);
		static void incrementCurrentTime(struct tm * tm, time_t timeStamp, int day, int hr, int min, int sec);
		static const time_t* now();		
	};
    time_t tstampBadUTC;
    AL_timeHelper::incrementCurrentTime(tstampBadUTC, 0, 0, 0, 10, 0);
    String buf = "";
    AL_timeHelper::getDateTimeString(buf, 0);
    Serial.printf("current: %s\n", buf.c_str());
    buf = "";
    AL_timeHelper::getDateTimeString(buf, tstampBadUTC);
    Serial.printf("current+10min: %s\n", buf.c_str());	


	class AL_getTime {
	  template<class X>
	  void set_cb_time(callback_t<X> f, uint8_t pos) {
	    switch (pos) {
	      case 0: cb_getDay           = std::move(f); break;
	      case 1: cb_getHours         = std::move(f); break;
	      case 2: cb_getMinutes       = std::move(f); break;
	      case 3: cb_getSeconds       = std::move(f); break;
	      default:break;
	    }
	  }
	  template<class X>
	  void set_cb_isTimeSet(callback_t<X> f) {cb_isTimeSet = std::move(f);}
	  template<class X>
	  void set_cb_getFormattedTime(callback_t<X> f) {cb_getFormattedTime = std::move(f);}

	  int     getDay()            {if (cb_getDay)     return cb_getDay();     else return -1;};
	  int     getHours()          {if (cb_getHours)   return cb_getHours();   else return -1;};
	  int     getMinutes()        {if (cb_getMinutes) return cb_getMinutes(); else return -1;};
	  int     getSeconds()        {if (cb_getSeconds) return cb_getSeconds(); else return -1;};
	  bool    isTimeSet()         {if (cb_isTimeSet)  return cb_isTimeSet();  else return false;};
	  String  getFormattedTime()  {if (cb_getFormattedTime)  return cb_getFormattedTime(); else return "";};    
