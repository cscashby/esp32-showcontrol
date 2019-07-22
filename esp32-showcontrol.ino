#include <Arduino.h>

struct LED {
    const uint8_t PIN;
    bool state;
    unsigned long lastChangeMillis;
};

struct Button {
    const uint8_t PIN;
    uint32_t numberKeyPresses;
    bool pressed;
    LED* led;
};

const long LED_TIME = 1000;
#define COUNT 4

LED go_led = { 33, false, -1 };
LED stop_led = { 15, false, -1 };
LED prev_led = { 4, false, -1 };
LED next_led = { 13, false, -1 };

Button go_button = {34, 0, false, &go_led};
Button stop_button = {35, 0, false, &stop_led};
Button prev_button = {36, 0, false, &prev_led};
Button next_button = {39, 0, false, &next_led};

char *names[] = { "go", "stop", "prev", "next" };

LED* leds[]= { &go_led, &stop_led, &prev_led, &next_led };
Button* buttons[] = { &go_button, &stop_button, &prev_button, &next_button };

void IRAM_ATTR isr(void* arg) {
    Button* s = static_cast<Button*>(arg);
    s->numberKeyPresses += 1;
    s->pressed = true;
    s->led->state = true;
    s->led->lastChangeMillis = millis();
    digitalWrite(s->led->PIN, !s->led->state);
}

void setup() {
    Serial.begin(115200);
    for( int c = 0; c < COUNT; c++ ) {
      pinMode(buttons[c]->PIN, INPUT_PULLUP);
      attachInterruptArg(buttons[c]->PIN, isr, buttons[c], FALLING);
      pinMode(leds[c]->PIN, OUTPUT);
      digitalWrite(leds[c]->PIN, true);
    }
}

void loop() {
    for( int c = 0; c < COUNT; c++ ) {
        if( buttons[c]->pressed ) {
          Serial.printf("%s has been pressed %u times\n", names[c], buttons[c]->numberKeyPresses);
          buttons[c]->pressed = false;
          // TODO: Debounce me!
        }
        unsigned long now = millis();
        if( leds[c]->state && (now - leds[c]->lastChangeMillis) >= LED_TIME) {
          leds[c]->state = false;
          digitalWrite(leds[c]->PIN, !leds[c]->state);
        }
    }    
}

