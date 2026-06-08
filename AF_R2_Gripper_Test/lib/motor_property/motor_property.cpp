#include "motor_property.h"

motor_property::motor_property(){}

motor_property::motor_property(Motor_type type){
    switch(type){
        case m3508:
            //m3508 p19
            //MAX MRPM: 8500rpm
            //MAX current: 20.0A
            //Number of poles: 14
            //Gear ratio: 3591/187 
            setAllparam(8500.0f, 20.0f, 14, (float)(3591.0f/187.0f));
            break;
        
        case BDUAV_5065_400KV:
            //BDUAV 5065 400KV
            //MAX MRPM: 9600rpm
            //MAX current: 60.0A
            //Number of poles: 14
            //Gear ratio: 1/1 
            setAllparam(9600.0f, 60.0f, 14, 1.0f);
            break;

        case BDUAV_5065_140KV:
            //BDUAV 5065 140KV
            //MAX MRPM: 3360rpm
            //MAX current: 60.0A
            //Number of poles: 14
            //Gear ratio: 1/1 
            setAllparam(3360.0f, 60.0f, 14, 1.0f);
            break;

        case EAGLEPOWER_LA8318_100KV:
            //EAGLEPOWER LA8318 100KV
            //MAX MRPM:  2400rpm
            //MAX current: 80.0A
            //Number of poles: 40
            //Gear ratio: 1/1 
            setAllparam(2400.0f, 80.0f, 40, 1.0f);
            break;

        case m2006:
            //m2006 p36
            //MAX MRPM: 3000rpm
            //MAX current: 3.0A
            //Number of poles: 14
            //Gear ratio:  36/1
            setAllparam(3000.0f, 3.0f, 14, 36.0f);
            break;

        default:
            break;
    }
}

void motor_property::setAllparam(float rpm, float current, int polesNumber, float ratio){
    setMaxRPM(rpm);
    setMaxCurrent(current);
    setPolesNumber(polesNumber);
    setGearRatio(ratio);
}

motor_property::motor_property(float rpm, float current, int polesNumber, float ratio){
    setAllparam(rpm, current, polesNumber, ratio);
}

void motor_property::setMaxRPM(float rpm){
    maxRPM = rpm;
}

void motor_property::setMaxCurrent(float current){
    maxCurrent = current;
}

void motor_property::setPolesNumber(int number){
    polesNumber = number;
}

void motor_property::setGearRatio(float ratio){
    gear_ratio = ratio;
}

float motor_property::getMaxRPM(){
    return maxRPM;
}

float motor_property::getMaxCurrent(){
    return maxCurrent;
}

int motor_property::getPolesNumber(){
    return polesNumber;
}

float motor_property::getGearRatio(){
    return gear_ratio;
}