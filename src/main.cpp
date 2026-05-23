#include <includes.h>

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  delay(200); // дать питанию и SPI стабилизироваться

  randomSeed(analogRead(A3));

  display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET, OLED_CS);
  displaySys = new DisplayOled(*display);

  display->begin(SSD1306_SWITCHCAPVCC);
  display->clearDisplay();
  display->display();

  fsm = new FSM(new StateStartV2(*displaySys), displaySys);

  Compass::getInstance().begin();
  SDModule::getInstance().begin();
  SoundManager::getInstance().Init();
  BatteryModule::getInstance().begin(A0,2);
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
  displaySys->update();

  FpsCount(dt);
  tight_loop_contents();
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
