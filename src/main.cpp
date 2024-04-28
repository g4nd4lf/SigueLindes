#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <driver/ledc.h>
#include <esp_log.h>
#include <driver/adc.h>
#include <sensorArray.h>

//#define LED_PIN GPIO_NUM_2
//Pins recomendados: 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33
//Pins motores: 17, 18, 21, 22, 25
//Pins mux: 16, 19, 23, 26, 27, 32, 33
// S0 - 16
// S1 - 19
// S2 - 23
// S3 - 26
// SIG - 27

//constexpr gpio_num_t PWMA_PIN = GPIO_NUM_16;
constexpr gpio_num_t AIN2_PIN = GPIO_NUM_18;
constexpr gpio_num_t AIN1_PIN = GPIO_NUM_17;
constexpr gpio_num_t STBY_PIN = GPIO_NUM_25;
constexpr gpio_num_t BIN1_PIN = GPIO_NUM_21;
constexpr gpio_num_t BIN2_PIN = GPIO_NUM_22;
//constexpr gpio_num_t PWMB_PIN = GPIO_NUM_23;

extern "C" void app_main();

void app_main() 
{
//Setup
printf("comenzamos!");
//Tema 10: Mux
constexpr uint32_t NUM_SENSORS = 8;
MuxSelection muxSel = 
{
    .S0 =GPIO_NUM_22,
    .S1 =GPIO_NUM_21,
    .S2 =GPIO_NUM_19,
    .S3 =GPIO_NUM_18,
};

SensorArray mySensorArray(muxSel, NUM_SENSORS, ADC1_CHANNEL_0);


// Tema 9: ADC
adc1_config_width(ADC_WIDTH_BIT_12);
adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
uint16_t adcValue;

//Configure STBY, AIN1,AIN2, BIN1 and BIN2 as digital pins in mode output:
gpio_config_t config;
config.mode = GPIO_MODE_OUTPUT;
config.pin_bit_mask = (1<<STBY_PIN);
config.intr_type = GPIO_INTR_DISABLE;
config.pull_up_en= GPIO_PULLUP_DISABLE;
config.pull_down_en= GPIO_PULLDOWN_DISABLE;

//set STBY to HIGH to enable H Bridge:
config.pin_bit_mask = (1<<STBY_PIN);
gpio_config(&config);
gpio_set_level(STBY_PIN, 1); 

//set AIN1 to HIGH and AIN2 to LOW for Motor A CW rotation:
config.pin_bit_mask = (1<<AIN1_PIN);
gpio_config(&config);
gpio_set_level(AIN1_PIN, 1); 
config.pin_bit_mask = (1<<AIN2_PIN);
gpio_config(&config);
gpio_set_level(AIN2_PIN, 0); 

//set BIN1 to HIGH and BIN2 to LOW for Motor A CW rotation:
config.pin_bit_mask = (1<<BIN1_PIN);
gpio_config(&config);
gpio_set_level(BIN1_PIN, 1); 
config.pin_bit_mask = (1<<BIN2_PIN);
gpio_config(&config);
gpio_set_level(BIN2_PIN, 1); 

//Set PWMA cycle

ledc_timer_config_t ledcTimer_A;
ledcTimer_A.speed_mode = LEDC_LOW_SPEED_MODE;
ledcTimer_A.freq_hz = 500;
ledcTimer_A.duty_resolution = LEDC_TIMER_13_BIT;
ledcTimer_A.clk_cfg = LEDC_AUTO_CLK;
ledcTimer_A.timer_num = LEDC_TIMER_0;
ledc_timer_config(&ledcTimer_A);

ledc_channel_config_t ledcChannel_A;
ledcChannel_A.channel = LEDC_CHANNEL_0;
ledcChannel_A.gpio_num = GPIO_NUM_16; //PWMA in pin 16
ledcChannel_A.duty = 4096;  //0...2^13-> 0...8192
ledcChannel_A.timer_sel = LEDC_TIMER_0;
ledcChannel_A.intr_type = LEDC_INTR_DISABLE;
ledcChannel_A.speed_mode = LEDC_LOW_SPEED_MODE;
ledcChannel_A.flags.output_invert = 0;
ledc_channel_config(&ledcChannel_A);

ledc_timer_config_t ledcTimer_B;
ledcTimer_B.speed_mode = LEDC_LOW_SPEED_MODE;
ledcTimer_B.freq_hz = 500;
ledcTimer_B.duty_resolution = LEDC_TIMER_13_BIT;
ledcTimer_B.clk_cfg = LEDC_AUTO_CLK;
ledcTimer_B.timer_num = LEDC_TIMER_1;
ledc_timer_config(&ledcTimer_B);

ledc_channel_config_t ledcChannel_B;
ledcChannel_B.channel = LEDC_CHANNEL_1;
ledcChannel_B.gpio_num = GPIO_NUM_23; //PWMA in pin 23
ledcChannel_B.duty = 4096;  //0...2^13-> 0...8192
ledcChannel_B.timer_sel = LEDC_TIMER_1;
ledcChannel_B.intr_type = LEDC_INTR_DISABLE;
ledcChannel_B.speed_mode = LEDC_LOW_SPEED_MODE;
ledcChannel_B.flags.output_invert = 0;
ledc_channel_config(&ledcChannel_B);



uint32_t cnt=0;
float lastLineValue;
//loop
while (1)
{
    lastLineValue=mySensorArray.getLinePosition();
    vTaskDelay(2);
    
    /*
    vTaskDelay(pdMS_TO_TICKS(10));
    adcValue = adc1_get_raw(ADC1_CHANNEL_0);
    printf("Value: %d\n", adcValue);
    
    vTaskDelay(pdMS_TO_TICKS(10));
    
    ledc_set_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,cnt);//+=30);
    ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0);
    ledc_set_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_1,cnt);//+=30);
    ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_1);
    
    if (cnt> 8192){ // (1<<13)){
        cnt=0;
    }
    */
}
    
}