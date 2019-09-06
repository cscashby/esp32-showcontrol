#include "Config.h"

Config::Config() {
  StaticJsonDocument<1500> doc;
  DeserializationError error = deserializeJson(doc, CONFIG_JSON);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    // If I get here, I'm very sad
    while(true) {};
  }

  //serializeJson(doc, Serial);
  JsonObject network = doc.as<JsonObject>()["network"].as<JsonObject>();
  netConfig.hostname = network["hostname"].as<char*>();
  
  JsonObject buttons = doc.as<JsonObject>()["buttons"].as<JsonObject>();
  for( JsonPair kv : buttons ) {
    JsonObject button = kv.value().as<JsonObject>();
    std::vector<String> cmds;
    
    Button b = Button(kv.key().c_str(), button["Button_pin"].as<uint8_t>(), button["LED_pin"].as<uint8_t>());
    if( button.containsKey("OSC_commands") ) {
      for( JsonVariant c : button["OSC_commands"].as<JsonArray>() ) {
        b.OSC_commands.push_back(c.as<char*>());
      }
    }
    Config::OSC_subscription sub = {
      .button = &b,
      .string = button["OSC_subscribe"]["string"].as<char*>()
    };
    this->buttons.push_back(b);
  }
}
