#ifndef Button_h
#define Button_h

#include <Arduino.h>
#include <map>

#include "OSC.h"
#include "const.h"

class Button;

class Button {
  public:
    Button() = delete;
    Button(const String n, const uint8_t p, const uint8_t l);

    void begin();
    void loop();
    void poll();

    void IRAM_ATTR isr();

    const String buttonName;
    const uint8_t pin;
    const uint8_t LED_pin;
    bool invert;
    
    std::vector<SCOSCMessage> OSC_messages;

  private:
    bool pressed;
    unsigned long debounceMillis;
    unsigned long ledLastChangeMillis;
};

#endif
