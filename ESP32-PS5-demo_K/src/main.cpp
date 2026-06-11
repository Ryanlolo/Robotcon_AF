#include <Arduino.h>
#include <ps5Controller.h>

#define uart_2_tx 17  
#define uart_2_rx 16
#define Serial2_BAUD_RATE 115200
#define uart_1_baud_rate  115200  

#define Mac_address "50:EE:32:6D:6B:BA"

#define Start_byte 0xAA

void checkPS5ButtonInputs() {
    uint8_t dataPacket[9] = {0x00};
    dataPacket[0] = Start_byte;

    // PS5 button inputs
    dataPacket[1] = ps5.LStickX();
    dataPacket[2] = ps5.LStickY();
    dataPacket[3] = ps5.RStickX();
    dataPacket[4] = ps5.RStickY();
    dataPacket[5] = ps5.L2Value();
    dataPacket[6] = ps5.R2Value(); 

    dataPacket[7] = 0x00;
    dataPacket[7] |= (ps5.Right()    ? 0x80 : 0x00);
    dataPacket[7] |= (ps5.Left()     ? 0x40 : 0x00);
    dataPacket[7] |= (ps5.Up()       ? 0x20 : 0x00);
    dataPacket[7] |= (ps5.Down()     ? 0x10 : 0x00);
    dataPacket[7] |= (ps5.Square()   ? 0x08 : 0x00);
    dataPacket[7] |= (ps5.Cross()    ? 0x04 : 0x00);
    dataPacket[7] |= (ps5.Circle()   ? 0x02 : 0x00);
    dataPacket[7] |= (ps5.Triangle() ? 0x01 : 0x00);

    dataPacket[8] = 0x00;
    dataPacket[8] |= (ps5.L1()        ? 0x80 : 0x00);
    dataPacket[8] |= (ps5.R1()        ? 0x40 : 0x00);
    dataPacket[8] |= (ps5.Share()     ? 0x20 : 0x00);
    dataPacket[8] |= (ps5.Options()   ? 0x10 : 0x00);
    dataPacket[8] |= (ps5.PSButton()  ? 0x08 : 0x00);
    dataPacket[8] |= (ps5.Touchpad()  ? 0x04 : 0x00);
    dataPacket[8] |= (ps5.L3()        ? 0x02 : 0x00);
    dataPacket[8] |= (ps5.R3()        ? 0x01 : 0x00);

    // Serial.printf("%2X %2X %2X %2X %2X %2X %2X %2X %2X\n",
    //                 dataPacket[0],
    //                 dataPacket[1],
    //                 dataPacket[2],
    //                 dataPacket[3],
    //                 dataPacket[4],
    //                 dataPacket[5],
    //                 dataPacket[6], 
    //                 dataPacket[7],
    //                 dataPacket[8]);
    
    Serial2.write(dataPacket, sizeof(dataPacket));
    // Serial2.flush();
    // delay(10);  // Add a small delay to avoid overwhelming the serial buffer
}

void setup() {
    Serial.begin(uart_1_baud_rate);

    if(!ps5.begin(Mac_address)){
      Serial.println("Connected");
    }
    else{
      Serial.println("Connect Failed");
    }

    Serial2.begin(Serial2_BAUD_RATE, SERIAL_8N1, uart_2_rx, uart_2_tx);  
}

void loop() {
  if (ps5.isConnected()) {
      checkPS5ButtonInputs();
  } else {
      Serial.println("ps5 controller not connected");
      delay(500);
  }
}