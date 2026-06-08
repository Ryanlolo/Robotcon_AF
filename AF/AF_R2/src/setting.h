#ifndef _SETTING_H_
#define _SETTING_H_

// functions
#define EN_PS5                              1
#define EN_chassis_cylinders                1

// CAN Bus (STM32 Built-in) - using CAN2 on PB_5/PB_13 (same as working code)
#define can_rx                              PA_11
#define can_tx                              PA_12
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

// chassis
#define chassis_freq                        100

// Cylinder control
#define front_chassis_cylinder_pin          PA_6
#define back_chassis_cylinder_pin           PA_7
#define chassis_cylinder_freq               10

// HWT101CT Gyroscope (IMU)
#define EN_HWT101CT                          1
#define imu_rx                               PA_0
#define imu_tx                               PA_1
#define imu_baud                             115200

// SK60p Laser Distance Sensor
#define EN_SK60P                              1
#define laser_rx                              PC_11
#define laser_tx                              PC_10
#define laser_baud                            19200

#endif
