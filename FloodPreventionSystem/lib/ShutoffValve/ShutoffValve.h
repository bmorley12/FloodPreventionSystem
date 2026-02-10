#ifndef SHUTOFF_VALVE_H
#define SHUTOFF_VALVE_H

#include <Servo.h>

class ShutoffValve {
    public:
        ShutoffValve(int pin, int min_pulse = 500, int max_pulse = 2500);
        void turn_off();
        void turn_on();
        bool shutoff_status();

    private:  
        Servo _valve;
        bool _shutoff;
        int _min_pulse;
        int _max_pulse;
        void _move_to_angle(int angle);

};

#endif