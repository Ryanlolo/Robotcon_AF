#include <mbed.h>

#include "setting.h"
#include "PS5.h"
#include "vesc.h"
#include "mecanum_chassis.h"

int mode = 0;   // 0 -> manual mode; 1 -> auto mode

#if EN_PS5
BufferedSerial ps5(ps5_tx, ps5_rx, ps5_baud);
PS5 ps5Controller(&ps5);
#endif

CAN _can3(can_rx, can_tx, can_baud);

vesc motor_fl(fl_id, &_can3, can_baud, Motor_type::m3508);
vesc motor_fr(fr_id, &_can3, can_baud, Motor_type::m3508);
vesc motor_bl(bl_id, &_can3, can_baud, Motor_type::m3508);
vesc motor_br(br_id, &_can3, can_baud, Motor_type::m3508);
mecanum_chassis chassis(&motor_fl, &motor_fr, &motor_bl, &motor_br, true);

#if EN_chassis_cylinders
DigitalOut front_chassis_cylinder(front_chassis_cylinder_pin);
DigitalOut back_chassis_cylinder(back_chassis_cylinder_pin);
#endif

int8_t x, y, w;

#if EN_PS5
Thread ps5_thread(osPriorityHigh2);
#endif
Thread chassis_thread(osPriorityHigh1);
#if EN_chassis_cylinders
Thread chassis_cylinder_thread(osPriorityHigh1);
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

    bool triangle = ps5Controller.isTrianglePressed();
    bool cross = ps5Controller.isXpressed();
    bool front_cylinder_flag = false;
    bool back_cylinder_flag = false;
    bool prev_triangle = false;
    bool prev_cross = false;

    while(1){
        triangle = ps5Controller.isTrianglePressed();
        cross = ps5Controller.isXpressed();

        if(triangle && !prev_triangle){
            if(front_cylinder_flag){
                front_chassis_cylinder.write(1);
            }
            else{
                front_chassis_cylinder.write(0);
            }

            front_cylinder_flag = !front_cylinder_flag;
        }

        if(cross && !prev_cross){
            if(back_cylinder_flag){
                back_chassis_cylinder.write(1);
            }
            else{
                back_chassis_cylinder.write(0);
            }

            back_cylinder_flag = !back_cylinder_flag;
        }

        prev_triangle = triangle;
        prev_cross = cross;

        ThisThread::sleep_for(1000ms/chassis_cylinder_freq);
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

    while(1){
        ThisThread::sleep_for(10ms);
    }
}
