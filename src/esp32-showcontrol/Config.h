#ifndef Config_h
#define Config_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include "config_json.h"
#include "Button.h"
#include "structs.h"

// Reference: Singleton design pattern here: https://stackoverflow.com/questions/1008019/c-singleton-design-pattern

class Config {
  public:
    // TODO: Move to structs.h
    struct OSCSubscription {
      Button* button;
      String string;
      String type;
    };

    struct OSCHost {
      String name;
      String host;
      uint16_t port;
    };

    struct network_config {
      String hostname;
      OSCHost master_host;
      std::vector<OSCHost> secondary_hosts;
      uint16_t osc_listen_port;
    };

    struct timer_config {
      unsigned long led_flash_ms;
      unsigned long switch_debounce_ms;
      unsigned long heartbeat_timeout_ms;
      uint64_t heartbeat_us;
      uint64_t poll_us;
    };
    
    std::vector<Button> buttons;
    std::vector<OSCSubscription> OSCsubscriptions;
    std::vector<SCOSCMessage> startOSCMessages;
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
