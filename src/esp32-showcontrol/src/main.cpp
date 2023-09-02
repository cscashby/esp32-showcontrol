#include <Arduino.h>
#include <ETH.h>
#include "Display.h"
#include "Network.h"
#include "Config.h"
#include "Button.h"
#include "const.h"

#include <esp_log.h>

Display d = Display();

hw_timer_t *heartbeatTimer = NULL;
hw_timer_t *pollTimer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

bool postInitDone = false;

void IRAM_ATTR hearbeatTimerISR() {
  if( Config::mainLoopStarted ) { 
    OSC::heartbeat();
  }
}

void IRAM_ATTR pollTimerISR() {
  for (unsigned i=0; i < Config::getConfig().buttons.size(); i++) {
    Config::getConfig().buttons.at(i).poll();
  }
}

void setup() {
  Serial.begin(460800);
  esp_log_level_set("*", ESP_LOG_DEBUG);

  // No OSC yet - we wait for a button press
  Config::mainLoopStarted = false;

  d.begin();
  Network::begin(&d);
  for (unsigned i=0; i < Config::getConfig().buttons.size(); i++) {
    Config::getConfig().buttons.at(i).begin();
  }

  pollTimer = timerBegin(POLL_TIMER, 80, true);
  timerAttachInterrupt(pollTimer, &pollTimerISR, true);
  timerAlarmWrite(pollTimer, Config::getConfig().timers.poll_us, true);
  timerAlarmEnable(pollTimer);  
}

void doPostInit() {
  d.clearTextArea();

  OSC::begin(&d);
  
  heartbeatTimer = timerBegin(HEARTBEAT_TIMER, 80, true);
  timerAttachInterrupt(heartbeatTimer, &hearbeatTimerISR, true);
  timerAlarmWrite(heartbeatTimer, Config::getConfig().timers.heartbeat_us, true);
  timerAlarmEnable(heartbeatTimer);    
  
  postInitDone = true;
}

void loop() {
  Network::loop();
  
  if( Config::mainLoopStarted ) { 
    if( !postInitDone )
      doPostInit();
    
    OSC::loop();
  }
}
