const char CONFIG_JSON[] PROGMEM = R"=====(
{
  "network": {
    "hostname": "esp32-showcontrol",
    "osc_hosts": {
      "test-imac": {
        "host": "192.168.5.104",
        "port": 53000
      }
    },
    "osc_listen_port": 53001
  },
  "general": {
    "timers" : {
      "led_flash_ms": 1000,
      "switch_debounce_ms": 200,
      "heartbeat_timeout_ms": 1500,
      "heartbeat_us": 500000,
      "poll_us": 100000
    },
    "regular_osc" : [
      {
        "string": "/updates",
        "arg_int": 1
      },
      {
        "string": "/alwaysReply",
        "arg_int": 1
      }
    ]
  },
  "buttons": {
    "go": {
      "LED_pin": 0,
      "Button_pin": 33,
      "OSC_messages": [
        "/stop",
        "/go"
      ]
    },
    "stop": {
      "LED_pin": 15,
      "Button_pin": 35,
      "OSC_messages": [
        "/stop"
      ]
    },
    "pause": {
      "LED_pin": 13,
      "Button_pin": 34,
      "OSC_messages": [
        "/pause",
        "/playhead/previous"
      ]
    },
    "previous": {
      "LED_pin": 16,
      "Button_pin": 36,
      "OSC_messages": [
        "/stop",
        "/playhead/previous"
      ]
    },
    "next": {
      "LED_pin": 32,
      "Button_pin": 39,
      "OSC_messages": [
        "/playhead/next"
      ]
    }
  }
}
)=====";
