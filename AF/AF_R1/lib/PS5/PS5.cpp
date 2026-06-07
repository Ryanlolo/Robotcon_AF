#include "mbed.h"
#include "PS5.h"

PS5::PS5(){}

PS5::PS5(BufferedSerial* ser){
    this->ser = ser;
}

void PS5::encode(){

}

void PS5::decode(){
    packet_valid = false;   // Assume invalid until proven otherwise
    readData();

    if (!frame_ready) {
        return;
    }

    this->incomingMsg = buffer;
    
    // ---- Start byte check (unchanged) ----
    if(incomingMsg[0] != start_byte){
        return;
    }

    // Check start byte
    if (buffer[0] != start_byte) {
        // Optionally flush the serial buffer if we're out of sync
        return;
    }

    // Optional CRC check (if you have a working CRC implementation)
    //if (crc_enabled) {
        // uint16_t crc_calc = CRC16::calc(buffer, 8, 0x0000);
        // uint16_t crc_recv = (buffer[9] << 8) | buffer[10];
        // if (crc_calc != crc_recv) return;
    //}

    packet_valid = true; // <-- ADDED: packet passed basic validation


    

    

    // if(this->CRCverify() != 0){
    //     return;
    // }

    //clear
    controller = PS5::butts();

    uint8_t op1 = incomingMsg[7];
    uint8_t op2 = incomingMsg[8];

    controller.Lx = (int8_t)incomingMsg[1];
    controller.Ly = (int8_t)incomingMsg[2];
    controller.Rx = (int8_t)incomingMsg[3];
    controller.Ry = (int8_t)incomingMsg[4];
    controller.L2 = (uint8_t)incomingMsg[5];
    controller.R2 = (uint8_t)incomingMsg[6];

    controller.Right = ((op1 & Right_butt_mask) != 0x00 ? true : false);
    controller.Left = ((op1 & Left_butt_mask) != 0x00 ? true : false);
    controller.Up = ((op1 & Up_butt_mask) != 0x00 ? true : false);          
    controller.Down = ((op1 & Down_butt_mask) != 0x00 ? true : false);      
    controller.Square = ((op1 & Square_butt_mask) != 0x00 ? true : false);  
    controller.Cross = ((op1 & Cross_butt_mask) != 0x00 ? true : false);    
    controller.Circle = ((op1 & Circle_butt_mask) != 0x00 ? true : false);  
    controller.Triangle = ((op1 & Trangle_butt_mask) != 0x00 ? true : false); 

    controller.L1 = ((op2 & L1_butt_mask) != 0x00 ? true : false);
    controller.R1 = ((op2 & R1_butt_mask) != 0x00 ? true : false);
    controller.Share = ((op2 & Share_butt_mask) != 0x00 ? true : false);          
    controller.Options = ((op2 & Options_butt_mask) != 0x00 ? true : false);      
    controller.PSButton = ((op2 & PsButton_butt_mask) != 0x00 ? true : false);  
    controller.PSTouchpad= ((op2 & PsTouchpad_butt_mask) != 0x00 ? true : false);    
    controller.L3 = ((op2 & L3_butt_mask) != 0x00 ? true : false);  
    controller.R3 = ((op2 & R3_butt_mask) != 0x00 ? true : false); 
}

// int PS5::CRCverify(){
//     int result = 0;
    
//     return result;
// }


bool PS5::isXpressed() {
    return controller.Cross; 
}

bool PS5::isSquarePressed() {
    return controller.Square; 
}

bool PS5::isCirclePressed() {
    return controller.Circle; 
}

bool PS5::isTrianglePressed() {
    return controller.Triangle; 
}

bool PS5::isLeftPressed() {
    return controller.Left; 
}

bool PS5::isRightPressed() {
    return controller.Right; 
}

bool PS5::isUpPressed() {
    return controller.Up; 
}

bool PS5::isDownPressed() {
    return controller.Down; 
}

bool PS5::isL1Pressed() {
    return controller.L1; 
}

bool PS5::isR1Pressed() {
    return controller.R1; 
}

bool PS5::isSharePressed() {
    return controller.Share; 
}

bool PS5::isOptionsPressed() {
    return controller.Options; 
}

bool PS5::isPSButtonPressed() {
    return controller.PSButton; 
}

bool PS5::isPSTouchpadPressed() {
    return controller.PSTouchpad; 
}

bool PS5::isL3Pressed() {
    return controller.L3; 
}

bool PS5::isR3Pressed() {
    return controller.R3; 
}

uint8_t PS5::getL2Value() {
    return controller.L2; 
}

uint8_t PS5::getR2Value() {
    return controller.R2; 
}

int8_t PS5::getLX() {
    return controller.Lx; 
}

int8_t PS5::getLY() {
    return controller.Ly; 
}

int8_t PS5::getRX() {
    return controller.Rx; 
}

int8_t PS5::getRY() {
    return controller.Ry; 
}
void PS5::readData(){
    frame_ready = false;
    while (ser->readable()) {
        uint8_t b;
        if (ser->read(&b, 1) < 1) {
            break;
        }
        if (rx_idx == 0 && b != start_byte) {
            continue;
        }
        buffer[rx_idx++] = b;
        if (rx_idx >= sizeof(buffer)) {
            frame_ready = true;
            rx_idx = 0;
        }
    }
}