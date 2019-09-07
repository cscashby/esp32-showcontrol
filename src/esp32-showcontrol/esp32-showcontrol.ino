#include <Arduino.h>
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

void IRAM_ATTR hearbeatTimerISR() {
// Needed?
//  portENTER_CRITICAL_ISR(&timerMux);
  d.heartBeat();
//  portEXIT_CRITICAL_ISR(&timerMux);
}

void IRAM_ATTR pollTimerISR() {
  for (unsigned i=0; i < Config::getConfig().buttons.size(); i++) {
    Config::getConfig().buttons.at(i).poll();
  }
}

void setup() {
  Serial.begin(115200);
  //esp_log_level_set("*", ESP_LOG_DEBUG);

  d.begin();
  Network::begin(d);
  OSC::begin();
  for (unsigned i=0; i < Config::getConfig().buttons.size(); i++) {
    Config::getConfig().buttons.at(i).begin();
  }

  heartbeatTimer = timerBegin(HEARTBEAT_TIMER, 80, true);
  timerAttachInterrupt(heartbeatTimer, &hearbeatTimerISR, true);
  timerAlarmWrite(heartbeatTimer, Config::getConfig().timers.heartbeat_us, true);
  timerAlarmEnable(heartbeatTimer);

  pollTimer = timerBegin(POLL_TIMER, 80, true);
  timerAttachInterrupt(pollTimer, &pollTimerISR, true);
  timerAlarmWrite(pollTimer, Config::getConfig().timers.poll_us, true);
  timerAlarmEnable(pollTimer);
}

void loop() {
  Network::loop();
  OSC::loop();
}
