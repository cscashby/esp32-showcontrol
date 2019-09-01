#include "Network.h"

WebServer Network_webServer;
AutoConnect Network_portal(Network_webServer);

void Network::begin() {
  Network_webServer.on("/", Network::rootPage);
  Network_portal.onNotFound(Network::handle404);

  if (Network_portal.begin()) {
    Serial.println("HTTP server:" + WiFi.localIP().toString());
  }
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
