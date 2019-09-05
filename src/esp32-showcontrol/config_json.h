const char CONFIG_JSON[] PROGMEM = R"=====(
{
  "buttons": {
    "go": {
      "LED_pin": 0,
      "Button_pin": 33,
      "OSC_commands": [
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
      "OSC_commands": [
        "/stop"
      ],
      "OSC_subscribe": {
        "string": "/reply/stop"
      }
    },
    "pause": {
      "LED_pin": 13,
      "Button_pin": 34,
      "OSC_commands": [
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
      "OSC_commands": [
        "/stop",
        "/playhead/previous"
      ]
    },
    "next": {
      "LED_pin": 32,
      "Button_pin": 39,
      "OSC_commands": [
        "/playhead/next"
      ]
    }
  }
}
)=====";
