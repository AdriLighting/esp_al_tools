#ifndef _ALHTTPTOOLS_H
#define _ALHTTPTOOLS_H
#include <Arduino.h>
namespace al_httptools {
    unsigned int get_httpdata(String & payload, const String &url);
}

#endif // _ALHTTPTOOLS_H