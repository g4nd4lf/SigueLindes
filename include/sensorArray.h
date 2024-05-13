#ifndef __SENSOR_ARRAY_H__
    #define __SENSOR_ARRAY_H__

    #include <driver/gpio.h>
    #include <driver/adc.h>
    #include <stdint.h>
    
    struct MuxSelection{
        gpio_num_t S0;
        gpio_num_t S1;
        gpio_num_t S2; 
        gpio_num_t S3;
    };

    class SensorArray
    {
    public:
        //Constructor
        SensorArray( struct MuxSelection muxsel, uint32_t numSensors, adc1_channel_t adcChannel);

        //Initialization
        void init();

        void calibrate();

        int32_t getLinePosition();
        int32_t readSensor(uint16_t sensor);

    private:
        struct MuxSelection privMuxSel;
        uint32_t privNumSensors;
        adc1_channel_t privAdcChannel;
    };

#endif