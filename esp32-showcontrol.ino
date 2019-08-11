#include <Arduino.h>
#include <SPI.h>
#include <SSD_13XX.h> //https://github.com/sumotoy/SSD_13XX

struct LED {
    const uint8_t PIN;
    bool state;
    unsigned long lastChangeMillis;
};

struct Button {
    const uint8_t PIN;
    long pressCount;
    bool pressed;
    unsigned long debounceMillis;
    LED* led;
};

const long LED_TIME = 1000;
const long DEBOUNCE_TIME = 100;
#define COUNT 4

// PIN, state, lastChangeMillis
LED go_led = { 33, false, -1 };
LED stop_led = { 15, false, -1 };
LED prev_led = { 4, false, -1 };
LED next_led = { 13, false, -1 };
// PIN, pressCount, pressed, debounceMillis, led
Button go_button = {34, 0, false, -1, &go_led};
Button stop_button = {35, 0, false, -1, &stop_led};
Button prev_button = {36, 0, false, -1, &prev_led};
Button next_button = {39, 0, false, -1, &next_led};

char *names[] = { "go", "stop", "prev", "next" };
LED* leds[]= { &go_led, &stop_led, &prev_led, &next_led };
Button* buttons[] = { &go_button, &stop_button, &prev_button, &next_button };

#define __CS    5  
#define __DC    16 
#define __RST   32
SSD_13XX tft = SSD_13XX(__CS, __DC, __RST);

void IRAM_ATTR isr(void* arg) {
    Button* s = static_cast<Button*>(arg);
    unsigned long now = millis();
    if( (now - s->debounceMillis) >= DEBOUNCE_TIME ) {
      s->debounceMillis = millis();
      s->pressCount++;
      s->pressed = true;
      s->led->state = true;
      s->led->lastChangeMillis = millis();
      digitalWrite(s->led->PIN, !s->led->state);
    }
}

void setup() {
    Serial.begin(115200);

    // Get pins read for buttons and LEDs
    for( int c = 0; c < COUNT; c++ ) {
      pinMode(buttons[c]->PIN, INPUT_PULLUP);
      attachInterruptArg(buttons[c]->PIN, isr, buttons[c], FALLING);
      pinMode(leds[c]->PIN, OUTPUT);
      digitalWrite(leds[c]->PIN, true);
    }

    // And now the screen
    tft.begin();
    tft.clearScreen();
}

void loop() {  
    unsigned long now = millis();
    for( int c = 0; c < COUNT; c++ ) {
        if( buttons[c]->pressed ) {
          Serial.printf("%s has been pressed %u times\n", names[c], buttons[c]->pressCount);
          tft.clearScreen();
          tft.setCursor(0, 0);
          tft.setTextColor(WHITE);
          tft.setTextScale(1);
          tft.println(names[c]);
          buttons[c]->pressed = false;
        }
        if( leds[c]->state && (now - leds[c]->lastChangeMillis) >= LED_TIME) {
          leds[c]->state = false;
          digitalWrite(leds[c]->PIN, !leds[c]->state);
        }
    }    
}

