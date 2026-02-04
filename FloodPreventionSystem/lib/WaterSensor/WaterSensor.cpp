#include "WaterSensor.h"

WaterSensor::WaterSensor(int pin) {
    _pin = pin;
    _triggered = false;
}

void WaterSensor::begin(void (*callback)()) {
    pinMode(_pin, INPUT_PULLUP);

    // Attach the interrupt to the function passed from main.cpp
    attachInterrupt(digitalPinToInterrupt(_pin), callback, FALLING);
}

bool WaterSensor::isDetected() {
    return _triggered;
}

void WaterSensor::reset() {
    _triggered = false;
}

