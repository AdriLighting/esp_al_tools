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

#include "../../include/meteo/weatherapiid.h"

alt_weatherApi::alt_weatherApi(){
  if (_wb_id)       delete _wb_id;
  if (_ow_id)       delete _ow_id;
  if (_wb_location) delete _wb_location;
  if (_ow_location) delete _ow_location;

  uint8_t len;


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