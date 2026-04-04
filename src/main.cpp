#include <includes.h>

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

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  delay(200); // дать питанию и SPI стабилизироваться

  if (!display.begin(SSD1306_SWITCHCAPVCC))
  {
    while (true)
      ;
  }

  fsm = new FSM(new StateStart(displaySys), &displaySys);

  Compass::getInstance().begin();
  SDModule::getInstance().begin();
  SoundManager::getInstance().Init();
  BatteryModule::getInstance().begin(A0);
  IRSensor::getInstance().begin(23);
  RTCModule::getInstance().begin();
}

void loop()
{
  float dt = getDeltaTime();

  SoundManager::getInstance().update();
  BatteryModule::getInstance().update(dt);
  IRSensor::getInstance().update();
  RTCModule::getInstance().update(dt);
  Compass::getInstance().update(dt);

  // FSM
  while (EventBus::hasEvents())
  {
    fsm->handleEvent(EventBus::poll());
  }
  fsm->update(dt);
  displaySys.update();

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
  SoundModule::getInstance().Init();

  delay(1500);

  SafetyModule::getInstance().NewMov(MotionState::FORWARD, 0.01f, 0.01f);
  MicRecorder::getInstance().Init(27);
}
void loop1()
{
  float dt1 = getDeltaTime1();


  SafetyModule::getInstance().update(dt1);

  FpsCount1(dt1);
  tight_loop_contents();
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