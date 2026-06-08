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
        void setTurnDeadzone(float deg);
        float getTurnDeadzone();

        void move(float x, float y, float w);
        void turnTo(float current_yaw, float target_yaw, float gain);

    private:
        vesc* fl;
        vesc* fr;
        vesc* bl;
        vesc* br;

        bool isTypeX = true;
        float max_speed = 10000.0f;
        float turn_deadzone = 5.0f;  // degrees - ignore yaw error smaller than this
};

#endif