//Headers
#ifndef TEMP_AND_HUMID_H
#define TEMP_AND_HUMID_H

#include <Wire.h>
#include <Adafruit_AHTX0.h>

class TempAndHumiditySensor {
    public:
        TempAndHumiditySensor();
        int begin(int sda, int scl);
        void read_sensors();
        float get_temp();
        float get_humidity();

    private:
        Adafruit_AHTX0 _aht;
        sensors_event_t _tempSensor;
        float _temp;
        sensors_event_t _humiditySensor;
        float _humidity;

};

#endif