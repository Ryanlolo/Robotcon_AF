#include "root_gripper.h"

root_gripper::root_gripper(DigitalOut& weapon_cylinder1, DigitalOut& weapon_cylinder2, DigitalOut& kfs_gripper, DigitalOut& clip, vesc& lifter) :
    _weapon_cylinder1(weapon_cylinder1),
    _weapon_cylinder2(weapon_cylinder2),
    _kfs_gripper(kfs_gripper),
    _clip(clip),
    _lifter(lifter),
    _lifter_speed(1000.0f),
    _lifter_brake_current(7.0f) {}

void root_gripper::stand_up(){
   _weapon_cylinder1.write(1);
}

void root_gripper::lay_down(){
    _weapon_cylinder1.write(0);   
}

void root_gripper::extend(){
    _weapon_cylinder2.write(0);
}

void root_gripper::contract(){
    _weapon_cylinder2.write(1);
}

void root_gripper::clip_open(){
    _clip.write(0);
}

void root_gripper::clip_close(){
    _clip.write(1);
}

void root_gripper::kfs_gripper_open(){
    _kfs_gripper.write(0);
}

void root_gripper::kfs_gripper_close(){
    _kfs_gripper.write(1);
}

void root_gripper::setLifterSpeed(float mrpm){
    _lifter_speed = mrpm;
}

float root_gripper::getLifterSpeed(){
    return _lifter_speed;
}

void root_gripper::setLifterBrakeCurrent(float current){
    _lifter_brake_current = current;
}

void root_gripper::lifter_update(bool up_pressed, bool down_pressed){
    if(up_pressed && !down_pressed){
        _lifter.comm_can_set_mrpm(_lifter_speed);
    }
    else if(down_pressed && !up_pressed){
        _lifter.comm_can_set_mrpm(-_lifter_speed);
    }
    else{
        _lifter.comm_can_set_current_brake(_lifter_brake_current);
    }
}
