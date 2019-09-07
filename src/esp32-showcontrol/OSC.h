#ifndef OSC_h
#define OSC_h

#include <Arduino.h>

class OSC {
  public:
    static void begin();
    static void loop();

    static std::vector<String> currentQueue;

    static void queueOSCMessage(const String m);
  private:
};

#endif // OSC_h
