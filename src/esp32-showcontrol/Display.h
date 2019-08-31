#ifndef Display_h
#define Display_h

#include <SPI.h>
#include <SSD_13XX.h> //https://github.com/sumotoy/SSD_13XX
#include "_fonts/mono_mid.c"

class Display {
  public:
    Display();
    void heartBeat();
    void brokenHeart();
  private:
    SSD_13XX* tft;
    bool heartBeatStatus = false;
};

#endif
