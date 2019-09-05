#ifndef Button_h
#define Button_h

#include <Arduino.h>

class Button {
  public:
    Button() = delete;
    Button(const char* name, const uint8_t pin, const uint8_t LED_pin)
      : name(name), pin(pin), LED_pin(LED_pin)
      {}

    void loop();

    const char* name;
    const uint8_t pin;
    const uint8_t LED_pin;
    
    std::vector<String> OSC_commands;

  private:
    bool pressed;
    unsigned long debounceMillis;
};

#endif
