#ifndef _ALHTTPTOOLS_H
#define _ALHTTPTOOLS_H
#include <Arduino.h>

namespace al_httptools {
  int get_httpdata(String &, const String &);
  int post_httpdata(String & payload, const String &url, const String &data);
  int downloadFile(const String &, const String &);
}

#endif // _ALHTTPTOOLS_H