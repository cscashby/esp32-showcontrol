#include <Arduino.h>
#include "Display.h"
#include "Network.h"

Display d = Display();

hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  d.loop();
  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Network::begin();

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  // uS units, so this is 1s
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
}

void loop() {
  Network::loop();
}
