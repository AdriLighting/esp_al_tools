#ifndef _ALHTTPTOOLS_H
#define _ALHTTPTOOLS_H
#include <Arduino.h>

namespace al_httptools {
  unsigned int get_httpdata(String &, const String &);
  unsigned int downloadFile(const String &, const String &);
}

#endif // _ALHTTPTOOLS_H