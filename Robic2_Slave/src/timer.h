#pragma once
#include <Arduino.h>

extern unsigned long lastTime;
extern unsigned long lastTime1;

extern float currentMillis;
extern int callsPerSecond;
extern float currentMillis1;
extern volatile int callsPerSecond1;
extern volatile bool clear1;

float getDeltaTime();
float getDeltaTime1();

int freeMemory();
void FpsCount(float dt);
void FpsCount1(float dt);
