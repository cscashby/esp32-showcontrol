# esp32-showcontrol case

Case for show control based on ESP32-PoE-ISO board.

Dimensions: 50w x 135d x 54h
Case holes: 2mm in base (for self taps) 4mm top (clearance holes), countersunk
Protrusion from top cover to support / hold PCB secure, should rest on end of ESP32 module itself

Switches for go/stop (bottom pair) with 16mm diameter drill, but flats to 15mm at sides
Switches for prev/next (top pair) with 16mm diameter drill only
Adafruit 0.96" display with 2mm clearance / 2mm thick inset for 1-2mm thick clear perspex

Source for 3d models:
* Base: https://www.tinkercad.com/things/kWZmtJTIC6w-esp32-poe-gobutton
* Top
  * https://www.tinkercad.com/things/iRHtitwJItW-esp32-gobutton-top-bfb-diymore-version
    * This is the version for a DIYmore 0.95" 90x64 screen. If this doesn't have enough pixels the Adafruit 0.96" may be better.
    * NOTE when building this I realised that some of the screens I have have the display stuck in a slightly lower-than-marked position on the board. Unfortunately I did the design using one of these screens not the ones that are positioned correctly, so if you are building this, move the display around according to your ACTUAL screen before printing!
  * https://www.tinkercad.com/things/iRHtitwJItW-esp32-gobutton-top-bfb-adafruit-version
    * This is the version for an Adafruit 160x80 display with SD card. It's a bit more expensive, but has more pixels, which is nice.
  * Both of these are 'BFB' versions (Big ... Button) which I prefer as it's fairly obvious what to hit for 'go'...
  * I've left the old version here for posterity - note that the board standoff pole is NOT in the right place in this version, nor is the screen hole the right size (🤦): https://www.tinkercad.com/things/igH9JtvcPgK-esp32-gobutton-top-old-version
