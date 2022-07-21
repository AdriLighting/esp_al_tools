/**The MIT License (MIT)
 
 Copyright (c) 2018 by ThingPulse Ltd., https://thingpulse.com
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#ifdef WEATHERBIT_ENABLED

#ifndef WEATHERBIT_H
  #define WEATHERBIT_H

  #include <JsonStreamingParser.h>
  #include <JsonListener.h>




  struct weatherbitList {
      const char *  key;
      const char *  desc;
      boolean enable;
      String data;
      // void    (* setValue ) (String msg);
  } ;


  class weatherbitCurrent: public JsonListener {
    private:
      String currentKey;
      String currentParent;

      void httpget_updateData(const String & url);
      void buildUrl(String & result, const String & appId, const String & locationId, const String & language) ;

    public:
      weatherbitCurrent();

      void httpget_updateData(const String & appId, const String & locationId, const String & language);
      void print();
      void getKey(String & result, const String & value);

      virtual void whitespace(char c);

      virtual void startDocument();

      virtual void key(String key);

      virtual void value(String value);

      virtual void endArray();

      virtual void endObject();

      virtual void endDocument();

      virtual void startArray();

      virtual void startObject();
  };


#endif
#endif
