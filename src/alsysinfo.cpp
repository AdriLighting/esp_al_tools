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


#include "../include/altools.h"

#ifdef ALSI_ENABLED

  #include "../include/alsysinfo.h"

  #if defined(ESP8266)
    #define WIFI_getChipId()  ESP.getChipId() 
    #include <ESP8266WiFi.h>
  #elif defined(ESP32)
    #define WIFI_getChipId()  (uint32_t)ESP.getEfuseMac()
    #include <rom/rtc.h>
    #include <WiFi.h>
    #include <esp_wifi.h>
  #endif


/*
#include <cmath>
#include <cstring>
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include <cstdio>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstring>
#include <cstdarg>

#if defined(USE_ESP8266)
#include <osapi.h>
#include <user_interface.h>
// for xt_rsil()/xt_wsr_ps()
#include <Arduino.h>
#elif defined(USE_ESP32_FRAMEWORK_ARDUINO)
#include <Esp.h>
#elif defined(USE_ESP_IDF)
#include "esp_system.h"
#include <freertos/FreeRTOS.h>
#include <freertos/portmacro.h>
#endif

#ifdef USE_ESP32_IGNORE_EFUSE_MAC_CRC
#include "esp_efuse.h"
#include "esp_efuse_table.h"
#endif

#ifdef USE_ESP32
#include <esp_heap_caps.h>
#endif
*/






#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
const char S_y                  [] PROGMEM = "Yes";
const char S_n                  [] PROGMEM = "No";


const char* const ALSI_CATEGORY [] PROGMEM = { ALSI_HEAPSIZE, ALSI_FILESYSTEM, ALSI_FLASHCHIP, ALSI_SKETCH, ALSI_BOARD, ALSI_TIME, ALSI_NETWORK };
const char* const ALSI_CATEGORY_KEY [] PROGMEM = { "ALSI_HEAPSIZE", "ALSI_FILESYSTEM", "ALSI_FLASHCHIP", "ALSI_SKETCH", "ALSI_BOARD", "ALSI_TIME", "ALSI_NETWORK" };

namespace al_sysinfo {
  static uint32_t get_free_heap() {
    #if defined(ESP8266)
      return ESP.getFreeHeap();  // NOLINT(readability-static-accessed-through-instance)
    #elif defined(ESP32)
      return heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
    #endif
    return 0;
  } 
  static uint32_t get_MaxFreeBlockSize() {
    #if defined(ESP8266)
    // NOLINTNEXTLINE(readability-static-accessed-through-instance)
      return ESP.getMaxFreeBlockSize();
    #elif defined(ESP32)
      return heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL);
    #endif
    return 0; 
  }
  void getMaxFreeBlockSize(const char *& r){
    char buffer[1024];
    sprintf(buffer, "%d bytes", get_MaxFreeBlockSize());
    r = (const char *)buffer;
  }
  void freeheap(const char *& r){
    char buffer[1024];
    sprintf(buffer, "%d bytes", get_free_heap());
    r = (const char *)buffer;
  }
  #ifdef ESP8266
    void getHeapFragmentation(const char *& r){
      char buffer[1024];
      sprintf(buffer, "%d", ESP.getHeapFragmentation());
      r = (const char *)buffer;
    } 
  #endif
  #ifdef FILESYSTEM
    void fs_totalBytes(const char *& r) {
      #if defined(ESP8266)
        FSInfo fs_info;
        FILESYSTEM.info(fs_info);   
        float value = (float)fs_info.totalBytes / 1024.0; 
      #elif defined(ESP32)
        float value = (float)FILESYSTEM.totalBytes() / 1024.0;
      #endif
      char buffer[1024];
      sprintf(buffer, "%.2f KB", value);  
      r = (const char *)buffer;  
    }
    void fs_usedBytes(const char *& r) {
      #if defined(ESP8266)
        FSInfo fs_info;
        FILESYSTEM.info(fs_info);   
        float value = (float)fs_info.usedBytes / 1024.0; 
      #elif defined(ESP32)
        float value = (float)FILESYSTEM.usedBytes() / 1024.0;
      #endif
      char buffer[1024];
      sprintf(buffer, "%.2f KB", value);  
      r = (const char *)buffer;  
    } 
    void fs_freebytes(const char *& r) {
      #if defined(ESP8266)
        FSInfo fs_info;
        FILESYSTEM.info(fs_info);   
        float value = ((float)fs_info.totalBytes-(float)fs_info.usedBytes) / 1024.0; 
      #elif defined(ESP32)
        float value = ((float)FILESYSTEM.totalBytes()-(float)FILESYSTEM.usedBytes()) / 1024.0;
      #endif
      char buffer[1024];
      sprintf(buffer, "%.2f KB", value);  
      r = (const char *)buffer;  
    }
    void fs_blockSize(const char *& r) {
      char buffer[1024];
      #if defined(ESP8266)
        FSInfo fs_info;
        FILESYSTEM.info(fs_info);   
        sprintf(buffer, "%d", fs_info.blockSize);
      #elif defined(ESP32)
        sprintf(buffer, "UNKNOW");
      #endif
      r = (const char *)buffer;  
    } 
    void fs_maxOpenFiles(const char *& r) {
      char buffer[1024];
      #if defined(ESP8266)
        FSInfo fs_info;
        FILESYSTEM.info(fs_info);   
        sprintf(buffer, "%d", fs_info.maxOpenFiles);
      #elif defined(ESP32)
        sprintf(buffer, "UNKNOW");
      #endif
      r = (const char *)buffer;  
    } 
    void fs_maxPathLength(const char *& r) {
      char buffer[1024];
      #if defined(ESP8266)
        FSInfo fs_info;
        FILESYSTEM.info(fs_info);   
        sprintf(buffer, "%d", fs_info.maxPathLength);
      #elif defined(ESP32)
        sprintf(buffer, "UNKNOW");
      #endif
      r = (const char *)buffer;  
    }   
  #endif    
  void getFlashChipSize(const char *& r) {
    float value = (float)ESP.getFlashChipSize() / 1024.0 / 1024.0;
    char buffer[1024];
    sprintf(buffer, "%.2f MB", value);  
    r = (const char *)buffer;  
  }
  #ifdef ESP8266
    void getFlashChipRealSize(const char *& r) {
      float value = (float)ESP.getFlashChipRealSize() / 1024.0 / 1024.0;
      char buffer[1024];
      sprintf(buffer, "%.2f MB", value);  
      r = (const char *)buffer;  
    }   
  #endif
  #ifdef ESP32
    void getPsramSize(const char *& r) {
      float value = (float)ESP.getPsramSize() / 1024.0 / 1024.0;
      char buffer[1024];
      sprintf(buffer, "%.2f", value); 
      r = (const char *)buffer;  
    }   
  #endif  
  void getFreeSketchSpace(const char *& r) {
    float value = (float)ESP.getFreeSketchSpace() / 1024.0 / 1024.0;
    char buffer[1024];
    sprintf(buffer, "%.2f MB", value);  
    r = (const char *)buffer;  
  }
  void getSketchSize(const char *& r) {
    float value = (float)ESP.getSketchSize() / 1024.0 / 1024.0;
    char buffer[1024];
    sprintf(buffer, "%.2f MB", value);  
    r = (const char *)buffer;  
  }
  void getTotalSketchSize(const char *& r) {
    float value = ((float)ESP.getSketchSize() + (float)ESP.getFreeSketchSpace()) / 1024.0 / 1024.0;
    char buffer[1024];
    sprintf(buffer, "%.2f MB", value);  
    r = (const char *)buffer;  
  } 
  #ifdef ESP8266
    void getFlashChipId(const char *& r) {
      uint32_t value = (uint32_t)ESP.getFlashChipId();
      char buffer[1024];
      sprintf(buffer, "%08X", value); 
      r = (const char *)buffer;  
    } 
  #endif
  void getChipId(const char *& r) {
    char buffer[1024];
    sprintf(buffer, "%s", String(WIFI_getChipId(),HEX).c_str());  
    r = (const char *)buffer;  
  }
  void stamacAddress(const char *& r) {
    char buffer[1024];
    sprintf(buffer, "%s", WiFi.macAddress().c_str()); 
    r = (const char *)buffer;  
  }
  void softAPmacAddress(const char *& r) {
    char buffer[1024];
    sprintf(buffer, "%s", WiFi.softAPmacAddress().c_str()); 
    r = (const char *)buffer;  
  }
  void ALSI_system_get_sdk_version(const char *& r) {
    char buffer[1024];
    #if defined(ESP32)
    sprintf(buffer, "%s", esp_get_idf_version()); 
    #elif defined(ESP8266)
    sprintf(buffer, "%s", system_get_sdk_version());  
    #endif
    r = (const char *)buffer;  
  }
  #ifdef ESP8266
    void getCoreVersion(const char *& r) {
      char buffer[1024];
      sprintf(buffer, "%s", ESP.getCoreVersion().c_str());  
      r = (const char *)buffer;  
    } 
    void ALSI_system_get_boot_version(const char *& r) {
      char buffer[1024];
      sprintf(buffer, "%d", system_get_boot_version()); 
      r = (const char *)buffer;  
    }
  #endif
  void getCpuFreqMHz(const char *& r) {
    char buffer[1024];
    sprintf(buffer, "%d", ESP.getCpuFreqMHz()); 
    r = (const char *)buffer;  
  }


  #ifdef ESP8266
  /*
  ESP8266 boot details

  reset causes:
          0:
          1: normal boot
          2: reset pin
          3: software reset
          4: watchdog reset

      boot device:
          0:
          1: ram
          3: flash - See more at: http://www.esp8266.com/viewtopic.php?p=2096#p2112
  */

    struct boot_flags {
      unsigned char raw_rst_cause : 4;
      unsigned char raw_bootdevice : 4;
      unsigned char raw_bootmode : 4;

      unsigned char rst_normal_boot : 1;
      unsigned char rst_reset_pin : 1;
      unsigned char rst_watchdog : 1;

      unsigned char bootdevice_ram : 1;
      unsigned char bootdevice_flash : 1;
    };

    struct boot_flags bootmode_detect(void) {
      int reset_reason = 0, bootmode = 0;
      asm (
        "movi %0, 0x60000600\n\t"
        "movi %1, 0x60000200\n\t"
        "l32i %0, %0, 0x114\n\t"
        "l32i %1, %1, 0x118\n\t"
        : "+r" (reset_reason), "+r" (bootmode) /* Outputs */
        : /* Inputs (none) */
        : "memory" /* Clobbered */
      );

      struct boot_flags flags;
      flags.raw_rst_cause     = (reset_reason&0xF);
      flags.raw_bootdevice    = ((bootmode>>0x10)&0x7);
      flags.raw_bootmode      = ((bootmode>>0x1D)&0x7);
      flags.rst_normal_boot   = flags.raw_rst_cause == 0x1;
      flags.rst_reset_pin     = flags.raw_rst_cause == 0x2;
      flags.rst_watchdog      = flags.raw_rst_cause == 0x4;
      flags.bootdevice_ram    = flags.raw_bootdevice == 0x1;
      flags.bootdevice_flash  = flags.raw_bootdevice == 0x3;
      return flags;
    }

    String reset_cause(struct boot_flags flags) {
      String reset_cause=String(flags.raw_rst_cause);
      if (flags.rst_normal_boot)  reset_cause+="(normal boot)";
      if (flags.rst_reset_pin)    reset_cause+="(reset pin)";
      if (flags.rst_watchdog)     reset_cause+="(watchdog reset)";
      return reset_cause;
    }
    String reset_cause() {
      boot_flags f = bootmode_detect();
      return reset_cause(f) ;
    }
    String boot_device(boot_flags flags) {
      String boot_device=String(flags.raw_bootdevice);
      if (flags.bootdevice_ram)   boot_device+=" (ram)";
      if (flags.bootdevice_flash) boot_device+=" (flash)";
      return boot_device;
    }
    String boot_device() {
      boot_flags f = bootmode_detect();
      return boot_device(f) ;
    }
    void lasResetReason(const char *&r) {
      char buffer[1024];
      sprintf(buffer, "%s - %s", ESP.getResetReason().c_str(), reset_cause().c_str());  
      r = (const char *)buffer;     
    }
  #endif
  #ifdef ESP32
    void lasResetReason(const char *&r) {
      char buffer[1024];
        for(int i=0;i<2;i++){
          int reason = rtc_get_reset_reason(i);
          switch (reason)
          {
            //@todo move to array
            case 1  : sprintf_P(buffer, PSTR("%s"), "Vbat power on reset");break;
            case 3  : sprintf_P(buffer, PSTR("%s"), "Software reset digital core");break;
            case 4  : sprintf_P(buffer, PSTR("%s"), "Legacy watch dog reset digital core");break;
            case 5  : sprintf_P(buffer, PSTR("%s"), "Deep Sleep reset digital core");break;
            case 6  : sprintf_P(buffer, PSTR("%s"), "Reset by SLC module, reset digital core");break;
            case 7  : sprintf_P(buffer, PSTR("%s"), "Timer Group0 Watch dog reset digital core");break;
            case 8  : sprintf_P(buffer, PSTR("%s"), "Timer Group1 Watch dog reset digital core");break;
            case 9  : sprintf_P(buffer, PSTR("%s"), "RTC Watch dog Reset digital core");break;
            case 10 : sprintf_P(buffer, PSTR("%s"), "Instrusion tested to reset CPU");break;
            case 11 : sprintf_P(buffer, PSTR("%s"), "Time Group reset CPU");break;
            case 12 : sprintf_P(buffer, PSTR("%s"), "Software reset CPU");break;
            case 13 : sprintf_P(buffer, PSTR("%s"), "RTC Watch dog Reset CPU");break;
            case 14 : sprintf_P(buffer, PSTR("%s"), "for APP CPU, reseted by PRO CPU");break;
            case 15 : sprintf_P(buffer, PSTR("%s"), "Reset when the vdd voltage is not stable");break;
            case 16 : sprintf_P(buffer, PSTR("%s"), "RTC Watch dog reset digital core and rtc module");break;
            default : sprintf_P(buffer, PSTR("%s"), "NO_MEAN");
          }
        }   
      r = (const char *)buffer;     
    }
  #endif

  void getFlashChipSpeed(const char *&r) {
    char buffer[1024];
    float flashFreq = (float)ESP.getFlashChipSpeed() / 1000.0 / 1000.0;
    sprintf(buffer, "%.2f MHz", flashFreq); 
    r = (const char *)buffer;     
  }

  void getFlashChipMode(const char *&r) {
    char buffer[1024];
    const char *flash_mode;
    switch (ESP.getFlashChipMode()) {  // NOLINT(readability-static-accessed-through-instance)
      case FM_QIO:
        flash_mode = "QIO";
        break;
      case FM_QOUT:
        flash_mode = "QOUT";
        break;
      case FM_DIO:
        flash_mode = "DIO";
        break;
      case FM_DOUT:
        flash_mode = "DOUT";
        break;
  #ifdef USE_ESP32
      case FM_FAST_READ:
        flash_mode = "FAST_READ";
        break;
      case FM_SLOW_READ:
        flash_mode = "SLOW_READ";
        break;
  #endif
      default:
        flash_mode = "UNKNOWN";
    }
    sprintf(buffer, "%s", flash_mode);  
    r = (const char *)buffer;     
  }

  #ifdef ESP8266
    void getBootMode(const char *&r) {
      char buffer[1024];
      sprintf(buffer, "%d", ESP.getBootMode()); 
      r = (const char *)buffer;     
    }
    void getResetInfo(const char *&r) {
      char buffer[1024];
      sprintf(buffer, "%s", ESP.getResetInfo().c_str());  
      r = (const char *)buffer;     
    }   
  #endif
  #ifdef ESP32
    void getModel(const char *&r) {
      esp_chip_info_t info;
      esp_chip_info(&info); 
      const char *model;
      switch (info.model) {
        case CHIP_ESP32:
          model = "ESP32";
          break;
        default:
          model = "UNKNOWN";
      }     
      char buffer[1024];
      sprintf(buffer, "%s",model);  
      r = (const char *)buffer;     
    }   
    void getfeatures(const char *&r) {
      esp_chip_info_t info;
      esp_chip_info(&info); 
      String features = "";
      if (info.features & CHIP_FEATURE_EMB_FLASH) {
        features += "EMB_FLASH,";
        info.features &= ~CHIP_FEATURE_EMB_FLASH;
      }
      if (info.features & CHIP_FEATURE_WIFI_BGN) {
        features += "WIFI_BGN,";
        info.features &= ~CHIP_FEATURE_WIFI_BGN;
      }
      if (info.features & CHIP_FEATURE_BLE) {
        features += "BLE,";
        info.features &= ~CHIP_FEATURE_BLE;
      }
      if (info.features & CHIP_FEATURE_BT) {
        features += "BT,";
        info.features &= ~CHIP_FEATURE_BT;
      }   
      char buffer[1024];
      sprintf(buffer, "%s",features.c_str()); 
      r = (const char *)buffer;     
    }   
    void getCores(const char *&r) {
      esp_chip_info_t info;
      esp_chip_info(&info); 
      char buffer[1024];
      if (info.features) sprintf(buffer, "%d", info.cores); 
      r = (const char *)buffer;     
    }   
    void getRevision(const char *&r) {
      esp_chip_info_t info;
      esp_chip_info(&info); 
      char buffer[1024];
      if (info.features) sprintf(buffer, "%d", info.revision);  
      r = (const char *)buffer;     
    }   
    void getEspIdfVersion(const char *&r) {
      esp_chip_info_t info;
      esp_chip_info(&info); 
      char buffer[1024];
      if (info.features) sprintf(buffer, "%s", esp_get_idf_version());  
      r = (const char *)buffer;     
    } 

    void get_mac_address_raw(uint8_t *mac) {
    #if defined(USE_ESP32)
    #if defined(USE_ESP32_IGNORE_EFUSE_MAC_CRC)
      // On some devices, the MAC address that is burnt into EFuse does not
      // match the CRC that goes along with it. For those devices, this
      // work-around reads and uses the MAC address as-is from EFuse,
      // without doing the CRC check.
      esp_efuse_read_field_blob(ESP_EFUSE_MAC_FACTORY, mac, 48);
    #else
      esp_efuse_mac_get_default(mac);
    #endif
    #elif defined(USE_ESP8266)
      wifi_get_macaddr(STATION_IF, mac);
    #endif
    } 
    String get_mac_address_pretty() {
      uint8_t mac[6];
      get_mac_address_raw(mac);
      char buffer[1021];
      sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
      return String(buffer);
    }   
    void getEfuseMac(const char *&r) {
      char buffer[1024];
      sprintf(buffer, "%s", get_mac_address_pretty().c_str());  
      r = (const char *)buffer;     
    }   
  #endif

  void getSTAip(const char *&r) {
    char buffer[1024];
    sprintf(buffer, "%s", WiFi.localIP().toString().c_str()); 
    r = (const char *)buffer;     
  }   
  void getSTAsubnet(const char *&r) {
    char buffer[1024];
    sprintf(buffer, "%s", WiFi.subnetMask().toString().c_str());  
    r = (const char *)buffer;     
  }   
  void getSTAgateway(const char *&r) {
    char buffer[1024];
    sprintf(buffer, "%s", WiFi.gatewayIP().toString().c_str()); 
    r = (const char *)buffer;     
  }   
  void getConeected(const char *&r) {
    char buffer[1024];
    String val =  WiFi.isConnected() ? FPSTR(S_y) : FPSTR(S_n);
    sprintf(buffer, "%s", val.c_str()); 
    r = (const char *)buffer;     
  }   
  void getAutoconnect(const char *&r) {
    char buffer[1024];
    String ret = WiFi.getAutoConnect() ? FPSTR(S_y) : FPSTR(S_n);
    sprintf(buffer, "%s", ret.c_str()); 
    r = (const char *)buffer;     
  }   
  void getDNSserver(const char *&r) {
    char buffer[1024];
    sprintf(buffer, "%s", WiFi.dnsIP().toString().c_str()); 
    r = (const char *)buffer;     
  }   
  void getHostname(const char *&r) {
    char buffer[1024];
    #if defined(ESP32)
    sprintf(buffer, "%s", WiFi.getHostname());  
    #elif defined(ESP8266)
    String ret = WiFi.hostname();
    sprintf(buffer, "%s", ret.c_str());
    #endif
    r = (const char *)buffer;     
  }   
  void getUptime(const char *&r) {
    char buffer[1024];
    String value;
    al_tools::on_time_d(value);
    sprintf(buffer, "%s", value.c_str());
    r = (const char *)buffer;     
  }
  void getBuildate(const char *&r) {
    char buffer[1024];
    String value = String(__DATE__) + " " + String(__TIME__);
    sprintf(buffer, "%s", value.c_str());
    r = (const char *)buffer;     
  }
} 


PROGMEM ALSI_LIST ALSI_items [] = {
  {ALSI_FREEHEAP,       &al_sysinfo::freeheap,                      ALSI_HEAPSIZE, "ALSI_FREEHEAP"},
  {ALSI_HEAPBLOCK,      &al_sysinfo::getMaxFreeBlockSize,           ALSI_HEAPSIZE, "ALSI_HEAPBLOCK"},
#ifdef ESP8266
  {ALSI_HEAPFRAG,       &al_sysinfo::getHeapFragmentation,          ALSI_HEAPSIZE, "ALSI_HEAPFRAG"}, 
#endif
  {ALSI_INITHEAP,       &al_sysinfo::freeheap,                      ALSI_HEAPSIZE, "ALSI_INITHEAP"},

#ifdef FILESYSTEM
  {ALSI_FSTOTALBYTES,   &al_sysinfo::fs_totalBytes,                 ALSI_FILESYSTEM, "ALSI_FSTOTALBYTES"},
  {ALSI_FSUSEDBYTES,    &al_sysinfo::fs_usedBytes,                  ALSI_FILESYSTEM, "ALSI_FSUSEDBYTES"},
  {ALSI_FSFREEBYTES,    &al_sysinfo::fs_freebytes,                  ALSI_FILESYSTEM, "ALSI_FSFREEBYTES"},
  {ALSI_FSBLOCKSIZE,    &al_sysinfo::fs_blockSize,                  ALSI_FILESYSTEM, "ALSI_FSBLOCKSIZE"},
  {ALSI_FSMAXOPENFILES, &al_sysinfo::fs_maxOpenFiles,               ALSI_FILESYSTEM, "ALSI_FSMAXOPENFILES"},
  {ALSI_FSMAXPATH,      &al_sysinfo::fs_maxPathLength,              ALSI_FILESYSTEM, "ALSI_FSMAXPATH"}, 
#endif

#ifdef ESP8266
  {ALSI_FLASHCHIPID,    &al_sysinfo::getFlashChipId,                ALSI_FLASHCHIP, "ALSI_FLASHCHIPID"},  
#endif
#ifdef ESP32
  {ALSI_PSRAMSIZE,      &al_sysinfo::getPsramSize,                  ALSI_FLASHCHIP, "ALSI_PSRAMSIZE"},  
#endif
  {ALSI_FLASHSIZE,      &al_sysinfo::getFlashChipSize,              ALSI_FLASHCHIP, "ALSI_FLASHSIZE"},
#ifdef ESP8266
  {ALSI_RFLASHSIZE,     &al_sysinfo::getFlashChipRealSize,          ALSI_FLASHCHIP, "ALSI_RFLASHSIZE"},  
#endif
  {ALSI_FLASHFREQUENCY, &al_sysinfo::getFlashChipSpeed,             ALSI_FLASHCHIP, "ALSI_FLASHFREQUENCY"},
  {ALSI_FLASHMOD,       &al_sysinfo::getFlashChipMode,              ALSI_FLASHCHIP, "ALSI_FLASHMOD"},

  {ALSI_SKETCHSIZETOTAL, &al_sysinfo::getTotalSketchSize,           ALSI_SKETCH, "ALSI_SKETCHSIZETOTAL"},
  {ALSI_SKETCHSIZE,     &al_sysinfo::getSketchSize,                 ALSI_SKETCH, "ALSI_SKETCHSIZE"},
  {ALSI_FREESKETCHSIZE, &al_sysinfo::getFreeSketchSpace,            ALSI_SKETCH, "ALSI_FREESKETCHSIZE"},

#ifdef ESP32
  {ALSI_MODEL,          &al_sysinfo::getModel,                      ALSI_BOARD, "ALSI_MODEL"},
  {ALSI_FEATURES,       &al_sysinfo::getfeatures,                   ALSI_BOARD, "ALSI_FEATURES"},
  {ALSI_CORES,          &al_sysinfo::getCores,                      ALSI_BOARD, "ALSI_CORES"},
  {ALSI_REVISION,       &al_sysinfo::getRevision,                   ALSI_BOARD, "ALSI_REVISION"},
  {ALSI_ESPIDFVERS,     &al_sysinfo::getEspIdfVersion,              ALSI_BOARD, "ALSI_ESPIDFVERS"},
  {ALSI_EFUSEMAC,       &al_sysinfo::getEfuseMac,                   ALSI_BOARD, "ALSI_EFUSEMAC"},
#endif
  {ALSI_CHIPID,         &al_sysinfo::getChipId,                     ALSI_BOARD, "ALSI_CHIPID"},
  {ALSI_SDKVERSION,     &al_sysinfo::ALSI_system_get_sdk_version,   ALSI_BOARD, "ALSI_SDKVERSION"},
#ifdef ESP8266
  {ALSI_COREVERSION,    &al_sysinfo::getCoreVersion,                ALSI_BOARD, "ALSI_COREVERSION"},
  {ALSI_BOOTVERSION,    &al_sysinfo::ALSI_system_get_boot_version,  ALSI_BOARD, "ALSI_BOOTVERSION"},
#endif
  {ALSI_CPUFREQUENCY,   &al_sysinfo::getCpuFreqMHz,                 ALSI_BOARD, "ALSI_CPUFREQUENCY"},

  {ALSI_STAMAC,         &al_sysinfo::stamacAddress,                 ALSI_BOARD, "ALSI_STAMAC"},
  {ALSI_APAMAC,         &al_sysinfo::softAPmacAddress,              ALSI_BOARD, "ALSI_APAMAC"},

  {ALSI_LASTRESET,      &al_sysinfo::lasResetReason,                ALSI_BOARD, "ALSI_LASTRESET"},


  {ALSI_STAIP,          &al_sysinfo::getSTAip,                      ALSI_NETWORK, "ALSI_STAIP"},
  {ALSI_STASUBNET,      &al_sysinfo::getSTAsubnet,                  ALSI_NETWORK, "ALSI_STASUBNET"},
  {ALSI_STAGATEWAY,     &al_sysinfo::getSTAgateway,                 ALSI_NETWORK, "ALSI_STAGATEWAY"},
  {ALSI_CONNECT,        &al_sysinfo::getConeected,                  ALSI_NETWORK, "ALSI_CONNECT"},
  {ALSI_AUTOCONNECT,    &al_sysinfo::getAutoconnect,                ALSI_NETWORK, "ALSI_AUTOCONNECT"},
  {ALSI_DNSSERVER,      &al_sysinfo::getDNSserver,                  ALSI_NETWORK, "ALSI_DNSSERVER"},
  {ALSI_HOSTNAME,       &al_sysinfo::getHostname,                   ALSI_NETWORK, "ALSI_HOSTNAME"},
  {ALSI_BUILDDATE,      &al_sysinfo::getBuildate,                   ALSI_TIME, "ALSI_BUILDDATE"},
  {ALSI_UPTIME,         &al_sysinfo::getUptime,                     ALSI_TIME, "ALSI_UPTIME"},
};
const uint8_t ALSI_ITEMSSIZE    =  ARRAY_SIZE(ALSI_items);
const uint8_t ALSI_CATEGORYSIZE =  ARRAY_SIZE(ALSI_CATEGORY);
const char ALSI_PTJSON_001 [] PROGMEM = "system_infos";

void ALSYSINFO_print() {
  Serial.printf_P(PSTR("[%-3s] %-25s %-32s %s\n\n"), "pos", "NAME", "value", "KEY"); 
  for(int j = 0; j < ALSI_CATEGORYSIZE; ++j) {
    Serial.printf_P(PSTR("[%-3d] %-25s %-32s %-22s << CATEGORY\n"), j, ALSI_CATEGORY[j], "-", ALSI_CATEGORY_KEY[j]); 
    for(int i = 0; i < ALSI_ITEMSSIZE; ++i) {
      if (ALSI_items[i].GRP != ALSI_CATEGORY[j]) continue;
      const char * r = "";
      ALSI_items[i].DATA(r);
      Serial.printf_P(PSTR("[%-3d] %-25s %-32s %s\n"), i, ALSI_items[i].NAME, r, ALSI_items[i].KEY); 
      delay(1);
    } 
    Serial.println(); 
  }   
} 
void ALSYSINFO_getterByCat(DynamicJsonDocument & doc, const char * key, uint8_t returnArray) {

  JsonArray array;
  if (!doc.containsKey(FPSTR(ALSI_PTJSON_001))) doc.createNestedObject(FPSTR(ALSI_PTJSON_001));
  if(isDigit(al_tools::ch_toString(key).charAt(0))) {
    for(int j = 0; j < ALSI_CATEGORYSIZE; ++j) {
      if (j != al_tools::ch_toString(key).toInt()) continue;
      for(int i = 0; i < ALSI_ITEMSSIZE; ++i) {
        if (ALSI_items[i].GRP != ALSI_CATEGORY[j]) continue;
        const char * r = "";
        ALSI_items[i].DATA(r);
        if (!returnArray) {
          if (!doc[FPSTR(ALSI_PTJSON_001)][FPSTR(ALSI_CATEGORY[j])][FPSTR(ALSI_items[i].NAME)]) { 
            doc[FPSTR(ALSI_PTJSON_001)][FPSTR(ALSI_CATEGORY[j])][FPSTR(ALSI_items[i].NAME)] = al_tools::ch_toString(r);          
          } 
        } else {
          array = doc[FPSTR(ALSI_PTJSON_001)][FPSTR(ALSI_CATEGORY[j])][F("items")];
          if (array.isNull()) {
            doc[FPSTR(ALSI_PTJSON_001)][FPSTR(ALSI_CATEGORY[j])].createNestedArray("items");
          }          
          if (returnArray == 1) {
            array.add(al_tools::ch_toString(r));
          }
          else if (returnArray == 2) {
            JsonArray array_2 = array.createNestedArray() ;
            array_2.add(FPSTR(ALSI_items[i].NAME));     
            array_2.add(al_tools::ch_toString(r));     
            array.add(array_2);   
          }
        }
      } 
    }
  }


  String sC_2 = al_tools::ch_toString(key);
  sC_2 = sC_2.toLowerCase();    

  for(int i = 0; i < ALSI_ITEMSSIZE; ++i) {
    String sC_1 = al_tools::ch_toString(ALSI_items[i].GRP)
    sC_1 = sC_1.toLowerCase();

    if (sC_1 != sC_2) continue;
    const char * r = "";
    ALSI_items[i].DATA(r);
    if (!returnArray) {
      if (!doc[FPSTR(ALSI_PTJSON_001)][FPSTR(key)][FPSTR(ALSI_items[i].NAME)]) { 
        doc[FPSTR(ALSI_PTJSON_001)][FPSTR(key)][FPSTR(ALSI_items[i].NAME)] = al_tools::ch_toString(r);          
      }       
    } else {
      array = doc[FPSTR(ALSI_PTJSON_001)][FPSTR(key)][F("items")];
      if (array.isNull()) {
        doc[FPSTR(ALSI_PTJSON_001)][FPSTR(key)].createNestedArray("items");
      }      
      if (returnArray == 1) {
        array.add(al_tools::ch_toString(r));     
      }
      else if (returnArray == 2) {
        JsonArray array_2 = array.createNestedArray() ;
        array_2.add(FPSTR(ALSI_items[i].NAME));     
        array_2.add(al_tools::ch_toString(r));     
        array.add(array_2);     
      }      

    }

  }     
}
boolean ALSYSINFO_getCatByKey(const char * key, uint8_t & ret) {
  String sC_1 = al_tools::ch_toString(ALSI_items[i].NAME);
  // sC_1 = sC_1.toLowerCase();
  String sC_2 = al_tools::ch_toString(key);
  // sC_2 = sC_2.toLowerCase();

  for(int i = 0; i < ALSI_ITEMSSIZE; ++i) {
    if ( sC_1 == sC_2) {
      for(int j = 0; j < ALSI_CATEGORYSIZE; ++j) {
        if (al_tools::ch_toString(ALSI_items[i].GRP) == al_tools::ch_toString(ALSI_CATEGORY[j])) {
          ret = j; 
          return true;        
        }
      }
    }
  }   
  return false;
}
boolean ALSYSINFO_getValByKey(const char * key, const char *& ret) {
  if(isDigit(al_tools::ch_toString(key).charAt(0))) {
    uint8_t p = al_tools::ch_toString(key).toInt();
    ALSI_items[p].DATA(ret);
    return true;
  }
  String sC_1 = al_tools::ch_toString(ALSI_items[i].NAME);
  sC_1 = sC_1.toLowerCase();
  String sC_2 = al_tools::ch_toString(key);
  sC_2 = sC_2.toLowerCase();     
  for(int i = 0; i < ALSI_ITEMSSIZE; ++i) {
    if (sC_1 == sC_2) {
      ALSI_items[i].DATA(ret);
      return true;
    }
  }  
  return false;
}
void ALSYSINFO_getterByKey(DynamicJsonDocument & doc, const char * key, bool keyStr) {
  if (!doc.containsKey(FPSTR(ALSI_PTJSON_001))) doc.createNestedObject(FPSTR(ALSI_PTJSON_001));
  String item;
  if(isDigit(al_tools::ch_toString(key).charAt(0))) {
        uint8_t p = al_tools::ch_toString(key).toInt();
        const char * r = "";
        ALSI_items[p].DATA(r);
        uint8_t cp ;
        if (!ALSYSINFO_getCatByKey(ALSI_items[p].NAME, cp)) return;
        if (keyStr) item = FPSTR(ALSI_items[p].NAME);
        else        item = String(p);
        if (!doc[FPSTR(ALSI_PTJSON_001)][FPSTR(ALSI_CATEGORY[cp])][item]) { 
          doc[FPSTR(ALSI_PTJSON_001)][FPSTR(ALSI_CATEGORY[cp])][item] = al_tools::ch_toString(r);          
        }     
  } else {

    String sC_2 = al_tools::ch_toString(key);
    sC_2 = sC_2.toLowerCase();    
    for(int i = 0; i < ALSI_ITEMSSIZE; ++i) {
      String sC_1 = al_tools::ch_toString(ALSI_items[i].NAME)
      sC_1 = sC_1.toLowerCase();      
      if (sC_1 != sC_2) continue;
      const char * r = "";
      ALSI_items[i].DATA(r);
      uint8_t cp ;
      if (!ALSYSINFO_getCatByKey(ALSI_items[i].NAME, cp)) continue;   
      if (keyStr) item = FPSTR(ALSI_items[i].NAME);
      else        item = String(i);       
      if (!doc[FPSTR(ALSI_PTJSON_001)][FPSTR(ALSI_CATEGORY[cp])][item]) { 
        doc[FPSTR(ALSI_PTJSON_001)][FPSTR(ALSI_CATEGORY[cp])][item] = al_tools::ch_toString(r);          
      } 
    }       
  }
}
void ALSYSINFO_keyboard_getter(const String & v1) {
  int rSize = 0;
  DynamicJsonDocument doc(3500);

  LList<SplitItem *> _SplitItem;
  splitText(v1, "&",  ':', &_SplitItem);

  for(int j = 0; j < _SplitItem.size(); ++j) {
    const char** split = al_tools::explode(_SplitItem[j]->_value, ',', rSize);
    if (split) {
      for(int i = 0; i < rSize; ++i) {
        Serial.printf_P(PSTR("[%d] %s\n"), i , split[i]);
        if (strcmp_P(_SplitItem[j]->_cmd, "alsi") == 0)   ALSYSINFO_getterByCat(doc, split[i]);                           
        if (strcmp_P(_SplitItem[j]->_cmd, "alsii") == 0)  ALSYSINFO_getterByKey(doc, split[i]);                           
      }
      for(int i = 0; i < rSize; ++i) {
        delete split[i];
      }
      delete[] split; 
    } else {
        if (strcmp_P(_SplitItem[j]->_cmd, "alsi") == 0)   ALSYSINFO_getterByCat(doc, _SplitItem[j]->_value);                           
        if (strcmp_P(_SplitItem[j]->_cmd, "alsii") == 0)  ALSYSINFO_getterByKey(doc, _SplitItem[j]->_value);       
    }
  }
  while (_SplitItem.size()) {
    SplitItem *eff = _SplitItem.shift();
    delete eff;
  }
  _SplitItem.clear();

  serializeJsonPretty(doc,Serial);Serial.println(); 
   
}
void ALSYSINFO_keyboard_print() {
  Serial.printf_P(PSTR("@&alsi:0,network=\n"));
  Serial.printf_P(PSTR("@&alsii:0,network=\n"));
  Serial.printf_P(PSTR("alsi\n"));
  for(int i = 0; i < ALSI_CATEGORYSIZE; ++i) {
     Serial.printf_P(PSTR("[%-3d] %s\n"), i, ALSI_CATEGORY[i]);
  }   
}
/*
  _Sr_menu.add("sysinfo", "t", []() { ALSYSINFO_print(); });
  _Sr_menu.add("api", "r", [this]() { ALSYSINFO_keyboard_print(); });
  _Sr_menu.add("api getter", "@", [this](const String & v1, const String & v2) {  
    Serial.printf("CMD: %s - VAL: %s\n", v1.c_str(), v2.c_str());
    ALSYSINFO_keyboard_getter(v1);    
  }, SR_MM::SRMM_KEYVAL);   
*/

#endif