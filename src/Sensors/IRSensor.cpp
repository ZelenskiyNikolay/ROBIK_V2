#include "IRSensor.h"
#include <IRremote.hpp>

IRSensor::IRSensor() {}

void IRSensor::begin(int pin)
{
    SensorPin = pin;
    pinMode(SensorPin, INPUT);
    IrReceiver.begin(SensorPin);
}
void IRSensor::update()
{

    if (IrReceiver.decode())
    {
        uint16_t cmd = IrReceiver.decodedIRData.command;

        if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)
        {
            IrReceiver.resume();
            return;
        }

        switch (cmd)
        {
        case 0x40: // ОК
            lastState = ButtonIR::ButtonOk;
            break;
        case 0x15: // Вниз
            lastState = ButtonIR::ButtonDown;
            break;
        case 0x46: // Вверх
            lastState = ButtonIR::ButtonUp;
            break;
        case 0x43: // Вправо
            lastState = ButtonIR::ButtonRight;
            break;
        case 0x44: // Влево
            lastState = ButtonIR::ButtonLeft;
            break;

        case 0x16: // кнопка 1
            lastState = ButtonIR::Button1;
            break;

        case 0x19: // кнопка 2
            lastState = ButtonIR::Button2;
            break;

        case 0x0D: // кнопка 3
            lastState = ButtonIR::Button3;
            break;

        case 0xC: // кнопка 4
            lastState = ButtonIR::Button4;
            break;

        case 0x18: // кнопка 5
            lastState = ButtonIR::Button5;
            break;

        case 0x5E: // кнопка 6
            lastState = ButtonIR::Button6;
            break;

        case 0x8: // кнопка 7
            lastState = ButtonIR::Button7;
            break;

        case 0x1C: // кнопка 8
            lastState = ButtonIR::Button8;
            break;

        case 0x5A: // кнопка 9
            lastState = ButtonIR::Button9;
            break;

        case 0x52: // кнопка 0
            lastState = ButtonIR::Button0;
            break;
        case 0x42: // Звездочка
            lastState = ButtonIR::ButtonStar;
            break;
        case 0x4A: // Решетка
            lastState = ButtonIR::ButtonHash;
            break;
        default:
            Serial.print("ИК код: 0х");
            Serial.println(cmd, HEX);
            break;
            // и т.д.
        }
        IrReceiver.resume();
    }
}
ButtonIR IRSensor::GetSensorState()
{
    ButtonIR tmp = lastState;
    lastState = ButtonIR::NOOL;
    return tmp;
}