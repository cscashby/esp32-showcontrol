#include <ArduinoOSC.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <SSD_13XX.h> //https://github.com/sumotoy/SSD_13XX
#include <AutoConnect.h>
#include <QueueArray.h>
#include "_fonts/mono_mid.c"

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
  const char** oscMessages;
};

struct CueDisplay {
  String cueID = "";
  String displayName = "";
};

// OSC variables (TODO: Externalise)
const int OSC_PORT = 53001;
const String HOST_IP = "192.168.5.104";
const int HOST_PORT = 53000;
const unsigned long OSC_HEARTBEAT_INTERVAL = 500;
unsigned long oscHeartbeatTimer;
const unsigned long QLAB_KEEPALIVE_INTERVAL = 30000;
unsigned long qlabKeepAliveTimer;
bool oscHeartbeatState = false;
OscWiFi osc;
QueueArray <char*> oscStringsPending;

CueDisplay runningCue;
CueDisplay nextCue;

// LED and button variables
const long LED_TIME = 1000;
const long DEBOUNCE_TIME = 500;
#define COUNT 4

// PIN, state, lastChangeMillis
LED go_led = { 0, false, -1 };
LED stop_led = { 15, false, -1 };
LED prev_led = { 16, false, -1 };
LED next_led = { 32, false, -1 };
const char* names[] = { "go", "stop", "prev", "next" };
LED* leds[]= { &go_led, &stop_led, &prev_led, &next_led };
// Needs empty string termination for iterator
const char* buttonMessagesGo[] = {"/stop", "/go", ""};
const char* buttonMessagesStop[] = {"/stop", ""};
const char* buttonMessagesPrev[] = {"/stop", "/playhead/previous", ""};
const char* buttonMessagesNext[] = {"/stop", "/playhead/next", ""};
// PIN, pressCount, pressed, debounceMillis, led
Button go_button = {34, 0, false, -1, &go_led, buttonMessagesGo};
Button stop_button = {35, 0, false, -1, &stop_led, buttonMessagesStop};
Button prev_button = {36, 0, false, -1, &prev_led, buttonMessagesPrev};
Button next_button = {39, 0, false, -1, &next_led, buttonMessagesNext};
Button* buttons[] = { &go_button, &stop_button, &prev_button, &next_button };
#define LINE_LENGTH 13

// We use this to decode JSON
StaticJsonDocument<5000> doc;

// Display variables
#define __SCL   14 //unused
#define __SDA   2  //unused
#define __CS    5  
#define __DC    4 
#define __RST   3
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
    char* m;
    unsigned int i = 0;
    Serial.print("Button pressed, ");
    while( true ) {
      m = (char *)s->oscMessages[i++];
      if( m == "" )
        break;
      Serial.printf("sending %s (%d), ", m, i-1);
      oscStringsPending.enqueue(m);
    }
    Serial.println();
    digitalWrite(s->led->PIN, !s->led->state);
  }
}

String truncateString(String in, long maxChars) {
  if( in.length() > maxChars ) {
    return in.substring(0, maxChars - 3) + "...";
  } else {
    return in;
  }
}

void sendOscRegularCommands() {
  OscMessage m1(HOST_IP, HOST_PORT, "/updates");
  m1.push(1);
  osc.send(m1);
  OscMessage m2(HOST_IP, HOST_PORT, "/alwaysReply");
  m2.push(1);
  osc.send(m2);
}

void cuesChanged() {
  // Redraw the screen with running and new cues
  tft.clearScreen();
  tft.setCursor(0, 0);
  tft.setTextColor(BLUE);
  tft.setTextScale(1,2);
  tft.setFont(&mono_mid);
  tft.println(truncateString(nextCue.displayName, LINE_LENGTH));
  if( nextCue.displayName.length() <= LINE_LENGTH )
    tft.println();
  tft.setTextColor(GREEN);
  tft.println(truncateString(runningCue.displayName, LINE_LENGTH));
  drawHeartbeat(oscHeartbeatState);
}

// BEGIN Arduino standard functions
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
  osc.subscribe("/reply/workspace/*/thump", [](OscMessage& m) {
    // We either draw or erase our heart
    oscHeartbeatState = !oscHeartbeatState;
    drawHeartbeat(oscHeartbeatState);
  });
  osc.subscribe("/reply/go", [](OscMessage& m) {
    oscStringsPending.enqueue("/runningOrPausedCues");
  });
  osc.subscribe("/reply/stop", [](OscMessage& m) {
    oscStringsPending.enqueue("/runningOrPausedCues");
  });
  osc.subscribe("/reply/cue_id/*/displayName", [](OscMessage& m) {
    // We copy the address as we don't want to break the string when we tokenise it
    String addr = m.address();
    char sizeString[] = "/reply/cue_id/0FD30761-96F0-4C1E-AD1A-155DEB772604/displayName";
    // TODO Make this a function!
    char a[sizeof(sizeString)];
    addr.toCharArray(a, sizeof(sizeString));
    
    Serial.print("Address: ");
    Serial.println(a);
    
    char* p = strtok(a, "/");
    char* split[6];
    int i = 0;
    while(p != NULL) {
      split[i++] = p;
      p = strtok(NULL, "/");
    }
    String cueID = split[2];
    Serial.print("Cue: ");
    Serial.println(cueID);

    String json = m.arg<String>(0);
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, json);
    const char* displayName;
    // Test if parsing succeeds.
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      displayName = "error";
    } else {
      displayName = doc["data"]; // "2"
    }
    Serial.print("Cue name found: ");
    Serial.println(displayName);
    // If we don't have the cue ID yet, then we probably have requested the current cue, which is by definition the next one
    if( nextCue.cueID.equals("selected")) {
      nextCue.cueID = cueID;
    }
    if( nextCue.cueID.equals(cueID) ) {
      nextCue.displayName = displayName;
    }
    cuesChanged();
  });
  osc.subscribe("/reply/workspace/*/runningOrPausedCues", [](OscMessage& m) {
    // We copy the address as we don't want to break the string when we tokenise it
    String addr = m.address();
    char sizeString[] = "/workspace/FBD9B081-1C68-4C9C-8B74-98712F4DD90B/runningOrPausedCues";
    
    String json = m.arg<String>(0);
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, json);
    const char* displayName;
    const char* cueID;
    // Test if parsing succeeds.
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      displayName = "error";
      cueID = "";
    } else {
      displayName = doc["data"][0]["name"];
      cueID = doc["data"][0]["uniqueID"];
    }
//    Serial.print("Running cue: ");
//    Serial.print(cueID);
//    Serial.print(": ");
//    Serial.println(displayName);
    runningCue.cueID = cueID;
    runningCue.displayName = (displayName == NULL || strlen(displayName) == 0) ? "---" : displayName;
    cuesChanged();
  });
  osc.subscribe("/update/workspace/*/cueList/*/playbackPosition", [](OscMessage& m) {
    // We copy the address as we don't want to break the string when we tokenise it
    String addr = m.address();
    char sizeString[] = "/update/workspace/FBD9B081-1C68-4C9C-8B74-98712F4DD90B/cueList/0FD30761-96F0-4C1E-AD1A-155DEB772604/playbackPosition";
    // TODO Make this a function!
    char a[sizeof(sizeString)];
    addr.toCharArray(a, sizeof(sizeString));
    
    Serial.print("Address: ");
    Serial.println(a);
    
    char* p = strtok(a, "/");
    char* split[6];
    int i = 0;
    while(p != NULL) {
      split[i++] = p;
      p = strtok(NULL, "/");
    }
    Serial.print("Workspace: ");
    Serial.println(split[2]);

    String cueID = m.arg<String>(0);
    Serial.print("Cue: ");
    Serial.println(cueID);

    // For some reason we get a space from the library so let's just check for >1 length not >0
    if( cueID.length() == 36 ) {
      String mstr = "/cue_id/" + cueID + "/displayName";
      OscMessage m(HOST_IP, HOST_PORT, mstr);
      osc.send(m);
      nextCue.cueID = cueID;
    } else {
      nextCue.cueID = "";
      nextCue.displayName = "---";
      cuesChanged();
    }
    // Now check to see what's running
    oscStringsPending.enqueue("/runningOrPausedCues");
  });
  // Ask for updates and replies, and update screen with appropriate stuff
  nextCue.cueID = "selected";
  oscStringsPending.enqueue("/cue/selected/displayName");
  oscStringsPending.enqueue("/runningOrPausedCues");
  oscStringsPending.enqueue("/displayName");
  osc.parse();
  sendOscRegularCommands();
}

void loop() {  
  unsigned long now = millis();
  for( int c = 0; c < COUNT; c++ ) {
    if( buttons[c]->pressed ) {
      Serial.printf("%s has been pressed %u times\n", names[c], buttons[c]->pressCount);
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
  osc.parse();
  if( (now - oscHeartbeatTimer) >= OSC_HEARTBEAT_INTERVAL) {
    oscStringsPending.enqueue("/thump");
    // TODO: I don't want to do this each time, but it doesn't work otherwise - yet.
    oscStringsPending.enqueue("/runningOrPausedCues");
    oscHeartbeatTimer = millis();
  }
  if( (now - qlabKeepAliveTimer) >= QLAB_KEEPALIVE_INTERVAL) {
    sendOscRegularCommands();
    qlabKeepAliveTimer = millis();
  }
  while( !oscStringsPending.isEmpty() ) {
    OscMessage m(HOST_IP, HOST_PORT, oscStringsPending.dequeue());
    osc.send(m);
  }
}
