# esp32-showcontrol
Showcontrol with an ESP32.

<img src="https://github.com/cscashby/esp32-showcontrol/raw/master/case-front.jpg" alt="Control box front"/>

The box is PoE powered (if you use a suitable board, I highly recommend the [ESP32-PoE-ISO](https://www.olimex.com/Products/IoT/ESP32/ESP32-POE-ISO/open-source-hardware) which has adequate isolation for the network port, or can be powered using a Micro USB connector and will support WiFi (though this is not recommended for production use for obvious reasons, and hasn't yet been implemented).

The current design allows for Go, Stop, Pause, Next and Previous buttons, and a Jack terminated footswitch (likely mapped to Go, obviously).

It communicates via one ore more QLab machines via OSC over UDP and (not yet implemented) will have a web interface to configure QLab machine IP address, button mapping, display layout (maybe!).

The control box has a small display (0.94-0.96" with 3-4 lines of text) and will display currently playing and next cue, as well as a 'heartbeat' indicator to show QLab is running and accepting network traffic.

The box wiring is currently soldered wires directly onto pushbuttons and LEDs, and soldered onto the OLED board.  [Pinouts are documented in a separate file](https://github.com/cscashby/esp32-showcontrol/blob/master/esp32-poe-iso-connections.md).

NOTE the current codebase for all the features sort of working is in the single esp32-showcontrol.ino file.  This is being modularised and written in a much better way in the src/ folder, so check that out for the hopefully more decent code.

The good, the bad and the ugly of the creation process are [documented in this photo album](https://photos.app.goo.gl/oM8NKtMR5yHytawS9).
