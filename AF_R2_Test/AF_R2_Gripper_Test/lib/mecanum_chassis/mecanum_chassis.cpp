#include "mecanum_chassis.h"

mecanum_chassis::mecanum_chassis(vesc* fl, vesc* fr, vesc* bl, vesc* br, bool isTypeX){
    this->fl = fl;
    this->fr = fr;
    this->bl = bl;
    this->br = br;
    this->isTypeX = isTypeX;
}
        
void mecanum_chassis::setType(bool isTypeX){
    this->isTypeX = isTypeX;
}

bool mecanum_chassis::getTypeX(){
    // true -> type X; false -> type O
    return isTypeX;
}

void mecanum_chassis::setMaxSpeed(float speed){
    max_speed = speed;
}

float mecanum_chassis::getMaxSpeed(){
    return max_speed;
}

void mecanum_chassis::move(float x, float y, float w){
    float fl_speed = 0.0f;
    float fr_speed = 0.0f;
    float bl_speed = 0.0f;
    float br_speed = 0.0f;

    if(isTypeX){
        // tyope X
        fl_speed = y + x + w;
        fr_speed = y - x - w;
        bl_speed = y - x + w;
        br_speed = y + x - w;
    }
    else{
        //type O
        fl_speed = y - x - w;
        fr_speed = y + x + w;
        bl_speed = y + x - w;
        br_speed = y - x + w;
    }

    float speed_gain = max_speed / sqrtf(30000.0f);   // Map joystick range to actual RPM
    fl_speed *= speed_gain;
    fr_speed *= speed_gain;
    bl_speed *= speed_gain;
    br_speed *= speed_gain;

    if(fabs(fl_speed) > max_speed || fabs(fr_speed) > max_speed || fabs(bl_speed) > max_speed || fabs(br_speed) > max_speed){
        float list[4] = {fabs(fl_speed), fabs(fr_speed), fabs(bl_speed), fabs(br_speed)};
        float* max_value = max_element(list, list+4);
        fl_speed = fl_speed * max_speed / *max_value;
        fr_speed = fr_speed * max_speed / *max_value;
        bl_speed = bl_speed * max_speed / *max_value;
        br_speed = br_speed * max_speed / *max_value;
    }
    
    fl->comm_can_set_mrpm(fl_speed);
    fr->comm_can_set_mrpm(fr_speed);
    bl->comm_can_set_mrpm(bl_speed);
    br->comm_can_set_mrpm(br_speed);
}