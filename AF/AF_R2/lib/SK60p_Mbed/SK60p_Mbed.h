/**
 * @file SK60p_Mbed.h
 * @brief Driver for SK60p Laser Distance Sensor via UART.
 *
 * Hardware: SK60p Laser Ranging Module
 * Communication: UART @ 19200 baud
 * Board: STM32 NUCLEO-F446RE (Mbed framework)
 *
 * Author: ChatGPT
 */

#ifndef SK60P_MBED_H
#define SK60P_MBED_H

#include <mbed.h>

#define LASER_BAUD 19200

class SK60p_Mbed {
public:
    struct MeasureResult {
        bool valid;
        uint8_t startByte;
        uint8_t addr;
        uint16_t regAddr;
        uint16_t validCalc;
        uint32_t distance;
        uint16_t signalQuality;
        uint8_t checksum;
        uint8_t status;
    };

    SK60p_Mbed(PinName tx, PinName rx, int baud = LASER_BAUD);
    ~SK60p_Mbed();

    void startSingleMeasure();
    void startContinuousAuto();
    void startContinuousSlow();
    void startContinuousFast();
    void stopMeasure();

    bool available();
    MeasureResult readFrame();

    enum Status {
        STATUS_OK                     = 0x00,
        STATUS_LOW_VOLTAGE            = 0x01,
        STATUS_HIGH_TEMP              = 0x02,
        STATUS_LOW_TEMP               = 0x03,
        STATUS_MEAS_FAIL              = 0x04,
        STATUS_OUT_OF_RANGE           = 0x05,
        STATUS_INVALID_RESULT         = 0x06,
        STATUS_BACKGROUND_LIGHT       = 0x07,
        STATUS_LASER_signal_FAIL      = 0x08,
        STATUS_HARDWARE_FAULT         = 0x09
    };

private:
    BufferedSerial _serial;

    static constexpr uint8_t FRAME_START  = 0xAA;
    static constexpr uint8_t FRAME_ERROR  = 0xFF;
    static constexpr int FRAME_LEN = 13;

    static constexpr uint8_t CMD_SINGLE        = 0x04;
    static constexpr uint8_t CMD_AUTO           = 0x06;
    static constexpr uint8_t CMD_SLOW_AUTO      = 0x05;
    static constexpr uint8_t CMD_FAST_AUTO       = 0x07;
    static constexpr uint8_t CMD_STOP           = 0x02;

    uint8_t _readByte(uint32_t timeout_us = 100000);
    void flushBuffer();
    void sendCommand(uint8_t cmd);

    MeasureResult _lastResult;
};

#endif
