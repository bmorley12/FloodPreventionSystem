#include "TempAndHumiditySensor.h"

TempAndHumiditySensor::TempAndHumiditySensor() {
    _temp = -100;
    _humidity = -1;
}

int TempAndHumiditySensor::begin(int sda, int scl){
    // Wire.begin(sda, scl);

    delay(1000);

    if (! _aht.begin() ){
        Serial.printf("Could not find AHT20 sensor!\n");
        return 1;
    }
    Serial.printf("AHT20 sensor found\n");
    read_sensors();
    return 0;

}

void TempAndHumiditySensor::read_sensors(){
    _aht.getEvent(&_humiditySensor, &_tempSensor);
    _temp = _tempSensor.temperature;
    _humidity = _humiditySensor.relative_humidity;
}


float TempAndHumiditySensor::get_temp(){
    return _temp;
}

float TempAndHumiditySensor::get_humidity(){
    return _humidity;
}