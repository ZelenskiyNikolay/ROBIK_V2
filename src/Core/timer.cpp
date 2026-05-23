#include "timer.h"

unsigned long lastTime = 0;
unsigned long lastTime1 = 0;

float currentMillis = 0;
int callsPerSecond = 0;
float currentMillis1 = 0;
volatile int callsPerSecond1 = 0;
volatile bool clear1 = false;

float getDeltaTime()
{
    unsigned long now = millis();
    unsigned long dt = now - lastTime;
    lastTime = now;

    return dt;
}
float getDeltaTime1()
{
    unsigned long now = millis();
    unsigned long dt = now - lastTime1;
    lastTime1 = now;

    return dt;
}

extern "C" char *sbrk(int incr);

int freeMemory()
{
    char stack_dummy = 0;
    return &stack_dummy - sbrk(0);
}
void FpsCount(float dt)
{
    callsPerSecond++;
    currentMillis += dt;
    if (currentMillis >= 1000)
    {
        Serial.print("FPS CPU0: ");
        Serial.print(callsPerSecond);
        Serial.print(" ||  FPS CPU1: ");
        Serial.print(callsPerSecond1);
        Serial.print(" Память: ");
        Serial.println(abs(freeMemory()));
        callsPerSecond = 0;
        currentMillis = 0;
        clear1 = true;
    }
}

void FpsCount1(float dt)
{
  callsPerSecond1++;
  currentMillis1 += dt;
  if (clear1)
  {
    callsPerSecond1 = 0;
    currentMillis1 = 0;
    clear1 = false;
  }
}