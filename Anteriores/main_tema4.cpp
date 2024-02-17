#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>

//#define LED_PIN GPIO_NUM_2
constexpr gpio_num_t LED_PIN = GPIO_NUM_2;
constexpr gpio_num_t BUTTON_PIN = GPIO_NUM_4;


extern "C" void app_main();

void app_main() 
{
//Setup
gpio_config_t config;
config.mode = GPIO_MODE_OUTPUT;
config.pin_bit_mask = (1<<LED_PIN);
config.intr_type = GPIO_INTR_DISABLE;
config.pull_up_en= GPIO_PULLUP_DISABLE;
config.pull_down_en= GPIO_PULLDOWN_DISABLE;
gpio_config(&config);

gpio_config_t config2;
config2.mode = GPIO_MODE_INPUT;
config2.pin_bit_mask = (1ULL<<BUTTON_PIN);
config2.intr_type = GPIO_INTR_DISABLE;
config2.pull_up_en= GPIO_PULLUP_DISABLE;
config2.pull_down_en= GPIO_PULLDOWN_ENABLE;
gpio_config(&config2);


//loop
while (1)
{
    if (gpio_get_level(BUTTON_PIN) == 1)
        {  
            gpio_set_level(LED_PIN, 1);        
        } 
        else
        {
            gpio_set_level(LED_PIN, 0);        
        }
    vTaskDelay(1);
    /*    
    gpio_set_level(LED_PIN,1);
    vTaskDelay(pdMS_TO_TICKS(300));
    gpio_set_level(LED_PIN,0);
    vTaskDelay(pdMS_TO_TICKS(300));
    */
}
    
}