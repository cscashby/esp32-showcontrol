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
    SSD_13XX* tft;
    
  private:
    bool heartBeatStatus = false;
    
    void heartBeat();
    void brokenHeart();
};

#endif
