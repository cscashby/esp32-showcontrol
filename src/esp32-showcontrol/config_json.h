const char CONFIG_JSON[] PROGMEM = R"=====(
{
  "network": {
    "hostname": "esp32-showcontrol",
    "osc_hosts": {
      "main": {
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
      "heartbeat_us": 1000000,
      "poll_us": 100000
    }
  },
  "buttons": {
    "go": {
      "LED_pin": 0,
      "Button_pin": 33,
      "OSC_messages": [
        "/stop",
        "/go"
      ],
      "OSC_subscribe": {
        "string": "/reply/go"
      }
    },
    "stop": {
      "LED_pin": 15,
      "Button_pin": 35,
      "OSC_messages": [
        "/stop"
      ],
      "OSC_subscribe": {
        "string": "/reply/stop"
      }
    },
    "pause": {
      "LED_pin": 13,
      "Button_pin": 34,
      "OSC_messages": [
        "/pause",
        "/playhead/previous"
      ],
      "OSC_subscribe": {
        "string": "/reply/pause"
      }
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
