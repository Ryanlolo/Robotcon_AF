#include <mbed.h>

#include "../setting.h"
#include "PS5.h"
#include "vesc.h"
#include "mecanum_chassis.h"
#include "root_gripper.h"

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

vesc motor_lifter(lifter_id, &_can3, can_baud, Motor_type::BDUAV_5065_400KV);

#if EN_root_cylinders
DigitalOut weapon_cylinder1(weapon_cylinder1_pin);
DigitalOut weapon_gripper(weapon_gripper_pin);
DigitalOut weapon_cylinder2(weapon_cylinder2_pin);
DigitalOut kfs_gripper(kfs_gripper_pin);
root_gripper root_gripper_1(weapon_cylinder1, weapon_cylinder2, kfs_gripper, weapon_gripper);
#endif

int8_t x, y, w;

#if EN_PS5
Thread ps5_thread(osPriorityHigh2);
#endif
Thread chassis_thread(osPriorityHigh1);
#if EN_root_cylinders
Thread root_cylinders_thread(osPriorityHigh1);
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
            // printf("Raw: LX=%d LY=%d | Filtered: X=%d Y=%d W=%d\n", raw_lx, raw_ly, x, y, w);
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

            // printf("%d,%d,%d",x,y,w);
        }
        
        ThisThread::sleep_for(1000ms/chassis_freq);
    }
}

#if EN_root_cylinders
void root_cylinders_control(){
    // default contract
    root_gripper_1.lay_down();
    root_gripper_1.extend();
    root_gripper_1.clip_open();
    root_gripper_1.kfs_gripper_open();

    bool triangle = ps5Controller.isTrianglePressed();
    bool square = ps5Controller.isSquarePressed();
    bool cross = ps5Controller.isXpressed();
    bool circle = ps5Controller.isCirclePressed();
    bool up = ps5Controller.isUpPressed();
    bool down = ps5Controller.isDownPressed();

    bool stand_cylinder_flag = false;
    bool extend_cylinder_flag = false;
    bool clip_cylinder_flag = false;
    bool kfs_gripper_flag = false;
    int lifter_state = 0;
    
    bool prev_triangle = false;
    bool prev_square = false;
    bool prev_cross = false;
    bool prev_circle = false;
    bool prev_Up = false;
    bool prev_Down = false;

    while(1){
        triangle = ps5Controller.isTrianglePressed();
        square = ps5Controller.isSquarePressed();
        cross = ps5Controller.isXpressed();
        circle = ps5Controller.isCirclePressed();
        up = ps5Controller.isUpPressed();
        down = ps5Controller.isDownPressed();  

        if(triangle && !prev_triangle){
            if(clip_cylinder_flag){
                root_gripper_1.clip_close();
            }
            else{
                root_gripper_1.clip_open();
            }

            clip_cylinder_flag = !clip_cylinder_flag;
        }

        if(square && !prev_square){
            if(extend_cylinder_flag){
                root_gripper_1.extend();
            }
            else{
                root_gripper_1.contract();
            }

            extend_cylinder_flag = !extend_cylinder_flag;
        }
        
        if(cross && !prev_cross){
            if(stand_cylinder_flag){
                root_gripper_1.stand_up();
            }
            else{
                root_gripper_1.lay_down();
            }

            stand_cylinder_flag = !stand_cylinder_flag;
        }

        if(circle && !prev_circle){
            if(kfs_gripper_flag){
                root_gripper_1.kfs_gripper_close();
            }
            else{
                root_gripper_1.kfs_gripper_open();
            }

            kfs_gripper_flag = !kfs_gripper_flag;
        }


        if(up || down){
            if(up){
                lifter_state = 1;
            }

            if(down){
                lifter_state = 2;
            }
        }
        else{
            lifter_state = 0;
            
        }

        switch(lifter_state){
            case 0:
                motor_lifter.comm_can_set_current_brake(7.0f);
                break;
            case 1:
                motor_lifter.comm_can_set_mrpm(1000.0f);
                break;
            case 2:
                motor_lifter.comm_can_set_mrpm(-1000.0f);
                break;
            default:
                motor_lifter.comm_can_set_current_brake(7.0f);
                break;
        }
        
        prev_triangle = triangle;
        prev_square = square;
        prev_circle = circle;
        prev_cross = cross;
        prev_Up = up;
        prev_Down = down;

        ThisThread::sleep_for(1000ms/root_cylinders_freq);
    }
}
#endif

int main(){
    #if EN_PS5
    ps5_thread.start(ps5_communication);
    #endif
    chassis_thread.start(chassis_control);
    #if EN_root_cylinders
    root_cylinders_thread.start(root_cylinders_control);
    #endif

    while(1){
        ThisThread::sleep_for(10ms);
    }
}
