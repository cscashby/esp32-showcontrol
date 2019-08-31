#include "Display.h"

Display d = Display();

void setup() {
  delay(1000);
  Serial.begin(115200);
}

void loop() {
  d.heartBeat();
  delay(500);
}
