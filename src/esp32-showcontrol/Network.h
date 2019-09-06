#ifndef Network_h
#define Network_h

#include <ETH.h>
#include <WiFi.h>
#include <WebServer.h>
#include <AutoConnect.h>
#include "wwwroot.h"
#include "config.h"
#include "Display.h"

class Network {
  public:
    static void WiFiEvent(WiFiEvent_t event);

    static void begin(Display d);
    static void loop();
    
    static void rootPage();
    static void handle404();
    
  private:
    static bool eth_connected;
    static bool wifi_connected;
};

#endif
