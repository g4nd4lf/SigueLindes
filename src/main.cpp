#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <driver/ledc.h>

//#define LED_PIN GPIO_NUM_2
constexpr gpio_num_t LED_PIN = GPIO_NUM_4;
//constexpr gpio_num_t BUTTON_PIN = GPIO_NUM_4;

extern "C" void app_main();

void app_main() 
{
//Setup
printf("********test PWM********* \n");

gpio_config_t config;
config.mode = GPIO_MODE_OUTPUT;
config.pin_bit_mask = (1ULL<<LED_PIN);
config.intr_type = GPIO_INTR_DISABLE;
config.pull_up_en= GPIO_PULLUP_DISABLE;
config.pull_down_en= GPIO_PULLDOWN_DISABLE;
gpio_config(&config);

ledc_timer_config_t ledcTimer;
ledcTimer.speed_mode = LEDC_LOW_SPEED_MODE;
ledcTimer.freq_hz = 500;
ledcTimer.duty_resolution = LEDC_TIMER_13_BIT;
ledcTimer.clk_cfg = LEDC_AUTO_CLK;
ledcTimer.timer_num = LEDC_TIMER_0;
//ledc_timer_config(&ledcTimer);
if (ledc_timer_config(&ledcTimer) != ESP_OK) {
        printf("Error: no se pudo configurar el temporizador LEDC\n");
        return;
    }

ledc_channel_config_t ledcChannel;
ledcChannel.channel = LEDC_CHANNEL_0;
ledcChannel.gpio_num = GPIO_NUM_22;
ledcChannel.duty = 4096;  //0...2^13-> 0...8192
ledcChannel.timer_sel = LEDC_TIMER_0;
ledcChannel.intr_type = LEDC_INTR_DISABLE;
ledcChannel.speed_mode = LEDC_LOW_SPEED_MODE;
ledcChannel.flags.output_invert = 0;
//ledc_channel_config(&ledcChannel);
if (ledc_channel_config(&ledcChannel) != ESP_OK) {
        printf("Error: no se pudo configurar el canal LEDC\n");
        return;
    }


uint32_t cnt=0;
//loop
while (1)
{
    
    vTaskDelay(pdMS_TO_TICKS(10));
    ledc_set_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,cnt+=30);
    ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0);
    if (cnt> 8192){ // (1<<13)){
        cnt=0;
    }
}
    
}