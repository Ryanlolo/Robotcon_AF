#include "weapon_gripper.h"

weapon_gripper::weapon_gripper(vesc& extend_motor, DigitalOut& weapon_gripper, DigitalOut& gripper_cylinder,DigitalOut& electromagnet) : _extend_motor(extend_motor), _weapon_gripper(weapon_gripper), _gripper_cylinder(gripper_cylinder),_electromagnet(electromagnet) {
}

void weapon_gripper::extend(){
    _extend_motor.comm_can_set_pos(weapon_gripper_extend_position);
}

void weapon_gripper::contract(){
    _extend_motor.comm_can_set_pos(weapon_gripper_contract_position);
}

void weapon_gripper::gripper_open(){
    _weapon_gripper.write(0);
}

void weapon_gripper::gripper_close(){
    _weapon_gripper.write(1);
}

void weapon_gripper::gripper_cylinder_up(){
    _gripper_cylinder.write(1);
}

void weapon_gripper::gripper_cylinder_down(){
    _gripper_cylinder.write(0);
}

void weapon_gripper::electromagnet_on(){
    _electromagnet.write(1);
}

void weapon_gripper::electromagnet_off(){
    _electromagnet.write(0);
}
