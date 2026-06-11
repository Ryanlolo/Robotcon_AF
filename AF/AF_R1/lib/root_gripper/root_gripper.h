#ifndef _ROOT_GRIPPER_H_
#define _ROOT_GRIPPER_H_

#include <mbed.h>
#include "vesc.h"

class root_gripper {
public:
    root_gripper(DigitalOut& weapon_cylinder1, DigitalOut& weapon_cylinder2, DigitalOut& kfs_gripper, DigitalOut& clip, vesc& lifter);

    void stand_up();
    void lay_down();

    void extend();
    void contract();

    void clip_close();
    void clip_open();

    void kfs_gripper_open();
    void kfs_gripper_close();

    void setLifterSpeed(float mrpm);
    float getLifterSpeed();
    void setLifterBrakeCurrent(float current);
    void lifter_update(bool up_pressed, bool down_pressed);

private:
    DigitalOut& _weapon_cylinder1;
    DigitalOut& _weapon_cylinder2;
    DigitalOut& _kfs_gripper;
    DigitalOut& _clip;
    vesc& _lifter;

    float _lifter_speed;
    float _lifter_brake_current;
};
#endif
