//headers
#ifndef WATER_SENSOR_H
#define WATER_SENSOR_H

#include <Arduino.h>

class WaterSensor {
    public:
        WaterSensor(int pin);
        void begin(void (*callback)()); 
        bool isDetected();
        void reset();

    private:
        int _pin;
        volatile bool _triggered;

};

#endif
