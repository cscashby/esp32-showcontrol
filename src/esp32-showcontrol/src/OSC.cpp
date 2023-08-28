#include "OSC.h"
#include "config.h"

#include <ArduinoOSCEther.h>

std::deque<SCOSCMessage> OSC::currentQueue;
Display* OSC::d;
unsigned long OSC::lastThumpMillis;
OSC::CueInfo OSC::runningCue;
OSC::CueInfo OSC::nextCue;
bool OSC::heartBeatOK = false;
bool OSC::sendHeartBeat = false;

void IRAM_ATTR OSC::heartbeat() {
  // Heartbeat needs to be scheduled in loop()
  OSC::sendHeartBeat = true;
  // And check for missing heartbeats
  unsigned long now = millis();
  if( now - OSC::lastThumpMillis > Config::getConfig().timers.heartbeat_timeout_ms ) {
    Serial.print("Heartbeat missed by (ms): "); Serial.println(now - OSC::lastThumpMillis);
    OSC::d->brokenHeart();
    OSC::heartBeatOK = false;
  }
}

void IRAM_ATTR OSC::queueOSCMessage(SCOSCMessage msg) {
  // We have to queue commands as OSCClient.h is not safe in an ISR context
  currentQueue.push_back(msg);
}

void OSC::begin(Display* d) {
  OSC::d = d;
  // Hard coded replies
  OscEther.subscribe(Config::getConfig().network.osc_listen_port, "/reply/workspace/*/thump", [](OscMessage& m) {
    // We could collapse this by redefining heartbeatReply() with OscMessage param, but can't due to ArduinoOSC.h include issue
    OSC::heartbeatReply();  
  });
  OscEther.subscribe(Config::getConfig().network.osc_listen_port, "/update/workspace/*/cueList/*/playbackPosition", [](OscMessage& m) {
    // We copy the address as we don't want to break the string when we tokenise it
    String addr = m.address();
    String json = m.arg<String>(0);

    char sizeString[] = "/update/workspace/FBD9B081-1C68-4C9C-8B74-98712F4DD90B/cueList/0FD30761-96F0-4C1E-AD1A-155DEB772604/playbackPosition";
    // TODO Make this a function!
    char a[sizeof(sizeString)];
    addr.toCharArray(a, sizeof(sizeString));
    
    Serial.print("Address: ");
    Serial.println(a);
    
    char* p = strtok(a, "/");
    char* split[6];
    int i = 0;
    while(p != NULL) {
      split[i++] = p;
      p = strtok(NULL, "/");
    }
    Serial.print("Workspace: ");
    Serial.println(split[2]);

    String cueID = m.arg<String>(0);
    Serial.print("Cue: ");
    Serial.println(cueID);

    // For some reason we get a space from the library so let's just check for >1 length not >0
    if( cueID.length() == 36 ) {
      String mstr = "/cue_id/" + cueID + "/displayName";
      SCOSCMessage msg;
      msg.command = mstr;
      currentQueue.push_back(msg);
      OSC::nextCue.cueID = cueID;
    } else {
      OSC::nextCue.cueID = "";
      OSC::nextCue.displayName = "---";
      OSC::updateDisplay();
    }
  });
  OscEther.subscribe(Config::getConfig().network.osc_listen_port, "/reply/cue_id/*/displayName", [](OscMessage& m) {
    // We copy the address as we don't want to break the string when we tokenise it
    String addr = m.address();
    char sizeString[] = "/reply/cue_id/0FD30761-96F0-4C1E-AD1A-155DEB772604/displayName";
    // TODO Make this a function!
    char a[sizeof(sizeString)];
    addr.toCharArray(a, sizeof(sizeString));
    
    Serial.print("Address: ");
    Serial.println(a);
    
    char* p = strtok(a, "/");
    char* split[6];
    int i = 0;
    while(p != NULL) {
      split[i++] = p;
      p = strtok(NULL, "/");
    }
    String cueID = split[2];
    Serial.print("Cue: ");
    Serial.println(cueID);

    String json = m.arg<String>(0);
    // Deserialize the JSON document
    StaticJsonDocument<5000> doc;
    DeserializationError error = deserializeJson(doc, json);
    const char* displayName;
    // Test if parsing succeeds.
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      displayName = "error";
    } else {
      displayName = doc["data"]; // "2"
    }
    Serial.print("Cue name found: ");
    Serial.println(displayName);
    // If we don't have the cue ID yet, then we probably have requested the current cue, which is by definition the next one
    if( OSC::nextCue.cueID.equals("selected")) {
      OSC::nextCue.cueID = cueID;
    }
    if( OSC::nextCue.cueID.equals(cueID) ) {
      OSC::nextCue.displayName = displayName;
    }
    OSC::updateDisplay();
  });
  OscEther.subscribe(Config::getConfig().network.osc_listen_port, "/reply/workspace/*/runningOrPausedCues", [](OscMessage& m) {
    // We copy the address as we don't want to break the string when we tokenise it
    String addr = m.address();
    char sizeString[] = "/workspace/FBD9B081-1C68-4C9C-8B74-98712F4DD90B/runningOrPausedCues";
    
    String json = m.arg<String>(0);
    // Deserialize the JSON document
    StaticJsonDocument<5000> doc;
    DeserializationError error = deserializeJson(doc, json);
    const char* displayName;
    const char* cueID;
    // Test if parsing succeeds.
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      displayName = "error";
      cueID = "";
    } else {
      displayName = doc["data"][0]["name"];
      cueID = doc["data"][0]["uniqueID"];
    }
    OSC::runningCue.cueID = cueID;
    OSC::runningCue.displayName = (displayName == NULL || strlen(displayName) == 0) ? "---" : displayName;
    OSC::updateDisplay();
  });
  // Set the cueID to selected so that we can get the selected cue's name as part of startup
  OSC::nextCue.cueID = "selected";
  // Send startup (non-thump) messages to QLab, waiting for a heartbeat to do so
  for( SCOSCMessage oscm : Config::getConfig().startOSCMessages ) {
      oscm.wait_for_heartbeat = true;
      currentQueue.push_back(oscm);
  }
}

void OSC::heartbeatReply() {
  unsigned long now = millis();
  // Only display heartbeat if we're not outside our timeout
  if( now - OSC::lastThumpMillis < Config::getConfig().timers.heartbeat_timeout_ms ) {
    OSC::d->heartbeat();
  }
  OSC::lastThumpMillis = now;
  OSC::heartBeatOK = true;
}

void OSC::loop() {
  // Process my queue of outbound messages, but only if we have a good heartbeat, and send one if we need to
  for( SCOSCMessage msg : currentQueue ) {
    // I do remove some entries, but Iterators are meant to work for deque even if items are removed (and pop is last in for loop so shouldn't break things)
    // TODO: Support secondary hosts for some messages (//for( Config::OSCHost h : Config::getConfig().network.secondary_hosts ) { )
      Config::OSCHost h = Config::getConfig().network.master_host;
      if( OSC::heartBeatOK ) {
        Serial.print("Sending: "); Serial.print(msg.command); Serial.print(" to "); Serial.print(h.name); Serial.print(":"); Serial.println(h.port);
        OscMessage o(h.host, h.port, msg.command);
        if( msg.has_arg_int )
          o.push(msg.arg_int);
        
        //OscEther.send(o);
      }
    if( !msg.wait_for_heartbeat || OSC::heartBeatOK )
      currentQueue.pop_front();
  }
  // TODO: Support secondary hosts for some messages
  Config::OSCHost h = Config::getConfig().network.master_host;
  if( OSC::sendHeartBeat ) {
    OscEther.send(h.host, h.port, "/thump");
    OscEther.send(h.host, h.port, "/runningOrPausedCues");
  }
  if( OSC::sendHeartBeat )
    OSC::sendHeartBeat = false;
  yield();
  //osc.parse();  
}

void OSC::updateDisplay() {
  d->clearTextArea();
  d->tft->setTextColor(BLUE);
  d->tft->setTextScale(1,1);
  d->tft->setFont(&mono_mid);
  d->tft->println("Next:");
  // One line, smaller
  d->tft->println(" " + Display::truncateString(nextCue.displayName, 13));
  d->tft->setTextScale(1,2);
  d->tft->setTextColor(GREEN);
  // Allow over 2 lines
  d->tft->println(Display::truncateString(runningCue.displayName, 29));
}
