#include <Arduino.h>
#include <hardware/pio.h>
#include "ultrasonic.pio.h"
#include "hardware/clocks.h"

#define UP_TIME 500

class Ultrasonic {
private:
    float getDistance();

    PIO pio;
    uint sm;
    uint offset;

    float _last_dis = -1;
    bool busy;
    float timer = 0;
public:
    Ultrasonic(PIO pio_hw, uint trig_pin, uint echo_pin);
    void TriggerUltrasonic();
    float DistanceUltrasonic();
    void  update(float dt);
};
