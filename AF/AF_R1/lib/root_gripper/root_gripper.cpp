#include "root_gripper.h"

root_gripper::root_gripper(DigitalOut* main_cylinder, DigitalOut* gripper_cylinder, DigitalOut* clip){
    _main_cylinder = main_cylinder;
    _gripper_cylinder = gripper_cylinder;
    _clip = clip;
} 

void root_gripper::stand_up(){
    _main_cylinder->write(1);
}

void root_gripper::lay_down(){
    _main_cylinder->write(0);   
}

void root_gripper::extend(){
    _gripper_cylinder->write(1);
}

void root_gripper::contract(){
    _gripper_cylinder->write(0);
}

void root_gripper::clip_open(){
    _clip->write(0);
}

void root_gripper::clip_close(){
    _clip->write(1);
}

// void root_gripper::setMode(int mode){
//     switch(mode){
//         case 1:
//             main_cylinder_up();
//             ThisThread::sleep_for(main_cylinder_sleep);
//             gripper_cylinder_up();
//             ThisThread::sleep_for(gripper_cylinder_sleep);
//             break;

//         case 2:
//             clip_close();
//             ThisThread::sleep_for(gripper_cylinder_sleep);
//             break;

//         case 3:
//             gripper_cylinder_down();
//             ThisThread::sleep_for(gripper_cylinder_sleep);
//             break;
//         case 4:
//             clip_open();
//             ThisThread::sleep_for(clip_sleep);
//             main_cylinder_down();
//             ThisThread::sleep_for(clip_sleep);
//             break;

//         default:
//             break;
//     }
// }