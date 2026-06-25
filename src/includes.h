#include <Arduino.h>
#include "MOVE/Motor.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "MOVE/MotionController.h"
#include "MOVE/SafetyModule.h"

#include "Sensors/Compass.h"
#include "Sensors/IRSensor.h"
#include "Module/SDModule.h"
#include "Module/USBModule.h"

#include "Core/FSM.h"
#include "State/StateStartV2.h"
#include "State/StateStart.h"

#include "Sound/SoundModule.h"
#include "Sound/SoundManager.h"

#include "SoundMic/MicRecorder.h"

#include "Module/BatteryModule.h"
#include "Module/RTCModule.h"
#include "Core/timer.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_DC 21
#define OLED_CS 17
#define OLED_RESET 20


FSM *fsm = nullptr;

Adafruit_SSD1306 *display = nullptr;
DisplayOled *displaySys = nullptr;