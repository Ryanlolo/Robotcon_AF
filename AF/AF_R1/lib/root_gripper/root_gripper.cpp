#include "root_gripper.h"

root_gripper::root_gripper(DigitalOut& weapon_cylinder1, DigitalOut& weapon_cylinder2, DigitalOut& kfs_gripper, DigitalOut& clip) :
 _weapon_cylinder1(weapon_cylinder1), _weapon_cylinder2(weapon_cylinder2), _kfs_gripper(kfs_gripper), _clip(clip) {}

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