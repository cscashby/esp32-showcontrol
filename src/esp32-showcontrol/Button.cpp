#include "Button.h"

static std::map<uint8_t, Button*> buttonMap;

void IRAM_ATTR button_isr(void* arg) {
  Button* b = static_cast<Button*>(arg);
  b->isr();
}

Button::Button(const String n, const uint8_t p, const uint8_t l)
      : buttonName(n), pin(p), LED_pin(l) { 
  buttonMap.insert(std::make_pair(p, this));
}

void Button::begin() {
  pinMode(pin, INPUT);
  attachInterruptArg(pin, button_isr, this, FALLING);
  pinMode(LED_pin, OUTPUT);
  digitalWrite(LED_pin, false);
}

void IRAM_ATTR Button::isr() {
  unsigned long now = millis();
  if( (now - debounceMillis) >= DEBOUNCE_TIME ) {
    debounceMillis = millis();
    pressed = true;
    digitalWrite(LED_pin, true);
    ledLastChangeMillis = millis();
    char* m;
    unsigned int i = 0;
    Serial.print("Button pressed: ");
    Serial.println(buttonName);
  }
}

void Button::loop() {
  
}

void Button::poll() {
  unsigned long now = millis();
  if( (now - ledLastChangeMillis) > LED_TIME ) {
    digitalWrite(LED_pin, false);
    ledLastChangeMillis = 0;
  }
}
