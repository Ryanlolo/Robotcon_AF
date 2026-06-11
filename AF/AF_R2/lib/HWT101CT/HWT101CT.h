/**
 * @file HWT101CT.h
 * @brief Driver for HWT101CT gyroscope module via UART communication.
 *
 * Hardware: HWT101CT Z-axis gyroscope
 * Communication: UART @ 115200 baud
 * Protocol: 5-byte command / 11-byte response frames
 * Board: STM32 NUCLEO-F446RE (Mbed framework)
 *
 * Author: ChatGPT
 */

#ifndef HWT101CT_H
#define HWT101CT_H

#include <mbed.h>

#define IMU_BAUD 115200

class HWT101CT {
public:
    HWT101CT(BufferedSerial* _serial);

    float Get_Yaw(bool ToNegative = false);
    float Get_GY(bool ToNegative = false);
    float Get_GZ(bool ToNegative = false);

    float Get_FirmwareVersion(bool ToNegative = false);

    float Get_ZeroBias(bool ToNegative = false);
    float Get_PeakToPeakNoise(bool ToNegative = false);

    bool Set_OutputRate(int rate, bool ToNegative = false);
    bool Set_BaudRate(int baud, bool ToNegative = false);
    bool Set_WorkMode(int mode, bool ToNegative = false);

private:
    BufferedSerial* serial;

    static constexpr uint8_t FRAME_HEAD = 0xFF;
    static constexpr uint8_t FRAME_ADDR = 0xAA;

    static constexpr uint8_t REG_YAW     = 0x01;
    static constexpr uint8_t REG_GY      = 0x02;
    static constexpr uint8_t REG_GZ      = 0x03;
    static constexpr uint8_t REG_VERSION = 0x03;

    static constexpr uint8_t REG_ZERO_BIAS      = 0x04;
    static constexpr uint8_t REG_PEAK_TO_PEAK   = 0x05;

    static constexpr uint8_t REG_OUTPUT_RATE    = 0x09;
    static constexpr uint8_t REG_BAUD_RATE      = 0x0A;
    static constexpr uint8_t REG_WORK_MODE     = 0x0B;

    void sendCommand(uint8_t addr, uint8_t reg, int16_t data);
    int16_t receiveResponse();
    int16_t readRegister(uint8_t reg, bool ToNegative);
    bool writeRegister(uint8_t reg, int16_t data, bool ToNegative);
};

#endif
