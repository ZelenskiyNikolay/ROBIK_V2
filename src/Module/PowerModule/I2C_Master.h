#pragma once
#include <Arduino.h>
#include <Wire.h>

#define KP_I2C_ADDR 0x25 // Адрес нашего КП-слейва

enum SoundEffect
{
  FX_NONE,
  FX_WELCOME,
  FX_SHUTDOWN,
  FX_CHARGING, // Звук подключения питания
  FX_NO_CHARGING,
  FX_FULL_CHARGING,
  FX_MENU_CLICK, // Короткий клик меню
  FX_MENU_CLICK_Q,
  FX_ERROR, // Ошибка / Низкий заряд
  FX_R2D2   // Твой старый добрый рандом
};
//Для кралоты управления
const bool ENABLED = true;
const bool DISABLED = false;
// Адреса регистров (должны совпадать с КП)
const byte REG_BAT_LEVEL = 0x01;
const byte REG_SYS_CONTROL = 0x02;
const byte REG_SYS_STATUS = 0x03;
const byte REG_I2C_SOUND = 0x05;
const byte REG_SOUND_FX = 0x06;
const byte REG_SOUND_R2D2 = 0x07;

enum Status_Register
{
  IN_POWER = 0,
  CHARGE,
  FULL
};

inline const char *getSoundEffectName(SoundEffect fx)
{
  switch (fx)
  {
  case FX_NONE:
    return "FX_NONE";
  case FX_WELCOME:
    return "FX_WELCOME";
  case FX_SHUTDOWN:
    return "FX_SHUTDOWN";
  case FX_CHARGING:
    return "FX_CHARGING";
  case FX_NO_CHARGING:
    return "FX_NO_CHARGING";
  case FX_FULL_CHARGING:
    return "FX_FULL_CHARGING";
  case FX_MENU_CLICK:
    return "FX_MENU_CLICK";
  case FX_MENU_CLICK_Q:
    return "FX_CLICK_Quiet";
  case FX_ERROR:
    return "FX_ERROR";
  case FX_R2D2:
    return "FX_R2D2";
  default:
    return "UNKNOWN";
  }
}
inline void writeBit(byte &Byte_var, uint8_t bitNumber, bool value)
{
  if (value)
  {
    Byte_var |= (1 << bitNumber);
  }
  else
  {
    Byte_var &= ~(1 << bitNumber);
  }
}

inline bool getBit(byte &Byte_var, uint8_t bitNumber)
{
  if (bitNumber > 7)
    return false;

  return (Byte_var & (1 << bitNumber)) != 0;
}

// --- ФУНКЦИЯ ЗАПИСИ В РЕГИСТР ---
inline void writeRegister(byte regAddr, byte value)
{
  Wire.beginTransmission(KP_I2C_ADDR); // Зовем КП
  Wire.write(regAddr);                 // Выбираем ящик
  Wire.write(value);                   // Кладем данные
  Wire.endTransmission();              // Хлопаем дверью
}

// --- ФУНКЦИЯ ЧТЕНИЯ ИЗ РЕГИСТРА ---
inline byte readRegister(byte regAddr)
{
  // Этап 1: Говорим КП, какой регистр нас интересует
  Wire.beginTransmission(KP_I2C_ADDR);
  Wire.write(regAddr);
  Wire.endTransmission(false); // false означает Repeated Start (не отпускаем шину)

  // Этап 2: Запрашиваем 1 байт данных
  Wire.requestFrom(KP_I2C_ADDR, 1);

  if (Wire.available())
  {
    return Wire.read(); // Возвращаем то, что прислал КП
  }

  return 0; // Если КП не ответил
}
inline void phaseControl(bool pico, bool sensor = DISABLED, bool sound = DISABLED, bool move = DISABLED)
{
    byte cb = 255;
    writeBit(cb, 1, !pico);
    writeBit(cb, 2, !sensor);
    writeBit(cb, 3, !sound);
    writeBit(cb, 4, !move);
    writeRegister(REG_SYS_CONTROL, cb);
}