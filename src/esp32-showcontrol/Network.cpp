#include "Network.h"

WebServer Network_webServer;
AutoConnect Network_portal(Network_webServer);

bool Network::eth_connected;
bool Network::wifi_connected;

void Network::WiFiEvent(WiFiEvent_t event)
{
  switch (event) {
    case SYSTEM_EVENT_ETH_START:
      Serial.println("ETH Started");
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
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
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;
    case SYSTEM_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;
    case SYSTEM_EVENT_STA_CONNECTED:
      Serial.println("WiFi AP connected");
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi AP disconnected");
      wifi_connected = false;
      break;
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.print("WiFi MAC: ");
      Serial.print(WiFi.macAddress());
      Serial.print(", IPv4: ");
      Serial.print(WiFi.localIP());
      wifi_connected = true;
      break;
    case SYSTEM_EVENT_STA_LOST_IP:
      Serial.println("WiFi AP lost IP");
      wifi_connected = false;
      break;
    default:
      Serial.print("WifiEvent: ");
      Serial.println(event);
      break;
  }
}

void Network::begin(Display d) {
  eth_connected = false;
  wifi_connected = false;
  WiFi.onEvent(Network::WiFiEvent);
  
  d.tft->clearScreen();
  d.tft->setCursor(0, 0);
  d.tft->setTextScale(1,2);
  d.tft->setFont(&mono_mid);
  d.tft->println();
  d.tft->setTextColor(WHITE);
  d.tft->println("-- Ethernet --");

  ETH.begin();
  
  d.tft->clearScreen();
  d.tft->println();
  d.tft->setTextColor(WHITE);
  d.tft->println("   -- WiFi --");
  
  Network_webServer.on("/", Network::rootPage);
  Network_portal.onNotFound(Network::handle404);

  AutoConnectConfig config;
  config.hostName = Config::getConfig().netConfig.hostname;
  Network_portal.config(config);

  if (Network_portal.begin()) {
    Serial.println("HTTP server:" + WiFi.localIP().toString());
  }

  d.tft->clearScreen();
}

void Network::loop() {
  Network_portal.handleClient();
}

void Network::rootPage() {
  Network_webServer.send(200, "text/html", WEBPAGE_ROOT);
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
