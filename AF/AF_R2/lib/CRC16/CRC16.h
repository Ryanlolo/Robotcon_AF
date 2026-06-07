/*
 * This is a fork of the CRC16 library COPYRIGHT(c) Emilie Laverge
 * published at [https://developer.mbed.org/users/EmLa/code/CRC16/]
 * using the polynomial 0x8005: X^16 + X^15 + X^2 + 1.
 * Default initial CRC value = 0x0000
 *
 * Modified by Zoltan Hudak
 */

#ifndef _CRC16_H_
#define _CRC16_H_
#include "mbed.h"

class   CRC16
{
private:
    static const unsigned int   SHIFTER;
    static const unsigned short TABLE[];
public:
    CRC16(void){};
    ~CRC16(void){};
    uint16_t calc(uint8_t* input, int length, uint16_t crc = 0x0000);
};
#endif // _CRC16_H_
