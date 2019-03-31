// demo: CAN-BUS Shield, send data
#include <SPI.h>
#include <mcp_can.h>

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;
const int ledHIGH = 1;
const int ledLOW = 0;

MCP_CAN CAN(SPI_CS_PIN);  // Set CS pin

void setup() {
    Serial.begin(115200);

    while (CAN_OK != CAN.begin(CAN_250KBPS))  // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println(" Init CAN BUS Shield again");
        delay(100);
    }
    Serial.println("CAN BUS Shield init ok!");
}

unsigned char stmp[8] = {ledHIGH, 1, 2, 3, ledLOW, 5, 6, 7};

void loop() {
    unsigned char len = 0;
    unsigned char buf[8];

    // Serial.println("In loop");
    // send data:  id = 0x70, standard frame, data len = 8, stmp: data buf
    if (CAN_MSGAVAIL == CAN.checkReceive()) {
        CAN.readMsgBuf(&len, buf);  // read data,  len: data length, buf: data buf

        unsigned long canId = CAN.getCanId();

        Serial.println("-----------------------------");
        Serial.print("Get data from ID: 0x");
        Serial.println(canId, HEX);

        for (int i = 0; i < len; i++)  // print the data
        {
            Serial.print(buf[i], HEX);
            Serial.print("\t");
        }
        Serial.println();
    }

    // CAN.sendMsgBuf(0x70, 1, 8, stmp);

    delay(200);  // send data once per second
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
