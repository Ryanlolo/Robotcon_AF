#include <mbed.h>

#include "setting.h"
#include "PS5.h"
#include "vesc.h"
#include "mecanum_chassis.h"
#include "weapon_gripper.h"

int mode = 0;   // 0 -> manual mode; 1 -> auto mode

#if EN_PS5
BufferedSerial ps5(ps5_tx, ps5_rx, ps5_baud);
PS5 ps5Controller(&ps5);
#endif

CAN _can3(can_rx, can_tx, can_baud);

//chassis
vesc motor_fl(fl_id, &_can3, can_baud, Motor_type::m3508);
vesc motor_fr(fr_id, &_can3, can_baud, Motor_type::m3508);
vesc motor_bl(bl_id, &_can3, can_baud, Motor_type::m3508);
vesc motor_br(br_id, &_can3, can_baud, Motor_type::m3508);
mecanum_chassis chassis(&motor_fl, &motor_fr, &motor_bl, &motor_br, true);

//chassis cylinders
#if EN_chassis_cylinders
DigitalOut front_chassis_cylinder(front_chassis_cylinder_pin);
DigitalOut back_chassis_cylinder(back_chassis_cylinder_pin);
#endif

//weapon gripper
#if EN_weapon_gripper
DigitalOut gripper(weapon_gripper_pin);
DigitalOut electromagnet(electromagnet_pin);
vesc extend_motor(extend_motor_id, &_can3, can_baud, Motor_type::m3508);
weapon_gripper weapon_gripper_1(&extend_motor, &gripper, &electromagnet);
#endif

int8_t x = 0, y = 0, w = 0;

#if EN_PS5
Thread ps5_thread(osPriorityHigh2);
#endif
Thread chassis_thread(osPriorityHigh1);
#if EN_chassis_cylinders
Thread chassis_cylinder_thread(osPriorityHigh1);
#endif
#if EN_weapon_gripper
Thread weapon_gripper_thread(osPriorityHigh1);
#endif

#if EN_PS5
int8_t deadzone_filter(int8_t value, int8_t deadzone_value){
    if(abs(value) < deadzone_value){
        return 0;
    }

    return value;
}

void ps5_communication(){

    while(1){
        ps5Controller.decode();

        if (ps5Controller.isPacketValid()) {
            // ---- Get raw values ----
            int8_t raw_lx = ps5Controller.getLX();
            int8_t raw_ly = ps5Controller.getLY();
            int8_t raw_rx = ps5Controller.getRX();
            int8_t raw_ry = ps5Controller.getRY();

            x = deadzone_filter(raw_lx, PS5_deadzone);
            y = deadzone_filter(raw_ly, PS5_deadzone);
            w = deadzone_filter(raw_rx, PS5_deadzone);

            // Optional debug output
            //printf("Raw: LX=%d LY=%d | Filtered: X=%d Y=%d W=%d\n", raw_lx, raw_ly, x, y, w);
        }
        else{
            x = 0;
            y = 0;
            w = 0;
        } 

        ThisThread::sleep_for(1000ms/PS5_freq);
    }
}
#endif

void chassis_control(){
    chassis.setMaxSpeed(10000.0f);

    while(1){
        if(mode == 1){
            //auto mode
            
        }
        else{
            // manual mode
            chassis.move(x, y, w);
        }
        
        ThisThread::sleep_for(1000ms/chassis_freq);
    }
}

#if EN_chassis_cylinders
void chassis_cylinder_control(){
    // default contract
    front_chassis_cylinder.write(0);
    back_chassis_cylinder.write(0);

    bool up = ps5Controller.isUpPressed();
    bool down = ps5Controller.isDownPressed();

    bool front_cylinder_flag = false;
    bool back_cylinder_flag = false;

    bool prev_up = false;
    bool prev_down = false;

    while(1){
        up = ps5Controller.isUpPressed();
        down = ps5Controller.isDownPressed();

        if(up && !prev_up){
            if(front_cylinder_flag){
                front_chassis_cylinder.write(1);
            }
            else{
                front_chassis_cylinder.write(0);
            }

            front_cylinder_flag = !front_cylinder_flag;
        }

        if(down && !prev_down){
            if(back_cylinder_flag){
                back_chassis_cylinder.write(1);
            }
            else{
                back_chassis_cylinder.write(0);
            }

            back_cylinder_flag = !back_cylinder_flag;
        }

        prev_up = up;
        prev_down = down;

        ThisThread::sleep_for(1000ms/chassis_cylinder_freq);
    }
}
#endif

#if EN_weapon_gripper
void weapon_gripper_control(){
    //default
    weapon_gripper_1.contract();
    weapon_gripper_1.gripper_open();
    weapon_gripper_1.electromagnet_open();

    bool triangle = ps5Controller.isTrianglePressed();
    bool square = ps5Controller.isSquarePressed();
    bool cross = ps5Controller.isXpressed();

    bool extend_motor_flag = false;
    bool gripper_flag = false;
    bool electromagnet_flag = false;

    bool prev_triangle = false;
    bool prev_square = false;
    bool prev_cross = false;

    while(1){
        triangle = ps5Controller.isTrianglePressed();
        square = ps5Controller.isSquarePressed();
        cross = ps5Controller.isXpressed();

        if(triangle && !prev_triangle){
            if(extend_motor_flag){
                weapon_gripper_1.extend();
            }
            else{
                weapon_gripper_1.contract();
            }

            extend_motor_flag = !extend_motor_flag;
        }

        if(square && !prev_square){
            if(gripper_flag){
                weapon_gripper_1.gripper_close();
            }
            else{
                weapon_gripper_1.gripper_open();
            }

            gripper_flag = !gripper_flag;
        }

        if(cross && !prev_cross){
            if(electromagnet_flag){
                weapon_gripper_1.electromagnet_off();
            }
            else{
                weapon_gripper_1.electromagnet_on();
            }

            electromagnet_flag = !electromagnet_flag;
        }

        prev_triangle = triangle;
        prev_square = square;
        prev_cross = cross;

        ThisThread::sleep_for(1000ms/weapon_gripper_freq);
    }
}
#endif

int main(){
    #if EN_PS5
    ps5_thread.start(ps5_communication);
    #endif
    chassis_thread.start(chassis_control);
    #if EN_chassis_cylinders
    chassis_cylinder_thread.start(chassis_cylinder_control);
    #endif
    #if EN_weapon_gripper
    weapon_gripper_thread.start(weapon_gripper_control);
    #endif

    while(1){
        ThisThread::sleep_for(10ms);
    }
}
