#ifndef ALSTRING_H
	#define ALSTRING_H

	#include <Arduino.h>
	#include <Print.h>
	#include <ArduinoJson.h>
	#include <LinkedList.h>
	#include "altools.h"


/*
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
  _alstring.set_col_size(3,20);

  uint8_t cols = _alstring.get_col_nb();
  for(int i = 0; i < cols; i++) {
    uint8_t sizeMax = _alstring.get_col_size(i);
    String  sep;
    String  str_pad = String(i) + " ";
    uint8_t size    = str_pad.length();
    _alstring.get_col_seperator(sep);
    while (size < sizeMax) {str_pad += " "; size = str_pad.length();}    
    if (i==0) Serial.print(str_pad) ;
    else Serial.print(sep + str_pad) ; 
  }
  Serial.println();
  _alstring.print();
*/
class alstring_str
{
	char 		* _buffer = nullptr;
	uint8_t _col = 0;
	int 		_end = 0;
public:
	alstring_str(uint8_t col, int end, const char * const & str) {
		_col = col;
		_end = end;
		al_tools::c_str(_buffer, str);
	}
	~alstring_str() {
		if (_buffer) {Serial.println("delete buffer");delete _buffer;}
	}
	uint8_t get_len() { return strlen(_buffer); }
	uint8_t get_col() { return _col; }
	int get_end() { return _end; }
	void get_str(String & r) { r = String(_buffer); }


};
class alstring
{
	LList<alstring_str *> _list;
	uint8_t _nb_col = 0;
	String 	_col_separator = " - ";
	uint8_t * _col_sizeArray = nullptr;
public:
	alstring(){};
	~alstring(){
		if (_col_sizeArray) delete[] _col_sizeArray;
  	while (_list.size()) {alstring_str * item = _list.shift(); delete item;}
  	_list.clear();		
	};
	uint8_t get_col_nb() {return _nb_col;}
	uint8_t get_col_size(uint8_t p) {return _col_sizeArray[p];}
	void get_col_seperator(String & r) {r = _col_separator;}
	void set_col_seperator(const String & r) {_col_separator = r;}
	void set_col_size(uint8_t p, uint8_t s) {_col_sizeArray[p] = s;}

	void add_str(uint8_t col, int end, const char * const & str) {
		// Serial.printf_P(PSTR("col: %d - end: %d - str: %s\n"), col, end, str);
		if ((end == -1) && (col > 0)) {
			for(uint8_t i = 0; i < col; i++) {
				_list.add( new alstring_str(i, 0, "") );
			}
		}	
		uint8_t lastItemPos = _list.size()-1;
		if (col > 0 && (_list.get(lastItemPos)->get_col() != col-1 ) ) {
			for(uint8_t i = _list.get(lastItemPos)->get_col()+1; i < col; i++) {
				_list.add( new alstring_str(i, 0, "") );
			}			
		}	
		_list.add( new alstring_str(col, end, str) );
	}

	void setup(){
		_nb_col = 0;

		for(int i = 0; i < _list.size(); ++i) {
			alstring_str * item = _list.get(i);
			if ( item->get_col() > _nb_col ) _nb_col = item->get_col() ;
		}

		if (_col_sizeArray) delete[] _col_sizeArray;
		_col_sizeArray = new uint8_t[_nb_col+1];

		_nb_col++;

		for(int i = 0; i < _nb_col; ++i) {_col_sizeArray[i]=0;}
		for(int i = 0; i < _list.size(); ++i) {
			alstring_str * item = _list.get(i);
			uint8_t p = item->get_col();
			if (item->get_len() > _col_sizeArray[p]) _col_sizeArray[p] = item->get_len()+1;
		}		
	}
	void print(){

		uint8_t _col_p;
		uint8_t _col_size;
		String 	_str_out = "";



		// for (int i = 0; i < _nb_col; i++) {
		// 		String 	str_pad = "";
		// 		uint8_t size 		= 0;

	 //      str_pad = String(i) + " ";
	 //      size 		= str_pad.length();
	 //      while (size < _col_sizeArray[i]) {str_pad += " "; size = str_pad.length();}  	
	 //      if (i==0) _str_out += str_pad;			
	 //      else _str_out += _col_separator + str_pad;			
		// }
		// Serial.println(_str_out);

		for(int i = 0; i < _list.size(); i++) {

			alstring_str * item = _list.get(i);

			String str_col;
			_str_out = "";

			if (item->get_col() == 0) {

				uint8_t pos 		= i;

				_col_p 		= item->get_col();
				_col_size = _col_sizeArray[_col_p];
				item->get_str(str_col);

				// Serial.printf_P(PSTR("pos: %d\n"), i);

				String 	str_pad = "";
				uint8_t size 		= 0;

	      str_pad = str_col;
	      size 		= str_pad.length();
	      while (size < _col_size) {str_pad += " "; size = str_pad.length();}  	
	      _str_out = str_pad;

				for (int j = pos+1; j < _list.size(); j++) {

					alstring_str * item_2 = _list.get(j);

					if (item_2->get_col() == 0) break;

					for(int k = 0; k < _nb_col; k++) {

						_col_p 		= item_2->get_col();
						_col_size = _col_sizeArray[_col_p];
						item_2->get_str(str_col);

						if (item_2->get_col() == k+1) {

				      str_pad = str_col;
				      size 		= str_pad.length();
				      while (size < _col_size) {str_pad += " "; size = str_pad.length();}  	
				      _str_out += _col_separator + str_pad;
							// Serial.printf_P(PSTR("\tcol: %d/%d:%d - %s\n"), k+1, _col_p, _col_size, str_col.c_str());
						}
					}
				}
				Serial.println(_str_out);
			}

		}

	}
};

#endif // _ALSTRING_H_