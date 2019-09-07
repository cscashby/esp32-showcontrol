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

    struct OSCHost {
      const String name;
      const String host;
      uint16_t port;
    };

    struct network_config {
      String hostname;
      std::vector<OSCHost> osc_hosts;
      uint16_t osc_listen_port;
    };

    struct timer_config {
      unsigned long led_flash_ms;
      unsigned long switch_debounce_ms;
      uint64_t heartbeat_us;
      uint64_t poll_us;
    };
    
    std::vector<Button> buttons;
    timer_config timers;
    network_config network;

    static Config& getConfig() {
      static Config instance;
      return instance;
    }
  private:
    Config();
    Config(Config const&);          // Not implemented
    void operator=(Config const&);  // Not implemented
};

#endif //Config_h
