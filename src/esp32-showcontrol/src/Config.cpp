#include "Config.h"

bool Config::mainLoopStarted = false;

Config::Config() {
  StaticJsonDocument<2500> doc;
  DeserializationError error = deserializeJson(doc, CONFIG_JSON);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    // If I get here, I'm very sad
    while(true) {};
  }

  //serializeJson(doc, Serial);
  JsonObject net = doc.as<JsonObject>()["network"].as<JsonObject>();
  network.hostname = net["hostname"].as<String>();
  JsonObject jo_master = net["master_host"].as<JsonObject>();
  network.master_host.name = jo_master["name"].as<String>();
  network.master_host.host = jo_master["host"].as<String>();
  network.master_host.port = jo_master["port"].as<uint16_t>();
  Serial.print("Adding master host: "); Serial.println(network.master_host.name); 
  JsonObject secondary_hosts = net["secondary_hosts"].as<JsonObject>();
  for( JsonPair kv : secondary_hosts ) {
    JsonObject jo = kv.value().as<JsonObject>();
    OSCHost h = {
      .name = jo["name"].as<String>(),
      .host = jo["host"].as<String>(),
      .port = jo["port"].as<uint16_t>()
    };
    Serial.print("Adding secondary host: "); Serial.println(h.name);
    network.secondary_hosts.push_back(h);
  }
  network.osc_listen_port = net["osc_listen_port"].as<uint16_t>();
  
  JsonObject general = doc.as<JsonObject>()["general"].as<JsonObject>();
  timers.led_flash_ms = general["timers"].as<JsonObject>()["led_flash_ms"].as<unsigned long>();
  timers.switch_debounce_ms = general["timers"].as<JsonObject>()["switch_debounce_ms"].as<unsigned long>();
  timers.heartbeat_timeout_ms = general["timers"].as<JsonObject>()["heartbeat_timeout_ms"].as<unsigned long>();
  timers.heartbeat_us = general["timers"].as<JsonObject>()["heartbeat_us"].as<uint64_t>();
  timers.poll_us = general["timers"].as<JsonObject>()["poll_us"].as<uint64_t>();
  JsonArray startOSC = general["start_osc"].as<JsonArray>();
  for( JsonObject osco : startOSC ) {
    SCOSCMessage oscm = {
      .command = osco["string"].as<String>(),
      .has_arg_int = osco.containsKey("arg_int")
    };
    if( oscm.has_arg_int )
        oscm.arg_int = osco["arg_int"].as<uint8_t>();

    startOSCMessages.push_back(oscm);
  }
  
  JsonObject buttons = doc.as<JsonObject>()["buttons"].as<JsonObject>();
  for( JsonPair kv : buttons ) {
    JsonObject button = kv.value().as<JsonObject>();
    std::vector<String> cmds;
    
    Button b = Button(kv.key().c_str(), button["Button_pin"].as<uint8_t>(), button["LED_pin"].as<uint8_t>());
    if( button.containsKey("LED_inverted") ) {
      b.invert = button["LED_inverted"].as<bool>();
    }
    if( button.containsKey("OSC_messages") ) {
      for( JsonVariant c : button["OSC_messages"].as<JsonArray>() ) {
        SCOSCMessage msg;
        msg.command = c.as<String>();
        b.OSC_messages.push_back(msg);
      }
    }
    Config::OSCSubscription sub = {
      .button = &b,
      .string = button["OSC_subscribe"]["string"].as<String>(),
      .type = button["OSC_subscribe"]["type"].as<String>()
    };
    this->OSCsubscriptions.push_back(sub);
    this->buttons.push_back(b);
  }
}
