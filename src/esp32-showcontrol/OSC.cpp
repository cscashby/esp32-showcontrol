#include "OSC.h"
#include "config.h"

#include <ArduinoOSC.h>

// This should be in OSC.h but needs ArduinoOSC.h which is broken so can't be included in the .h file
static OscWiFi osc;

std::vector<String> OSC::currentQueue;

void IRAM_ATTR OSC::queueOSCMessage(const String m) {
  // We have to queue commands as OSCClient.h is not safe in an ISR context
  currentQueue.push_back(m);
}

void OSC::begin() {
  osc.begin(Config::getConfig().network.osc_listen_port);
}

void OSC::loop() {
  osc.parse();
  for( Config::OSCHost h : Config::getConfig().network.osc_hosts ) {
    for( const String m : currentQueue ) {
      Serial.print("Sending: "); Serial.print(m); Serial.print(" to "); Serial.print(h.host); Serial.print(":"); Serial.println(h.port);
      OscMessage o(h.host, h.port, m);
      osc.send(o);
    }
  }
  currentQueue.clear();
}
