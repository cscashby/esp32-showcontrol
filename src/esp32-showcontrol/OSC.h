#ifndef OSC_h
#define OSC_h

#include <Arduino.h>
#include "Display.h"
#include "structs.h"
#include <deque>

// I should probably be a library not a class.

class OSC {
  public:
    struct CueInfo {
      String cueID;
      String displayName;
    };

    static CueInfo runningCue;
    static CueInfo nextCue;
  
    static void begin(Display* d);
    static void loop();
    static void IRAM_ATTR heartbeat();

    static void updateDisplay();

    static std::deque<SCOSCMessage> currentQueue;

    static void IRAM_ATTR queueOSCMessage(SCOSCMessage msg);

    static void heartbeatReply();
  private:
    static Display* d;
  
    static unsigned long lastThumpMillis;

    static bool heartBeatOK;
    static bool sendHeartBeat;
};

#endif // OSC_h
