#ifndef _ALHTTPTIMESYNC_H
	#define _ALHTTPTIMESYNC_H
	#include <Arduino.h>
	#ifdef ALTIME_USE_TIMELIB
		#include <TimeLib.h>	
	#endif
	#if defined(ESP8266)
		#include <ESP8266WiFi.h>
		#include <ESP8266HTTPClient.h>
	#elif defined(ESP32)
	  #include <WiFi.h>
	  #include <HTTPClient.h>
	#endif

	#define ALTIME_TIMEAPI_BUFSIZE  600

	static const char ALTIME_PT_timeapi_tz_url[] PROGMEM  = "http://worldtimeapi.org/api/timezone/";
	static const char ALTIME_PT_timeapi_ip_url[] PROGMEM  = "http://worldtimeapi.org/api/ip";


	typedef struct EzTask{
		bool 			activate = false;
		uint32_t 	time = 0;
	} EzTask_t;

	void showTime();


	typedef struct _RTC_DATA{
	  uint64_t timeAcc;
	  unsigned long storage;
	  uint32_t timeBase;
	  _RTC_DATA() {timeAcc = 0; storage = 0; timeBase = 0;}
	} RTC_DATA;

	class AL_httpTime {
		String tzone;
    String ntp2;              // хранилище для ntp-сервера2 (резервный, задается с UI)
    uint8_t ntpcnt = 2;

		void set_time(String str, uint32_t lastUpdate);

		// unsigned int getHttpData(String &payload, const String &url);

		static time_t getUnixTime() {return time(nullptr); }
		void httprefreshtimer(const uint32_t delay, uint32_t&r);
		void tzsetup(const char* tz);

		EzTask ntpResync_task;
		void ntpResync_run();
		void ntpResync_start();
	public:
		AL_httpTime();
		~AL_httpTime(){};

		// bool sntpIsSynced();

		void get_timeHTTP();

		// boolean get_timeHTTP(const String &, String&);
		// boolean getUtc(const String & search, String & result);

#ifdef ESP8266
		void onSTAGotIP(WiFiEventStationModeGotIP ipInfo);
		void onSTADisconnected(WiFiEventStationModeDisconnected event_info);	
#endif
		void handle();

	};

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


	class AL_getTime {
	  template<class X>
	  using callback_t = X(*)();    

	  callback_t<int>     cb_getDay           = nullptr;
	  callback_t<int>     cb_getHours         = nullptr;
	  callback_t<int>     cb_getMinutes       = nullptr;
	  callback_t<int>     cb_getSeconds       = nullptr;
	  callback_t<bool>    cb_isTimeSet        = nullptr;
	  callback_t<String>  cb_getFormattedTime = nullptr;
	public:
	  AL_getTime()          ;
	  ~AL_getTime()         {};

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
	  
	};	
#endif // _ALNETTIME_H