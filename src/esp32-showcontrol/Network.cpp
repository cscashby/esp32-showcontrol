#include "Network.h"
#include "config.h"

WebServer Network_webServer;

bool Network::eth_connected;
bool Network::wifi_connected;

Display* Network::d = NULL;

void Network::WiFiEvent(WiFiEvent_t event)
{
  switch (event) {
    case SYSTEM_EVENT_ETH_START:
      Serial.println("ETH Started");
      Network::displayDetails();
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
      Network::displayDetails();
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
      Serial.print("ETH MAC: ");
      Serial.print(ETH.macAddress());
      Serial.print(", IPv4: ");
      Serial.print(ETH.localIP());
      if (ETH.fullDuplex()) {
        Serial.print(", FULL_DUPLEX");
      }
      Serial.print(", ");
      Serial.print(ETH.linkSpeed());
      Serial.println("Mbps");
      eth_connected = true;
      Network::displayDetails();
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      Network::displayDetails();
      break;
    case SYSTEM_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      Network::displayDetails();
      break;
    case SYSTEM_EVENT_STA_CONNECTED:
      Serial.println("WiFi AP connected");
      wifi_connected = true;
      Network::displayDetails();
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi AP disconnected");
      wifi_connected = false;
      Network::displayDetails();
      break;
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.print("WiFi MAC: ");
      Serial.print(WiFi.macAddress());
      Serial.print(", IPv4: ");
      Serial.print(WiFi.localIP());
      wifi_connected = true;
      Network::displayDetails();
      break;
    case SYSTEM_EVENT_STA_LOST_IP:
      Serial.println("WiFi AP lost IP");
      wifi_connected = false;
      Network::displayDetails();
      break;
    default:
      Serial.print("WifiEvent: ");
      Serial.println(event);
      break;
  }
}

void Network::begin(Display* d) {
  Network::d = d;
  
  eth_connected = false;
  wifi_connected = false;
  WiFi.onEvent(Network::WiFiEvent);

  ETH.begin();
  
  Network_webServer.on("/", Network::rootPage);
  Network_webServer.on("/main.js", Network::mainJS);
  Network_webServer.on("/set/master", Network::handle_setMaster);
  Network_webServer.on("/get", Network::handle_get);
  Network_webServer.onNotFound(Network::handle404);
  Network_webServer.begin();
}

void Network::loop() {
  Network_webServer.handleClient();
}

void Network::rootPage() {
  Network_webServer.send(200, "text/html", WEBPAGE_ROOT);
}

void Network::mainJS() {
  Network_webServer.send(200, "text/javascript", MAINJS_ROOT);
}

void Network::handle_setMaster() {
  if (Network_webServer.method() != HTTP_POST) {
    Network_webServer.send(405, "text/plain", "Method Not Allowed");
  } else {
    // TODO: Definitely need to validate the form somehow!
    Serial.println("New master: " + Network_webServer.arg("master_name") + " IP: " + Network_webServer.arg("master_ip") + ":" + Network_webServer.arg("master_port"));
    Config::getConfig().network.master_host.name = Network_webServer.arg("master_name");
    Config::getConfig().network.master_host.host = Network_webServer.arg("master_ip");
    Config::getConfig().network.master_host.port = Network_webServer.arg("master_port").toInt();
    Network_webServer.send(205, "text/plain");
  }
}

void Network::handle_get() {
  Network_webServer.send(200, "application/json", CONFIG_JSON);
}

void Network::handle404() {
  String message = "File Not Found\n\n";
    message += "URI: ";
    message += Network_webServer.uri();
    message += "\nMethod: ";
    message += ( Network_webServer.method() == HTTP_GET ) ? "GET" : "POST";
    message += "\nArguments: ";
    message += Network_webServer.args();
    message += "\n";
    for ( uint8_t i = 0; i < Network_webServer.args(); i++ ) {
        message += " " + Network_webServer.argName ( i ) + ": " + Network_webServer.arg ( i ) + "\n";
    }
    Network_webServer.send ( 404, "text/plain", message );
}

void Network::displayDetails() {
  // We only run this display update if the main loop is not running.
  // TODO: Move to Display.h/cpp with parameterised text
  if( !Config::mainLoopStarted ) {
    d->clearTextArea();
    d->tft->setTextColor(WHITE);
    d->tft->setTextScale(1,2);
    d->tft->setFont(&mono_mid);
    d->tft->println("Press to start");
    d->tft->setTextScale(1,1);
    d->tft->setTextColor(BLUE);
    d->tft->println(ETH.macAddress());
    if( eth_connected ) {
      d->tft->setTextColor(ETH.linkSpeed() == 100 ? GREEN : MAGENTA);
      d->tft->setTextScale(1,2);
      d->tft->print(ETH.localIP());
      d->tft->setTextScale(1,1);
      d->tft->setCursor(77,31);
      d->tft->println(ETH.linkSpeed());
      d->tft->setCursor(77,38);
      d->tft->println(ETH.fullDuplex() ? "F" : "");
      // TODO: Is there a timing issue here? Is config ready?
      d->tft->setTextScale(1,1);
      d->tft->setTextColor(YELLOW);
      d->tft->print("m: ");
      d->tft->println(Config::getConfig().network.master_host.host);
    } else {
      d->tft->setTextColor(RED);
      d->tft->setTextScale(2,2);
      d->tft->println("No IP");
    }
  }
}
