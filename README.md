# CANTesting
Testing CAN communication with Teensy 3.6

Some things we've learned:
- Bit stuffing: for synchronization purposes, if there are 5 bits of the same value sent consecutively, then a bit of opposite value is inserted ("stuffed") after those 5 bits
	- See photos>bit_stuffing.PNG
- Connecting Teensy 3.6 to MCP 2562 Transceiver:
	- See photos>Transceiver_Teensy_Connections.PNG
	- do not forget to ground the STBY pin

Some resources we've found helpful:
- General CAN background: https://www.youtube.com/watch?v=3lkfK2-BUno (lower-level info starting around 11 minutes in)
- Orion BMS Wiring information: Solar Car Google Drive: Orion BMS Main IO Connector, Orion BMS Wiring Guide
- Orion BMS CAN information: Solar Car Google Drive: operational_manual.pdf, Orion BMS Software Utility
- MCP 2562 Datasheet: http://ww1.microchip.com/downloads/en/DeviceDoc/20005167C.pdf

Some tools we've used:
- Pawelsky FlexCAN library: https://github.com/pawelsky/FlexCAN_Library
- Teensyduino add on for Arduino: https://www.pjrc.com/teensy/teensyduino.html
- Software for Orion BMS: Orion BMS Utility