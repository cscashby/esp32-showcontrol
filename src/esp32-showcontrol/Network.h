#ifndef Network_h
#define Network_h

#include <ETH.h>
#include <WiFi.h>
#include <WebServer.h>
#include "wwwroot.h"
#include "wwwmainjs.h"
#include "Display.h"

class Network {
  public:
    static void WiFiEvent(WiFiEvent_t event);

    static void begin(Display* d);
    static void loop();
    
    static void rootPage();
    static void mainJS();
    static void handle404();
    static void handle_setMaster();
    static void handle_get();
    
  private:
    static bool eth_connected;
    static bool wifi_connected;
};

#endif
