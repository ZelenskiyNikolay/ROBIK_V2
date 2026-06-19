#include "BatteryModule.h"

void BatteryModule::begin()
{
    voltage = 0;
}

void BatteryModule::update(float dt)
{

    timer -= dt;

    if (timer > 0)
        return; // обновление раз в 20 секунд

    timer = timeUpdate;

    byte tmp = readRegister(REG_BAT_LEVEL);
    voltage = tmp / 50.0f;

    if (voltage < 1.0f)
    {
        Serial.println("Battery: Not connect or Power Off.....");
    }
    else
    {
        Serial.print("Battery: ");
        Serial.print(voltage);
        Serial.println(" V");
    }
}

bool BatteryModule::IsChargeConect() const
{
    byte tmp = readRegister(REG_SYS_STATUS);

    bool c  = getBit(tmp, CHARGE);
    return c;
}

float BatteryModule::getVoltage() const
{
    return voltage;
}
int BatteryModule::getBatteryPercent()
{

    float voltage = getVoltage();

    // Адаптируй под свой делитель!
    if (voltage > 4.0f)
        return 100;
    if (voltage < 3.40f)
        return 0;

    // return map(voltage * 100, 340, 420, 0, 100);
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