#include "BatteryModule.h"

void BatteryModule::begin(uint8_t pin, uint8_t charge_pin)
{
    this->pin = pin;
    this->charge_pin = charge_pin;
    pinMode(charge_pin, INPUT_PULLDOWN);
    adc_gpio_init(pin);
    analogReadResolution(12);
}

void BatteryModule::update(float dt)
{

    timer -= dt;

    if (timer > 0)
        return; // обновление раз в 20 секунд

    timer = timeUpdate;

    adc_run(false); // 1. Ставим микрофон на паузу

    // Определяем канал (пин 26 = 0, 27 = 1 и т.д.)
    adc_select_input(this->pin - 26); // 2. Явно переключаемся на АКБ

    long sum = 0;
    adc_read(); // 3. Холостой замер (сливаем остатки звука)
    for (int i = 0; i < 16; i++)
    { // берем 16 замеров подряд
        sum += adc_read();
    }
    int raw = sum / 16;

    adc_select_input(1); // 5. ВОЗВРАЩАЕМ АЦП микрофону (пин 27)
    adc_run(true);       // 6. Снова запускаем микрофонный конвейер
    // ------------------------------------------

    voltage = (raw / 4095.0f) * 3.3f * 2.0f;

    // if (voltage < 0.1f || IsChargeConect())
    // {
     //   voltage = new_voltage;
    // }
    // else
    // {
    //     voltage = (voltage * 0.9f) + (new_voltage * 0.1f);
    // }

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

    // int raw = analogRead(pin);

    // voltage = (raw / 4095.0f) * 3.3f * 2.0f; // измеренное напряжение

    // // voltage *= 0.95f; //калибровка

    // if (voltage < 1.0f)
    // {
    //     Serial.println("Battery: Not conect or Power Off.....");
    // }
    // else
    // {
    //     Serial.print("Battery: ");
    //     Serial.print(getVoltage()); // Печать значения в порт
    //     Serial.println(" V");
    // }
}

bool BatteryModule::IsChargeConect() const
{
    return digitalRead(charge_pin);
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