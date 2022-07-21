#ifdef OPENWEATHMAP_ENABLED
#ifndef OPENWEATHERMAP_H
#define OPENWEATHERMAP_H

  #include <Arduino.h>
  #include <ArduinoJson.h>

  typedef struct OpenWeatherMap {

    float coord_lon;
    float coord_lat;

    uint8_t weather_0_id;
    String  weather_0_main;
    String  weather_0_description;
    String  weather_0_icon;

    String base;

    float     main_temp;
    float     main_feels_like;
    float     main_temp_min;
    float     main_temp_max;
    uint32_t  main_pressure;
    uint8_t   main_humidity;

    uint32_t visibility;

    float   wind_speed;
    uint8_t wind_deg;

    uint8_t clouds_all;

    long dt;

    uint8_t   sys_type;
    uint32_t  sys_id;
    String    sys_country;
    long      sys_sunrise;
    long      sys_sunset;

    uint32_t timezone;
    long id;
    String name;
    uint8_t cod;


  } OpenWeatherMap_t;

  static const char ALMOWM_KEY_LON [] PROGMEM = "lon";
  static const char ALMOWM_KEY_LAT [] PROGMEM = "lat";
  static const char ALMOWM_KEY_WEATHER_0_ID          [] PROGMEM = "id";
  static const char ALMOWM_KEY_WEATHER_0_MAIN        [] PROGMEM = "main";
  static const char ALMOWM_KEY_WEATHER_0_DESCRIPTION [] PROGMEM = "description";
  static const char ALMOWM_KEY_WEATHER_0_ICON        [] PROGMEM = "icon";
  static const char ALMOWM_KEY_BASE [] PROGMEM = "base";
  static const char ALMOWM_KEY_MAIN_TEMP        [] PROGMEM = "temp";
  static const char ALMOWM_KEY_MAIN_FEELS_LIKE  [] PROGMEM = "feels_like";
  static const char ALMOWM_KEY_MAIN_TEMP_MIN    [] PROGMEM = "temp_min";
  static const char ALMOWM_KEY_MAIN_TEMP_MAX    [] PROGMEM = "temp_max";
  static const char ALMOWM_KEY_MAIN_PRESSURE    [] PROGMEM = "pressure";
  static const char ALMOWM_KEY_MAIN_HUMIDITY    [] PROGMEM = "humidity";
  static const char ALMOWM_KEY_VISIBILITY [] PROGMEM = "visibility";
  static const char ALMOWM_KEY_WIND_SPEED [] PROGMEM = "wind_speed";
  static const char ALMOWM_KEY_WIND_DEG [] PROGMEM = "wind_deg";
  static const char ALMOWM_KEY_CLOUDS_ALL [] PROGMEM = "clouds_all";
  static const char ALMOWM_KEY_DT [] PROGMEM = "dt";
  static const char ALMOWM_KEY_SYS_TYPE    [] PROGMEM = "sys_type";
  static const char ALMOWM_KEY_SYS_ID      [] PROGMEM = "sys_id";
  static const char ALMOWM_KEY_SYS_COUNTRY [] PROGMEM = "sys_country";
  static const char ALMOWM_KEY_SYS_SUNRISE [] PROGMEM = "sys_sunrise";
  static const char ALMOWM_KEY_SYS_SUNSET  [] PROGMEM = "sys_sunset";
  static const char ALMOWM_KEY_TIMEZONE [] PROGMEM = "timezone";
  static const char ALMOWM_KEY_ID [] PROGMEM = "id";
  static const char ALMOWM_KEY_NAME [] PROGMEM = "name";
  static const char ALMOWM_KEY_COD [] PROGMEM = "cod";

  struct OpenWeatherMapData_t {
      const char  * _name;
      void  (* get_data ) (String & r, OpenWeatherMap_t * data);
  } ;

  namespace al_openweathermap {
    void getValueByKey(OpenWeatherMap_t * data, const char * search, String & r);
    void serialPrint(OpenWeatherMap_t * data);
    void getJson(OpenWeatherMap_t * data, boolean isMetric, const String & appId, const String & locationId, const String & language) ;
  }

  extern uint8_t OpenWeatherMapData_itemsCount;
  extern PROGMEM OpenWeatherMapData_t OpenWeatherMapData_items [];


#endif // OPENWEATHERMAP_H
#endif // OPENWEATHERMAP_H