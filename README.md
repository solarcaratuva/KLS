# CAN
Code for interfacing over CAN using Teensy 3.6 for the UVA Solar Car project.

## Contents of this repository
* "kls.h" contains the class definition for KLS
* **WIP**


## KLS Motor Controller Library
* Written to interface with [Kelly Controls KLS 8080I family of motor controllers](https://kellyev.com/shop/kls-8080i-ips/) over CAN
* Tested with KLS 12201-8080I 
* Class written based on [manufactuer specifications for the motor controller's CAN protocol](https://kellyev.com/wp-content/uploads/kls-8080i-ips/Sinusoidal-Wave-Controller-KLS-D-8080I-8080IPS-Broadcast-CAN-Protocol.pdf)
* Requires the [fork of the FlexCAN library by pawelsky](https://github.com/pawelsky/FlexCAN_Library)
* **WIP** Library documentation


## Miscellaneous Notes
Some things we've learned:
* Bit stuffing: for synchronization purposes, if there are 5 bits of the same value sent consecutively, then a bit of opposite value is inserted ("stuffed") after those 5 bits
  * See photos>bit_stuffing.PNG
* Connecting Teensy 3.6 to MCP 2562 Transceiver:
  * See photos>Transceiver_Teensy_Connections.PNG
  * do not forget to ground the STBY pin

Some resources we've found helpful:
* [General CAN background](https://www.youtube.com/watch?v=3lkfK2-BUno) (lower-level info starting around 11 minutes in)
* Orion BMS Wiring information: Solar Car Google Drive: Orion BMS Main IO Connector, Orion BMS Wiring Guide
* Orion BMS CAN information: Solar Car Google Drive: operational_manual.pdf, Orion BMS Software Utility
* [MCP 2562 Datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/20005167C.pdf)
* Software for Orion BMS: Orion BMS Utility
