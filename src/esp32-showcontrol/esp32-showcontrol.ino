#include <Arduino.h>
#include "Display.h"
#include "Network.h"
#include "Config.h"
#include "Button.h"

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

  d.begin();
  Network::begin(d);

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  // uS units, so this is 1s
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);

  for (unsigned i=0; i < Config::getConfig().buttons.size(); i++) {
    Button b = Config::getConfig().buttons.at(i);
    Serial.print(b.name); Serial.print(" -- ");
    Serial.print("LED: "); Serial.print(b.LED_pin);
    Serial.print(" Button: "); Serial.print(b.pin);
    for (unsigned j=0; j < b.OSC_commands.size(); j++) {
      Serial.print(" Command: "); Serial.print(b.OSC_commands.at(j));
    }
    Serial.println();
  }
}

void loop() {
  Network::loop();
}
