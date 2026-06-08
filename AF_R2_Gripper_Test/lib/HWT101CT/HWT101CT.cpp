/**
 * @file HWT101CT.cpp
 * @brief Implementation of HWT101CT gyroscope driver.
 */

#include "HWT101CT.h"
#include <cmath>

HWT101CT::HWT101CT(BufferedSerial* _serial) : serial(_serial) {}

//--------------------------------------------------------------
// Private helpers
//--------------------------------------------------------------

void HWT101CT::sendCommand(uint8_t addr, uint8_t reg, int16_t data) {
    uint8_t cmd[5] = {
        FRAME_HEAD,
        addr,
        reg,
        static_cast<uint8_t>(data & 0xFF),
        static_cast<uint8_t>((data >> 8) & 0xFF)
    };
    serial->write(cmd, 5);
}

int16_t HWT101CT::receiveResponse() {
    uint8_t buf[11];
    for (int retry = 0; retry < 10; retry++) {
        int len = serial->read(buf, 11);
        if (len > 0) {
            int i = 0;
            while (i < len) {
                if (buf[i] == 0x55) {
                    int available = len - i;
                    if (available >= 11) {
                        if (buf[i+10] == 0) {
                            return (int16_t)((uint16_t)buf[i+3] | ((uint16_t)buf[i+4] << 8));
                        }
                        i += 10;
                    } else {
                        break;
                    }
                } else {
                    i++;
                }
            }
            if (serial->readable()) {
                continue;
            }
        }
        ThisThread::sleep_for(1ms);
    }
    return 0;
}

int16_t HWT101CT::readRegister(uint8_t reg, bool ToNegative) {
    sendCommand(FRAME_ADDR, reg, 0);
    ThisThread::sleep_for(2ms);
    int16_t value = receiveResponse();
    return ToNegative ? -value : value;
}

bool HWT101CT::writeRegister(uint8_t reg, int16_t data, bool ToNegative) {
    int16_t writeData = ToNegative ? -data : data;
    sendCommand(FRAME_ADDR, reg, writeData);
    ThisThread::sleep_for(2ms);
    int16_t response = receiveResponse();
    return (response == writeData);
}

//--------------------------------------------------------------
// Read functions
//--------------------------------------------------------------

float HWT101CT::Get_Yaw(bool ToNegative) {
    int16_t raw = readRegister(REG_YAW, ToNegative);
    return raw / 100.0f;
}

float HWT101CT::Get_GY(bool ToNegative) {
    int16_t raw = readRegister(REG_GY, ToNegative);
    return raw / 100.0f;
}

float HWT101CT::Get_GZ(bool ToNegative) {
    int16_t raw = readRegister(REG_GZ, ToNegative);
    return raw / 100.0f;
}

float HWT101CT::Get_FirmwareVersion(bool ToNegative) {
    int16_t raw = readRegister(REG_VERSION, ToNegative);
    return raw / 100.0f;
}

float HWT101CT::Get_ZeroBias(bool ToNegative) {
    int16_t raw = readRegister(REG_ZERO_BIAS, ToNegative);
    return raw / 100.0f;
}

float HWT101CT::Get_PeakToPeakNoise(bool ToNegative) {
    int16_t raw = readRegister(REG_PEAK_TO_PEAK, ToNegative);
    return raw / 100.0f;
}

//--------------------------------------------------------------
// Write functions
//--------------------------------------------------------------

bool HWT101CT::Set_OutputRate(int rate, bool ToNegative) {
    return writeRegister(REG_OUTPUT_RATE, rate, ToNegative);
}

bool HWT101CT::Set_BaudRate(int baud, bool ToNegative) {
    return writeRegister(REG_BAUD_RATE, baud, ToNegative);
}

bool HWT101CT::Set_WorkMode(int mode, bool ToNegative) {
    return writeRegister(REG_WORK_MODE, mode, ToNegative);
}
