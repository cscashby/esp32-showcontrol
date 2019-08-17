#include <ArduinoOSC.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <SSD_13XX.h> //https://github.com/sumotoy/SSD_13XX
#include <AutoConnect.h>

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

// OSC variables (TODO: Externalise)
const int OSC_PORT = 53001;
const char* HOST_IP = "192.168.5.104";
const int HOST_PORT = 53000;
const unsigned long OSC_HEARTBEAT_INTERVAL = 1000;
unsigned long oscHeartbeatTimer;
bool oscHeartbeatState = false;
OscWiFi osc;

// LED and button variables
const long LED_TIME = 1000;
const long DEBOUNCE_TIME = 100;
#define COUNT 4

// PIN, state, lastChangeMillis
LED go_led = { 33, false, -1 };
LED stop_led = { 3, false, -1 };
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

// Display variables
#define __CS    5  
#define __DC    16 
#define __RST   32
SSD_13XX tft = SSD_13XX(__CS, __DC, __RST);
void drawHeartbeat(bool state) {
  if( state ) {
    tft.setTextColor(GREEN,BLACK);
  } else {
    tft.setTextColor(BLACK,BLACK);
  }
  tft.setTextScale(2);
  tft.setCursor(80, 0);
  tft.print("X");
}

// Webserver / WiFi variables
WebServer webServer;
AutoConnect portal(webServer);
void rootPage() {
  char content[] = "Hello, world";
  webServer.send(200, "text/plain", content);
}

// Interrupt routine for buttons
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
  delay(1000);
  Serial.begin(115200);

  // Wifi / webserver initialisation
  Serial.println();
  webServer.on("/", rootPage);
  if (portal.begin()) {
    Serial.println("HTTP server:" + WiFi.localIP().toString());
  }

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

  // OSC initialisation
  osc.begin(OSC_PORT);
  oscHeartbeatTimer = millis();
  // add callbacks...
  osc.subscribe("/reply/workspace/*/thump", [](OscMessage& m)
  {
      // We either draw or erase our heart
      oscHeartbeatState = !oscHeartbeatState;
      drawHeartbeat(oscHeartbeatState);
  });
}

void loop() {  
  unsigned long now = millis();
  for( int c = 0; c < COUNT; c++ ) {
    if( buttons[c]->pressed ) {
      Serial.printf("%s has been pressed %u times\n", names[c], buttons[c]->pressCount);
      tft.clearScreen();
      tft.setCursor(0, 0);
      tft.setTextColor(WHITE);
      tft.setTextScale(4);
      tft.println(names[c]);
      drawHeartbeat(oscHeartbeatState);
      buttons[c]->pressed = false;
    }
    if( leds[c]->state && (now - leds[c]->lastChangeMillis) >= LED_TIME) {
      leds[c]->state = false;
      digitalWrite(leds[c]->PIN, !leds[c]->state);
    }
  }

  // And now handle the web clients if we have any
  portal.handleClient();

  // And deal with OSC
  osc.parse(); // should be called
  if( (now - oscHeartbeatTimer) >= OSC_HEARTBEAT_INTERVAL) {
    OscMessage msg(HOST_IP, HOST_PORT, "/thump");
    osc.send(msg);
    oscHeartbeatTimer = millis();
  }
}
