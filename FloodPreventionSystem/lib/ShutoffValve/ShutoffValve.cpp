#include "ShutoffValve.h"

ShutoffValve::ShutoffValve(int pin, int min_pulse, int max_pulse) {
    _shutoff = false;
    _min_pulse = min_pulse;
    _max_pulse = max_pulse;

    _valve.attach(pin, _min_pulse, _max_pulse);
    _move_to_angle(0);      // initialize valves to the open position
}

void ShutoffValve::turn_off() {
    _shutoff = true;
    _move_to_angle(90);
}

void ShutoffValve::turn_on() {
    _shutoff = false;
    _move_to_angle(0);
}

bool ShutoffValve::shutoff_status() {
    return _shutoff;
}

void ShutoffValve::_move_to_angle(int angle) {
    int pulse = map(angle * 100, 0, 27000, _min_pulse, _max_pulse);
    _valve.writeMicroseconds(pulse);

    // delay(2000);        // delay for movement, comment out for prod
}

