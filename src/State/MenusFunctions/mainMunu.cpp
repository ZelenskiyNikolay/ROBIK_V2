#include "../StateStartV2.h"
#include "Core/timer.h"

void StateStartV2::exitMenu()
{
  StateStartV2::menu->hide();
  timer = 0;
}
void StateStartV2::back()
{
  menu->back();
  timer = 0;
}
float drawTimer = 0;
void StateStartV2::infoBat()
{
  drawTimer -= getDeltaTime();
  if (drawTimer > 0)
    return;
  drawTimer = 500;

  display->clear();

  char buffer1[16];
  bool ch = BatteryModule::getInstance().IsCharging();
  if (ch)
  {
    display->drawText(" -\\\\BATTERY//-", 0, 0, 1);
    display->drawText(" CHARGING !!!", 0, 10, 1);
  }
  else if (!ch)
  {
    if (!BatteryModule::getInstance().IsBatFull())
      display->drawText("BATTERY SENSOR:", 0, 0, 1);
    else
    {
      display->drawText("BATTERY SENSOR: USB", 0, 0, 1);
      display->drawText("USB->CONNECTED", 0, 10, 1);
    }
  }

  int percent = BatteryModule::getInstance().getBatteryPercent();
  BatteryModule::getInstance().drawBatteryIcon(*display, 0, 20, percent);

  sprintf(buffer1, "Charge:%02d%s", percent, " %");
  display->drawText(buffer1, 0, 44, 1);

  float v = BatteryModule::getInstance().getVoltage();
  int whole = v;
  int fract = (v - whole) * 100;

  sprintf(buffer1, "Voltage:%d.%02d V", whole, fract);
  display->drawText(buffer1, 0, 54, 1);
}

void CompassIcon(int16_t x, int16_t y, DisplayOled &display)
{
  // 1. Рисуем круг (радиус 10 пикселей)
  display.drawCircle(x, y, 10, SSD1306_WHITE);

  // 2. Рисуем "Север" (закрашенный треугольник)
  // Координаты: (верхушка, левый угол, правый угол)
  display.fillTriangle(x, y - 8, x - 4, y, x + 4, y, SSD1306_WHITE);

  // 3. Рисуем "Юг" (пустой треугольник)
  display.drawTriangle(x, y + 8, x - 4, y, x + 4, y, SSD1306_WHITE);

  display.drawText("N", x - 2, y - 18, 1);
}
void StateStartV2::сompassTest()
{
  drawTimer -= getDeltaTime();
  if (drawTimer > 0)
    return;
  drawTimer = 500;

  float heading = Compass::getInstance().getHeading();
  display->clear();
  display->drawText("Compass", 0, 0, 2);
  display->drawText("Test:", 0, 20, 2);
  char buffer[16];

  CompassIcon(110, 25, *display);

  sprintf(buffer, "ANG:%.1f%s", heading, "'");
  display->drawText(buffer, 0, 44, 2);
}
void StateStartV2::ultrasonicTest()
{
  drawTimer -= getDeltaTime();
  if (drawTimer > 0)
    return;
  drawTimer = 100;

  byte reg = readRegister(REG_SYS_CONTROL);
  if (getBit(reg, 2))
  {
    phaseControl(ENABLED, ENABLED);
  }

  float dis = MovementModule::getInstance().GetDistance();
  display->clear();

  display->drawText("Ultrasinic", 0, 0, 2);
  display->drawText("Test:", 0, 20, 2);
  char buffer[16];

  sprintf(buffer, "D:%.1f%s", dis, "CM");
  display->drawText(buffer, 0, 44, 2);
}
void StateStartV2::SetNightMod()
{
  Action a = menu->getAction();
  bool night = display->IsNightMod();

  if (a == NEXT || a == PREV)
  {
    night = !night;
    display->nightMod(night);
  }
  else if (a == OK)
  {
    menu->setEditMode(false);
    menu->resetTimer();
  }
  char buf[16];
  sprintf(buf, ">%s<", night ? "true" : "false");
  menu->setEditModeEmpty(buf);
}
int Vol = 100;
void StateStartV2::SetSoundVolume()
{
  Action a = menu->getAction();

  if (a == NEXT)
  {
    if (Vol > 0)
      Vol -= 5;
  }
  else if (a == PREV)
  {
    if (Vol < 100)
      Vol += 5;
  }
  else if (a == OK)
  {
    // SoundManager->SetVolume(Val) это не реализовано..

    menu->setEditMode(false);
    menu->resetTimer();
  }

  char buf[16];
  sprintf(buf, ">%d%s<", Vol, "%");
  menu->setEditModeEmpty(buf);
}
inline const char *getClickName(MenuClick::BeepVolume volume)
{
  switch (volume)
  {
  case MenuClick::BeepVolume::BEEP_OFF:
    return "BEEP_OFF";
  case MenuClick::BeepVolume::BEEP_QUIET:
    return "BEEP_QUIET";
  case MenuClick::BeepVolume::BEEP_LOUD:
    return "BEEP_LOUD";
  default:
    return "UNKNOWN";
  }
}

MenuClick::BeepVolume vol = MenuClick::BeepVolume::BEEP_OFF;
void StateStartV2::ClickSounnd()
{
  Action a = menu->getAction();

  if (a == NEXT)
  {
    if (vol != MenuClick::BeepVolume::BEEP_LOUD)
      vol = (MenuClick::BeepVolume)((int)vol + 1);
  }
  else if (a == PREV)
  {
    if (vol != MenuClick::BeepVolume::BEEP_OFF)
      vol = (MenuClick::BeepVolume)((int)vol - 1);
  }
  else if (a == OK)
  {
    menu->click.setVol(vol);
    menu->setEditMode(false);
    menu->resetTimer();
  }

  char buf[32];
  sprintf(buf, ">%s<", getClickName(vol));
  menu->setEditModeEmpty(buf, false);
}
enum PowerOffSteeps
{
  SAFE_CHENGES = 0,
  COUNTDOWN,
  POWER_OFF,
  MAX_STEEP
};
void SavedMetodworck() {}
PowerOffSteeps steep = SAFE_CHENGES;
float off_timer = 3000;
int countdown = 5;
void StateStartV2::powerOff()
{
  char buffer1[16];
  switch (steep)
  {
  case MAX_STEEP:
    steep = SAFE_CHENGES;
    off_timer = 3000;
    break;
  case SAFE_CHENGES:
    if (off_timer == 3000)
    {
      display->clear();
      display->drawText("Power OFF", 0, 0, 2);
      display->drawText("Save chenges..", 0, 20, 1);
    }
    off_timer -= getDeltaTime();
    if (off_timer > 0)
    {
      SavedMetodworck();
      break;
    }
    else
    {
      steep = COUNTDOWN;
      off_timer = 1000;
      countdown = 5;
    }
    break;
  case COUNTDOWN:
    off_timer -= getDeltaTime();
    if (off_timer > 0)
      break;

    off_timer = 1000;
    if (countdown >= 0)
    {
      display->clear();
      sprintf(buffer1, "-%d-", countdown);
      display->drawText(buffer1, 0, 0, 7);

      countdown--;
    }
    else
    {
      display->clear();
      display->drawText("OFF", 0, 0, 7);
      Serial.println("Выключаю питание!!!");
      steep = POWER_OFF;
    }
    break;
  case POWER_OFF:
    off_timer -= getDeltaTime();
    if (off_timer > 0)
      break;

    else
    {
      steep = MAX_STEEP;
      phaseControl(DISABLED);
    }
    break;
  default:
    break;
  }
}

inline const char *getAngString(int Angle)
{
  if (Angle == 0)
    return "0\"";
  else if (Angle > 0 && Angle < 46)
    return "45\"";
  else if (Angle > 45 && Angle < 91)
    return "90\"";
  else if (Angle > 90 && Angle < 136)
    return "135\"";
  else if (Angle > 135 && Angle < 181)
    return "180\"";
  else
    return "UNKNOWN";
}
inline const int getAngIndex(int Angle)
{
  if (Angle == 0)
    return 0;
  else if (Angle > 0 && Angle < 46)
    return 1;
  else if (Angle > 45 && Angle < 91)
    return 2;
  else if (Angle > 90 && Angle < 136)
    return 3;
  else if (Angle > 135 && Angle < 181)
    return 4;
  else
    return 0;
}
int ang = 0;
bool init_servo = false;
void StateStartV2::servo(int angle)
{
  if (!init_servo)
  {
    phaseControl(ENABLED, ENABLED);
    init_servo = !init_servo;
    radar.SetMicrosServo(radar.servoAngleTable[getAngIndex(angle)]);
  }

  Action a = menu->getAction();

  if (a == NEXT)
  {
    radar.servoAngleTable[getAngIndex(angle)] -= 50;
    radar.SetMicrosServo(radar.servoAngleTable[getAngIndex(angle)]);
  }
  else if (a == PREV)
  {
    radar.servoAngleTable[getAngIndex(angle)] += 50;
    radar.SetMicrosServo(radar.servoAngleTable[getAngIndex(angle)]);
  }
  else if (a == OK)
  {
    phaseControl(ENABLED);
    init_servo = !init_servo;
    menu->setEditMode(false);
  }

  char buf[32];
  sprintf(buf, ">Servo milis>%d<", radar.servoAngleTable[getAngIndex(angle)]);
  menu->setEditModeEmpty(buf, false);
}

void DrawRadar(Radar radar, DisplayOled &disp)
{
  const int MAX_DIST = 1000; // Наш предел в 1 метра

  for (int i = 0; i < 5; i++)
  {
    int dist = radar.GetData(i);
    int barWidth = 0;

    // Если замер корректный и в пределах 2 метров
    if (dist > 0)
    {
      if (dist > MAX_DIST)
        dist = MAX_DIST; // Ограничиваем сверху

      // Быстрый целочисленный масштаб (эквивалент dist * SCREEN_WIDTH / MAX_DIST)
      barWidth = disp.SCREEN_WIDTH - ((dist * 8) / 62);
    }
    else if (dist == 0)
    {
      barWidth = disp.SCREEN_WIDTH; // Препятствие в упор — полоса на весь экран
    }
    // Если dist == -1 (нет данных), barWidth останется 0 (пустая строка)

    // Вычисляем координату Y для каждой из 5 полос
    // Стартуем с 14 пикселя, чтобы не затереть заголовок
    int y = 10 + (i * 10);

    // 1. Рисуем белую полосу препятствия СВЕРХУ СПРАВА (заполняет экран справа налево)
    if (barWidth > 0)
    {
      // Отрисовка заполненного прямоугольника: x, y, width, height
      // Стартуем из точки (128 - barWidth), чтобы прижиматься к правому краю
      disp.fillRect(disp.SCREEN_WIDTH - barWidth, y, barWidth, 7, WHITE);
    }
  }
  // 2. Печатаем схематично робота
  disp.drawText("*", 0, 30);
}
unsigned long lastToggleTime = 0;
bool isGrafic = false;
void StateStartV2::radar_Test()
{
  Action a = menu->getAction();
  if (a == NEXT || a == PREV)
    isGrafic = !isGrafic;
  if (a == OK)
  {
    radar.scan();
  }
  if (millis() - lastToggleTime > 1000)
  {
    display->clear();
    lastToggleTime = millis();

    display->drawText(" -\\\\RADAR//-", 0, 0, 1);

    if (!radar.IsScan())
      for (int i = 0; i < 5; i++)
      {
        radar.data[i] = radar.GetData(i);
        char buffer1[32];
        sprintf(buffer1, "Замер №-%d- = %d mm", i, radar.data[i]);
        Serial.println(buffer1);
        if (!isGrafic)
        {
          float tmp = radar.data[i];
          sprintf(buffer1, "Ang%d'-%.2fcm", i * 45, (tmp / 10));
          display->drawText(buffer1, 0, 10 + 10 * i);
        }
      }

    if (isGrafic)
      DrawRadar(radar, *display);
    display->NeedUpdate = true;
  }
}

char buffers[5][32];
void scanI2C()
{
  Serial.println("\n[I2C] Запуск сканирования шины...");
  byte error, address;
  int nDevices = 0;
  char buffer1[32];

  for (address = 1; address < 127; address++)
  {
    // Начинаем передачу по адресу
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("[I2C] Найден девайс на адресе: 0x");
      
      if (nDevices < 5)
      {
        sprintf(buffers[nDevices], "[I2C] %d: 0x%02X", nDevices, address);
      }
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("[I2C] Неизвестная ошибка на адресе: 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0)
  {
    Serial.println("[I2C] Устройства не найдены.\n");
  }
  else
  {
    Serial.println("[I2C] Сканирование завершено.\n");
  }
}
void StateStartV2::I2C_Scaner()
{
  Action a = menu->getAction();
  // if (a == NEXT || a == PREV)
  if (a == OK)
  {
    scanI2C();
  }
  if (millis() - lastToggleTime > 1000)
  {
    display->clear();
    lastToggleTime = millis();

    display->drawText(" -\\\\I2C Scaner//-", 0, 0, 1);

    for(int i=0; i<5;i++)
      display->drawText(buffers[i],0,10 + i*10,1);

    display->NeedUpdate = true;
  }
}

// float dis = 0;
// void StateStartV2::Tof()
// {
//   lox.rangingTest(&measure, false);

//   if (measure.RangeStatus != 4)
//   {
//     dis = ((float)(measure.RangeMilliMeter-25))/10;
//     Serial.print("Distanse:");
//     Serial.print(dis);
//     Serial.println("Cm");
//   }
//   else
//     Serial.println("ERR");
// }

void StateStartV2::One_Measure_Test()
{
  Action a = menu->getAction();

  if (a == OK)
  {
    radar.OneСornerMeasure(2);
    // Tof();
  }

  if (millis() - lastToggleTime > 1000)
  {
    display->clear();
    lastToggleTime = millis();

    display->drawText(" -\\One Measure Test/-", 0, 0, 1);
    if (radar.IsOneСornerMeasureRedy)
    {
      char buffer1[32];
      sprintf(buffer1, "Measure: %.2fcm", ((float)radar.Measure / 10));
      display->drawText(buffer1, 0, 30);
    }

    display->NeedUpdate = true;
  }
}

void StateStartV2::DrawClock()
{
  if (millis() - lastToggleTime > 1000)
  {
    display->clear();
    lastToggleTime = millis();

    _time = RTCModule::getInstance().getTime();
    char buffer[9]; // "HH:MM"

    auto second = _time.second();
    if (second % 2 == 0)
      sprintf(buffer, "%02d:%02d", _time.hour(), _time.minute());
    else
      sprintf(buffer, "%02d %02d", _time.hour(), _time.minute());

    display->drawText(buffer, 0, 0, 4);
    sprintf(buffer, "%02d/%02d", _time.day(), _time.month());
    display->drawText(buffer, 0, 32, 4);
    display->NeedUpdate = true;
  }
}