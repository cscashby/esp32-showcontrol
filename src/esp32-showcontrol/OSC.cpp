#include "OSC.h"
#include "config.h"

#include <ArduinoOSC.h>

// This should be in OSC.h but needs ArduinoOSC.h which is broken so can't be included in the .h file
static OscWiFi osc;

std::vector<String> OSC::currentQueue;
Display* OSC::d;
unsigned long OSC::lastThumpMillis;

void IRAM_ATTR OSC::heartbeat() {
  // Heartbeat needs to be scheduled in loop()
  currentQueue.push_back("/thump");
  // And check for missing heartbeats
  unsigned long now = millis();
  if( now - OSC::lastThumpMillis > Config::getConfig().timers.heartbeat_timeout_ms ) {
    Serial.print("Heartbeat missed by (ms): "); Serial.println(now - OSC::lastThumpMillis);
    OSC::d->brokenHeart();
  }
}

void IRAM_ATTR OSC::queueOSCMessage(const String m) {
  // We have to queue commands as OSCClient.h is not safe in an ISR context
  currentQueue.push_back(m);
}

void OSC::begin(Display* d) {
  OSC::d = d;
  osc.begin(Config::getConfig().network.osc_listen_port);
  // Hard coded replies
  osc.subscribe("/reply/workspace/*/thump", [](OscMessage& m){
    // We could collapse this by redefining heartbeatReply() with OscMessage param, but can't due to ArduinoOSC.h include issue
    OSC::heartbeatReply();  
  });
}

void OSC::heartbeatReply() {
  unsigned long now = millis();
  // Only display heartbeat if we're not outside our timeout
  if( now - OSC::lastThumpMillis < Config::getConfig().timers.heartbeat_timeout_ms ) {
    OSC::d->heartbeat();
  }
  OSC::lastThumpMillis = now;
}

void OSC::loop() {
  osc.parse();
  for( Config::OSCHost h : Config::getConfig().network.osc_hosts ) {
    for( const String m : currentQueue ) {
      //Serial.print("Sending: "); Serial.print(m); Serial.print(" to "); Serial.print(h.host); Serial.print(":"); Serial.println(h.port);
      OscMessage o(h.host, h.port, m);
      osc.send(o);
    }
  }
  currentQueue.clear();
}
