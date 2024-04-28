#include "sensorArray.h"

SensorArray::SensorArray(struct MuxSelection muxSel, 
                         uint32_t numSensors, adc1_channel_t adcChannel)
{
    privMuxSel = muxSel;
    privNumSensors = numSensors;
    privAdcChannel = adcChannel;
}

void SensorArray::init()
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(privAdcChannel, ADC_ATTEN_DB_11);

    gpio_config_t config;
    config.mode = GPIO_MODE_OUTPUT;
    config.pin_bit_mask = 
        (((uint64_t)1) << privMuxSel.S0) |  
        (((uint64_t)1) << privMuxSel.S1) |  
        (((uint64_t)1) << privMuxSel.S2) |  
        (((uint64_t)1) << privMuxSel.S3);

    config.intr_type = GPIO_INTR_DISABLE;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&config);
}

void SensorArray::calibrate()
{

}

int32_t SensorArray::getLinePosition()
{
    int32_t acum = 0;
    int32_t weightened = 0;
    uint16_t adcValue[privNumSensors];

    uint16_t sensorOffset = 3; // Mi sensor 0 es el C3 de la placa de expansion
    uint16_t currentSensor = 0;
    uint16_t currentValue = 0;
    printf("LineData: ");
    for( uint32_t i = 0; i < privNumSensors; i++)
    {
        currentSensor = sensorOffset + i;
        gpio_set_level(privMuxSel.S0, currentSensor & 0x1U );
        gpio_set_level(privMuxSel.S1, (currentSensor>>1) & 0x1U );
        gpio_set_level(privMuxSel.S2, (currentSensor>>2) & 0x1U );
        gpio_set_level(privMuxSel.S3, (currentSensor>>3) & 0x1U );

        currentValue = adc1_get_raw(privAdcChannel); 
        adcValue[i] = currentValue;

        if(currentValue > 2048)
        {
            weightened += ((float)currentValue)*((float)i - ((privNumSensors-1)/2.0f))*100.0f;
            acum += currentValue;
        }

        printf("%d ", adcValue[i]);
    }

    printf("%.2f ", (float)weightened/acum);
    return (int32_t)weightened/acum;
}