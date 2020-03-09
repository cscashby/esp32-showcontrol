#include "Button.h"
#include "Config.h"

static std::map<uint8_t, Button*> buttonMap;

void IRAM_ATTR button_isr(void* arg) {
  Button* b = static_cast<Button*>(arg);
  b->isr();
}

Button::Button(const String n, const uint8_t p, const uint8_t l)
      : buttonName(n), pin(p), LED_pin(l) { 
  buttonMap.insert(std::make_pair(p, this));
  invert = false;
}

void Button::begin() {
  pinMode(pin, INPUT);
  attachInterruptArg(pin, button_isr, this, FALLING);
  pinMode(LED_pin, OUTPUT);
  digitalWrite(LED_pin, invert);
}

void IRAM_ATTR Button::isr() {
  unsigned long now = millis();
  if( (now - debounceMillis) >= Config::getConfig().timers.switch_debounce_ms ) {
    debounceMillis = millis();
    pressed = true;
    digitalWrite(LED_pin, !invert);
    ledLastChangeMillis = millis();
    char* m;
    unsigned int i = 0;
    Serial.print("Button pressed: ");
    Serial.println(buttonName);
    for( SCOSCMessage msg : OSC_messages ) {
      OSC::queueOSCMessage(msg);
    }
  }
}

void IRAM_ATTR Button::poll() {
  unsigned long now = millis();
  if( (now - ledLastChangeMillis) > Config::getConfig().timers.led_flash_ms ) {
    digitalWrite(LED_pin, invert);
    ledLastChangeMillis = 0;
  }
}

void Button::loop() {
  
}
