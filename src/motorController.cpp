#include "motorController.h"
MotorController::MotorController(gpio_num_t AIN1, gpio_num_t AIN2, gpio_num_t BIN1, gpio_num_t BIN2, gpio_num_t PWM0, gpio_num_t PWM1)
{
    privAIN1 = AIN1;
    privAIN2 = AIN2;
    privBIN1 = BIN1;
    privBIN2 = BIN2;
    privPWM0 = PWM0;
    privPWM1 = PWM1;
}

void MotorController::init()
{
    // Configure GPIO pins for motors direction
    gpio_config_t config;
    config.mode = GPIO_MODE_OUTPUT;
    config.pin_bit_mask = (((uint64_t)1) << privAIN1) |  (((uint64_t)1) << privAIN2) |  (((uint64_t)1) << privBIN1)|  (((uint64_t)1) << privBIN2);
    config.intr_type = GPIO_INTR_DISABLE;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&config);

    // Configure LEDC PWM timer for Motors speed

    ledc_timer_config_t ledcTimer;
    ledcTimer.speed_mode = LEDC_LOW_SPEED_MODE;
    ledcTimer.freq_hz = 500;
    ledcTimer.duty_resolution = LEDC_TIMER_10_BIT;
    ledcTimer.clk_cfg = LEDC_AUTO_CLK;
    ledcTimer.timer_num = LEDC_TIMER_0;
    ledc_timer_config(&ledcTimer);

    ledc_channel_config_t ledcChannel;
    ledcChannel.channel = LEDC_CHANNEL_0;
    ledcChannel.gpio_num = privPWM0;
    ledcChannel.duty = 0;
    ledcChannel.timer_sel = LEDC_TIMER_0;
    ledcChannel.intr_type = LEDC_INTR_DISABLE;
    ledcChannel.speed_mode = LEDC_LOW_SPEED_MODE;
    ledcChannel.flags.output_invert = 0;
    ledc_channel_config(&ledcChannel);

    ledcChannel.channel = LEDC_CHANNEL_1;
    ledcChannel.gpio_num = privPWM1;
    ledc_channel_config(&ledcChannel);
}
void MotorController::setDirections(uint8_t a1, uint8_t a2, uint8_t b1, uint8_t b2){
    gpio_set_level(privAIN1, a1);
    gpio_set_level(privAIN2, a2);
    gpio_set_level(privBIN1, b1);
    gpio_set_level(privBIN2, b2);
}
void MotorController::setSpeeds(uint32_t speed1, uint32_t speed2){
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, speed1);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, speed2);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
}