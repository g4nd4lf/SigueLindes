#ifndef __MOTOR_CONTROLLER_H__
    #define __MOTOR_CONTROLLER_H__

    #include <driver/gpio.h>
    #include <driver/adc.h>
    #include <stdint.h>
    #include <driver/ledc.h>
   


    class MotorController
    {
    public:
        //Constructor
        MotorController( gpio_num_t AIN1, gpio_num_t AIN2, gpio_num_t BIN1, gpio_num_t BIN2, gpio_num_t PWM0, gpio_num_t PWM1);

        //Initialization
        void init();

        void setDirections(uint8_t a1, uint8_t a2, uint8_t b1, uint8_t b2);
        void setSpeeds(uint32_t speed1, uint32_t speed2);

    private:
        gpio_num_t privAIN1;
        gpio_num_t privAIN2;
        gpio_num_t privBIN1;
        gpio_num_t privBIN2;
        gpio_num_t privPWM0;
        gpio_num_t privPWM1;
    };

#endif