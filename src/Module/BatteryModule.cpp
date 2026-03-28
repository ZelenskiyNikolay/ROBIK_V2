#include "BatteryModule.h"

void BatteryModule::begin(uint8_t pin) {
    this->pin = pin;
    pinMode(pin, INPUT);
    analogReadResolution(12);
}

void BatteryModule::update(float dt) {

    timer -= dt;

    if (timer>0) return;  // обновление раз в 20 секунд

    timer = timeUpdate;

    int raw = analogRead(pin);
    voltage = (raw / 4095.0f) * 3.3f * 2.0f;  // измеренное напряжение

    //voltage *= 0.95f; //калибровка

    Serial.print("Battery: ");
    Serial.print(getVoltage());//Печать значения в порт
    Serial.println(" V");
}

float BatteryModule::getVoltage() const {
    return voltage;
}
int BatteryModule::getBatteryPercent()
{
    
    float voltage = getVoltage();

    // Адаптируй под свой делитель!
    if (voltage > 4.1f) return 100;
    if (voltage < 3.40f) return 0;

    //return map(voltage * 100, 340, 420, 0, 100);
    float percent = (voltage - 3.40f) / (4.15f - 3.40f) * 100.0f;
    return (int)constrain(percent, 0, 100);
}
void BatteryModule::drawBatteryIcon(DisplayOled &display, int x, int y, int percent)
{
    // Размеры батареи
    const int bodyWidth = 96;
    const int bodyHeight = 20;

    // Корпус
    display.drawRect(x, y, bodyWidth, bodyHeight, WHITE);

    // Клемма справа
    display.fillRect(x + bodyWidth, y + 3, 2, bodyHeight - 6, WHITE);

    // Заполнение (внутри 22×8)
    percent = constrain(percent, 0, 100);
    int fillWidth = map(percent, 0, 100, 0, bodyWidth - 2);

    display.fillRect(x + 1, y + 1, fillWidth, bodyHeight - 2, WHITE);
}