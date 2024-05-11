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
// S0 - 22
// S1 - 21
// S2 - 19
// S3 - 18
// SIG - 27

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

uint32_t cnt=0;
float lastLineValue;
int newAdcValue=0;
//loop
while (1)
{
    //lastLineValue=mySensorArray.getLinePosition();
    newAdcValue=mySensorArray.readSensor(7);
    vTaskDelay(pdMS_TO_TICKS(1000));

}
    
}