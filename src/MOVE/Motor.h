#pragma once
#include <Arduino.h>

//// Последовательность для half-step (плавно и тихо)
static const int steps[8][4] = {
  {1, 0, 0, 0},
  {1, 1, 0, 0},
  {0, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 1},
  {0, 0, 0, 1},
  {1, 0, 0, 1}
};

class Motor
{
public:
    Motor(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4);

    void begin();
    void step(int dir);
    void Idel();
private:
    uint8_t pins[4];
    int stepIndex = 0;
    bool idel = true;
    void setStep(uint8_t s);
};
