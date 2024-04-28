#ifndef __SENSOR_H__
    #define __SENSOR_H__

    #include <driver/gpio.h>
    #include <driver/adc.h>

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


    private:
        struct MuxSelection privMuxSel;
        uint32_t privNumSensors;
        adc1_channel_t privAdcChannel;
    };

#endif