#ifndef Config_h
#define Config_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include "config_json.h"
#include "Button.h"

// Reference: Singleton design pattern here: https://stackoverflow.com/questions/1008019/c-singleton-design-pattern

class Config {
  public:
  
    struct OSC_subscription {
      Button* button;
      String string;
    };
    
//    struct Button {
//      String name;
//      uint8_t LED_pin;
//      uint8_t Button_pin;
//      std::vector<String> OSC_commands;
//      OSC_subscription OSC_subscribe;
//    };

    std::vector<Button> buttons;

    static Config& getConfig() {
      static Config instance;
      return instance;
    }
  private:
    Config();
    Config(Config const&);          // Not implemented
    void operator=(Config const&);  // Not implemented
};

#endif
