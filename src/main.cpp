#include <Arduino.h>
#include <includes.h>

#include "Sensors/Compass.h"
#include "Sensors/IRSensor.h"

IRSensor ir(23);

float timer_compas = 500;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_DC 16
#define OLED_CS 17
#define OLED_RESET 20

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         &SPI, OLED_DC, OLED_RESET, OLED_CS);

// MotionController Move;
bool forvard = false;

void FpsCount(float dt);
void FpsCount1(float dt);

unsigned long lastTime = 0;
unsigned long lastTime1 = 0;
float currentMillis;
int callsPerSecond;
float currentMillis1;
volatile int callsPerSecond1;
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

void IrLogic();
bool autoMov = false;

void setup()
{
  Serial.begin(9600);
  delay(1000);

  Serial.println("Serial Active");

  SPI.begin();
  // SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));

  delay(200); // дать питанию и SPI стабилизироваться

  if (!display.begin(SSD1306_SWITCHCAPVCC))
  {
    while (true)
      ;
  }

  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("TEST");

  display.display();

  if (!Compass::getInstance().begin())
    Serial.println("Compass fail");
  else
    Serial.println("Compass OK");
}

void loop()
{
  float dt = getDeltaTime();

  Compass::getInstance().update(dt);
  timer_compas -= dt;
  if (timer_compas <= 0)
  {
    float h = Compass::getInstance().getHeading();

    display.clearDisplay();

    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    char buffer1[32];
    sprintf(buffer1, "Ang:%d''", (int)h);
    display.println(buffer1);

    display.display();

    Serial.print(h);
    Serial.println("'");
    timer_compas = 1000;
  }

  FpsCount(dt);
  tight_loop_contents();
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
// ================================================================
// ЯДРО 1 (Core 1): Занимается только моторами (Критично ко времени)
// ================================================================
static bool led = false;
void setup1()
{
  delay(1500);

  SafetyModule::getInstance().NewMov(MotionState::FORWARD, 1, 1);
}
void loop1()
{
  float dt1 = getDeltaTime1();

  IrLogic();

  SafetyModule::getInstance().update(dt1);

  if (autoMov)
    if (!SafetyModule::getInstance().isBusy())
    {
      if (!forvard)
      {
        SafetyModule::getInstance().NewMov(MotionState::FORWARD, 2, 2);
        forvard = !forvard;
      }
      else
      {
        SafetyModule::getInstance().NewMov(MotionState::TURN_LEFT90);
        forvard = !forvard;
      }
    }
  FpsCount1(dt1);
  tight_loop_contents();
}

void IrLogic()
{
  ir.update();
  ButtonIR tmp = ir.GetSensorState();
  switch (tmp)
  {
  case Button1:
    SafetyModule::getInstance().MoveSpeed(false);
    break;
  case Button2:
    SafetyModule::getInstance().MoveSpeed();
    break;

  case Button5:
    SafetyModule::getInstance().NewMov(MotionState::FORWARD, 0.1f, 0.1f);
    break;
  case ButtonUp:
    SafetyModule::getInstance().NewMov(MotionState::FORWARD, 0.5f, 0.5f);
    break;
  case ButtonDown:
    SafetyModule::getInstance().NewMov(MotionState::BACKWARD, 1, 1);
    break;
  case Button3:
    // SafetyModule::getInstance().NewMov(MotionState::TURN_RIGHT, 0, 0);
    break;
  case ButtonLeft:
    SafetyModule::getInstance().NewMov(MotionState::TURN_LEFT90);
    break;
  case ButtonRight:
    // SafetyModule::getInstance().NewMov(MotionState::TURN_RIGHT90);
    break;
  case ButtonOk:
    // autoMov = !autoMov;
    break;
  default:
    break;
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