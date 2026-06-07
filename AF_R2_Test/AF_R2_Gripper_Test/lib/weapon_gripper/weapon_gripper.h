#ifndef _WEAPON_GRIPPER_H_
#define _WEAPON_GRIPPER_H_

#include <mbed.h>
#include "vesc.h"
#include "../setting.h"

class weapon_gripper {
    public:
        weapon_gripper(vesc& extend_motor, DigitalOut& weapon_gripper, DigitalOut& electromagnet);

        void extend();
        void contract();

        void gripper_open();
        void gripper_close();

        void electromagnet_on();
        void electromagnet_off();

    private:
        vesc& _extend_motor;
        DigitalOut& _weapon_gripper;
        DigitalOut& _electromagnet;

};
#endif