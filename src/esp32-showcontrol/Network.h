#ifndef Network_h
#define Network_h

#include <WiFi.h>
#include <WebServer.h>
#include <AutoConnect.h>
#include "wwwroot.h"

class Network {
  public:
    static void begin();
    static void loop();
    
    static void rootPage();
    static void handle404();
    
  private:
};

#endif
