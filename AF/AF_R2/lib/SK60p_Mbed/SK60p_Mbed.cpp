/**
 * @file SK60p_Mbed.cpp
 * @brief Implementation of SK60p Laser Distance Sensor driver.
 */

#include "SK60p_Mbed.h"
#include <cstring>

SK60p_Mbed::SK60p_Mbed(PinName tx, PinName rx, int baud)
    : _serial(tx, rx, baud) {
    _serial.set_format(8, BufferedSerial::None, 1);
    memset(&_lastResult, 0, sizeof(_lastResult));
}

SK60p_Mbed::~SK60p_Mbed() {}

void SK60p_Mbed::sendCommand(uint8_t cmd) {
    uint8_t buf[4] = { FRAME_START, 0x01, cmd, 0x00 };
    buf[3] = (uint8_t)(buf[0] + buf[1] + buf[2]);
    _serial.write(buf, 4);
}

void SK60p_Mbed::startSingleMeasure() {
    flushBuffer();
    sendCommand(CMD_SINGLE);
}

void SK60p_Mbed::startContinuousAuto() {
    flushBuffer();
    sendCommand(CMD_AUTO);
}

void SK60p_Mbed::startContinuousSlow() {
    flushBuffer();
    sendCommand(CMD_SLOW_AUTO);
}

void SK60p_Mbed::startContinuousFast() {
    flushBuffer();
    sendCommand(CMD_FAST_AUTO);
}

void SK60p_Mbed::stopMeasure() {
    flushBuffer();
    sendCommand(CMD_STOP);
}

void SK60p_Mbed::flushBuffer() {
    uint8_t tmp;
    while (_serial.readable()) {
        _serial.read(&tmp, 1);
    }
}

uint8_t SK60p_Mbed::_readByte(uint32_t timeout_us) {
    uint8_t byte;
    int64_t start = us_ticker_read();
    while (true) {
        if (_serial.readable()) {
            _serial.read(&byte, 1);
            return byte;
        }
        if ((us_ticker_read() - start) >= timeout_us) {
            return FRAME_ERROR;
        }
    }
}

bool SK60p_Mbed::available() {
    return _serial.readable();
}

SK60p_Mbed::MeasureResult SK60p_Mbed::readFrame() {
    MeasureResult result;
    result.valid = false;
    result.startByte = _readByte(100000);
    if (result.startByte == FRAME_ERROR) {
        return result;
    }
    if (result.startByte == FRAME_ERROR) {
        result.valid = false;
        return result;
    }
    if (result.startByte != FRAME_START) {
        flushBuffer();
        result.valid = false;
        return result;
    }
    result.addr = _readByte();
    result.regAddr = (_readByte() << 8) | _readByte();
    result.validCalc = (_readByte() << 8) | _readByte();
    result.distance = ((uint32_t)_readByte() << 24) | ((uint32_t)_readByte() << 16) |
                     ((uint32_t)_readByte() << 8) | (uint32_t)_readByte();
    result.signalQuality = (_readByte() << 8) | _readByte();
    result.checksum = _readByte();

    result.valid = true;  // Trust frames starting with 0xAA

    result.status = (uint8_t)(result.validCalc & 0x0F);
    _lastResult = result;

    return result;
}
