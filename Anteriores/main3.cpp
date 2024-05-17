#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <driver/ledc.h>
#include <esp_log.h>
#include <driver/adc.h>
#include <esp_timer.h>
#include "sensorArray.h"
#include "motorController.h"
#include "pid.h"
//#define LED_PIN GPIO_NUM_2
//Pins recomendados: 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33
//Pins motores: 17, 18, 21, 22, 25
//AIN1 - 25
//AIN2 - 33
//BIN1 - 26
//BIN2 - 27
//PWM0 - 32
//PWM1 - 14

//constexpr gpio_num_t PWMB_PIN = GPIO_NUM_23;
//BIN2
// S0 - 22
// S1 - 21
// S2 - 19
// S3 - 18
// SIG - ADC1_0 -36

//constexpr gpio_num_t PWMB_PIN = GPIO_NUM_23;

extern "C" void app_main();

void app_main3() 
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
MotorController myMotorController(GPIO_NUM_25, GPIO_NUM_33, GPIO_NUM_26, GPIO_NUM_27, GPIO_NUM_32, GPIO_NUM_14);

PID pid(50.0f, 0.0f, 0.0f);

myMotorController.init();
myMotorController.setDirections(0,1,0,1);
// Tema 9: ADC
adc1_config_width(ADC_WIDTH_BIT_12);
adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
uint16_t adcValue;

uint32_t cnt=0;
float lastLineValue;
int newAdcValue=0;
mySensorArray.init();
//loop
uint64_t last_call = esp_timer_get_time();
uint64_t curr_time = esp_timer_get_time();
uint8_t first_run = 1;
float result = 0;

while (1)
{
    //lastLineValue=mySensorArray.getLinePosition();
    //newAdcValue=mySensorArray.readSensor(7);
    lastLineValue = mySensorArray.getLinePosition();

    //printf("\n");
    if(!first_run)
        {
            curr_time = esp_timer_get_time();
            result = pid.update(lastLineValue, curr_time-last_call);
            result = result / 100.0f;
            
            
            if (result > 100){
                result = 100;
            }
        }
    printf("%.2f ", result);
    printf("\n");
    last_call = curr_time;
    first_run = 0;
    myMotorController.setSpeeds(300+(int)result,300-(int)result);
    vTaskDelay(pdMS_TO_TICKS(2));

}
    
}