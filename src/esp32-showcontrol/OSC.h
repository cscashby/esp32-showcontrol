#ifndef OSC_h
#define OSC_h

#include <Arduino.h>
#include "Display.h"

class OSC {
  public:
    static void begin(Display* d);
    static void loop();
    static void IRAM_ATTR heartbeat();

    static std::vector<String> currentQueue;

    static void IRAM_ATTR queueOSCMessage(const String m);

    static void heartbeatReply();
  private:
    static Display* d;
  
    static unsigned long lastThumpMillis;
};

#endif // OSC_h
