#ifndef _MECANUM_CHASSIS_H_
#define _MECANUM_CHASSIS_H_

#include <mbed.h>
#include "vesc.h"

class mecanum_chassis{
    public:
        mecanum_chassis(vesc* fl, vesc* fr, vesc* bl, vesc* br, bool isTypeX);
        
        void setType(bool isTypeX);
        bool getTypeX();
        void setMaxSpeed(float speed);
        float getMaxSpeed();

        void move(float x, float y, float w);

    private:
        vesc* fl;
        vesc* fr;
        vesc* bl;
        vesc* br;

        bool isTypeX = true;
        float max_speed = 10000.0f;
};

#endif