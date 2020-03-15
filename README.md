# esp32-showcontrol
Showcontrol with an ESP32.

<img src="https://github.com/cscashby/esp32-showcontrol/raw/master/case-front.jpg" alt="Control box front"/>

## Overview

The box is PoE powered (if you use a suitable board, I highly recommend the [ESP32-PoE-ISO](https://www.olimex.com/Products/IoT/ESP32/ESP32-POE-ISO/open-source-hardware) which has adequate isolation for the network port, or can be powered using a Micro USB connector and will support WiFi (though this is not recommended for production use for obvious reasons, and hasn't yet been implemented).

The current design allows for Go, Stop, Pause, Next and Previous buttons, and a Jack terminated footswitch (likely mapped to Go, obviously).

It communicates via one ore more QLab machines via OSC over UDP and (not yet implemented) will have a web interface to configure QLab machine IP address, button mapping, display layout (maybe!).

The control box has a display (The first iteration has a 0.94-0.96" screen which can display 3-4 lines of text) and will show the currently playing and next cue, although this is customizable, as well as a 'heartbeat' indicator to show QLab is running and accepting network traffic.

The good, the bad and the ugly of the creation process are [documented in this photo album](https://photos.app.goo.gl/oM8NKtMR5yHytawS9).

## Wiring

The box wiring is currently soldered wires directly onto pushbuttons and LEDs, and soldered onto the OLED board.  [Pinouts are documented in a separate file for the ESP32-POE-ISO board](https://github.com/cscashby/esp32-showcontrol/blob/master/esp32-poe-iso-connections.md).

## Development

### IDE Setup

To set up the [Arduino IDE](https://www.arduino.cc/en/main/software) to develop on the [ESP32-PoE-ISO](https://www.olimex.com/Products/IoT/ESP32/ESP32-POE-ISO/open-source-hardware) board, you will need the drivers from the manufacturer's website.

You will also need the ESP32 default board set up through Board Manager within the Arduino IDE, [using these instructions](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md).
