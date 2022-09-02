#include "../../include/meteo/weatherapiid.h"




alt_weatherApi::alt_weatherApi(){
  if (_wb_id)       delete _wb_id;
  if (_ow_id)       delete _ow_id;
  if (_wb_location) delete _wb_location;
  if (_ow_location) delete _ow_location;

  uint8_t len;
  len = strlen(WEATHERBIT_APPID);
  _wb_id = new char[len+1];
  strcpy(_wb_id, WEATHERBIT_APPID);
  len = strlen(OPENWEATHERMAP_APPID);
  _ow_id = new char[len+1];
  strcpy(_ow_id, OPENWEATHERMAP_APPID);

  len = strlen(WEATHER_LOCATIONID);
  _wb_location = new char[len+1];
  strcpy(_wb_location, WEATHER_LOCATIONID);
  len = strlen(WEATHER_LOCATIONID);
  _ow_location = new char[len+1];
  strcpy(_ow_location, WEATHER_LOCATIONID);

  len = strlen(WEATHER_LANGUAGE);
  _lang = new char[len+1];
  strcpy(_lang, WEATHER_LANGUAGE);  
}

alt_weatherApi _alt_weatherApi;