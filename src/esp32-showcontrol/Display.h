#ifndef Display_h
#define Display_h

#include <SPI.h>
#include <SSD_13XX.h> //https://github.com/sumotoy/SSD_13XX
#include "_fonts/mono_mid.c"
#include "DisplayConfig.h"

class Display {
  public:
    Display();
    void begin();
    void loop();
    void heartbeat();
    void brokenHeart();
    void clearTextArea();
    static String truncateString(String in, long maxChars);
    SSD_13XX* tft;
    
  private:
    bool heartbeatStatus = false;
};

#endif
