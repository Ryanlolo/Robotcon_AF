#ifndef _ROOT_GRIPPER_H_
#define _ROOT_GRIPPER_H_

#include <mbed.h>

class root_gripper {
public:
    root_gripper(DigitalOut* main_cylinder, DigitalOut* gripper_cylinder, DigitalOut* clip);

    void stand_up();
    void lay_down();

    void extend();
    void contract();

    void clip_close();
    void clip_open();

    void setMode(int mode);

private:
    DigitalOut* _main_cylinder;
    DigitalOut* _gripper_cylinder;
    DigitalOut* _clip;

};
#endif