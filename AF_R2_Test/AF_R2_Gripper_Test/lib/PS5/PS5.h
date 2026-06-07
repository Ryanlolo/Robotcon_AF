#ifndef _PS5_H_
#define _PS5_H_

#include <mbed.h>
#include "CRC16.h"

class PS5{
    private:
        BufferedSerial* ser;
        uint8_t buffer[32];

       

        struct butts_mask{
            #define Right_butt_mask 0x80
            #define Left_butt_mask 0x40
            #define Up_butt_mask 0x20
            #define Down_butt_mask 0x10
            #define Square_butt_mask 0x08
            #define Cross_butt_mask 0x04
            #define Circle_butt_mask 0x02
            #define Trangle_butt_mask 0x01

            #define L1_butt_mask 0x80
            #define R1_butt_mask 0x40
            #define Share_butt_mask 0x20
            #define Options_butt_mask 0x10
            #define PsButton_butt_mask 0x08
            #define PsTouchpad_butt_mask 0x04
            #define L3_butt_mask 0x02
            #define R3_butt_mask 0x01
        };

        struct msgFormat{
            //format
            /////////////////////////////////////////////////////
            //     LS_x  LS_Y  RS_X    RS_Y     L2     R2    OP1   OP2
            //      1B   1B      1B     1B      1B      1B    1B    1B
            /////////////////////////////////////////////////////

            //OP1
            //Right  Left  Up  Down  Square  Cross  Circle  Triangle      
            // 1b     1b   1b   1b    1b      1b      1b       1b

            //OP2

            //L1  R1  Share  Options  PSButton  PSTouchpad  L3  R3  
            //1b  1b   1b      1b       1b         1b       1b  1b
            
            uint8_t L2;
            uint8_t R2;
            int8_t LS_X;
            int8_t LS_Y;
            uint8_t RS_X;
            uint8_t RS_Y;
            uint8_t OP1;
            uint8_t OP2;       
        };

        struct butts{
            bool Right = false;
            bool Left = false;
            bool Up = false;
            bool Down = false;
            bool Square = false;
            bool Cross = false;
            bool Circle = false;
            bool Triangle = false;
            bool L1 = false;
            bool R1 = false;
            bool Share = false;
            bool Options = false;
            bool PSButton = false;
            bool PSTouchpad = false;
            bool L3 = false;
            bool R3= false;
            int8_t Lx = 0;
            int8_t Ly = 0;
            int8_t Rx = 0;
            int8_t Ry = 0;
            uint8_t L2 = 0;
            uint8_t R2 = 0;
        };

        butts controller;
        const uint8_t start_byte = 0xAA; // 開始字節
        uint8_t* incomingMsg;

        uint8_t rx_idx = 0;
        bool frame_ready = false;

         // In the private section add:
        bool packet_valid = false;
        bool crc_enabled = false;

        int CRCverify();

    public:
        PS5();
        PS5(BufferedSerial* ser);

        

        void encode();
        void decode();
        void readData();

        bool isXpressed();          // Cross button
        bool isSquarePressed();     // Square button
        bool isCirclePressed();     // Circle button
        bool isTrianglePressed();   // Triangle button
        bool isLeftPressed();       // Left button
        bool isRightPressed();      // Right button
        bool isUpPressed();         // Up button
        bool isDownPressed();       // Down button
        bool isL1Pressed();         // L1 button
        bool isR1Pressed();         // R1 button
        bool isSharePressed();      // Share button
        bool isOptionsPressed();    // Options button
        bool isPSButtonPressed();    // PS Button
        bool isPSTouchpadPressed(); // PS Touchpad
        bool isL3Pressed();         // L3 button
        bool isR3Pressed();         // R3 button
    
        uint8_t getL2Value();       
        uint8_t getR2Value();              
        int8_t getLX();             
        int8_t getLY();             
        int8_t getRX();             
        int8_t getRY(); 

        // ========== ADD THESE TWO LINES ==========
        bool isPacketValid() const { return packet_valid; }
        const uint8_t* getRawBuffer() const { return buffer; }
        // =========================================
            
        
};

#endif