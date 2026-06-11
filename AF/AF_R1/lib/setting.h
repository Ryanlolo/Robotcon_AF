#ifndef _SETTING_H_
#define _SETTING_H_

// functions
#define EN_PS5                              1
#define EN_root_cylinders                   1

// CAN Bus (STM32 Built-in) - using CAN2 on PB_5/PB_13 (same as working code)
#define can_rx                              PB_5
#define can_tx                              PB_13
#define can_baud                            1000000

// PS5 Serial - using PC_12/PD_2 (same as working code)
#define ps5_rx                              PD_2
#define ps5_tx                              PC_12
#define ps5_baud                            115200
#define PS5_deadzone                        10
#define PS5_freq                            100

// vesc id
#define fl_id                               2
#define fr_id                               1
#define bl_id                               3
#define br_id                               4
#define lifter_id                           5

// chassis
#define chassis_freq                        100
#define chassis_max_speed                   10000.0f
#define chassis_slow_speed                  5000.0f
#define PS5_trigger_threshold               128

// Cylinder control
#define weapon_cylinder1_pin                PA_6
#define weapon_cylinder2_pin                PA_7
#define kfs_gripper_pin                     PB_6
#define weapon_gripper_pin                  PB_0
#define root_cylinders_freq                 10
#define lifter_speed                        1000.0f
#define lifter_brake_current                7.0f

#endif
