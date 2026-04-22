#include "Sensors/Ultrasonic.h"

void ultrasonic_program_init(PIO pio, uint sm, uint offset, uint trig_pin, uint echo_pin)
{
    pio_sm_config c = ultrasonic_program_get_default_config(offset);

    // Trig на выход (используем SET в ассемблере)
    sm_config_set_set_pins(&c, trig_pin, 1);
    pio_gpio_init(pio, trig_pin);
    pio_sm_set_consecutive_pindirs(pio, sm, trig_pin, 1, true);

    // Echo на вход (используем WAIT и JMP PIN)
    sm_config_set_in_pins(&c, echo_pin);
    sm_config_set_jmp_pin(&c, echo_pin); // БЕЗ ЭТОГО JMP PIN НЕ СРАБОТАЕТ
    pio_gpio_init(pio, echo_pin);
    pio_sm_set_consecutive_pindirs(pio, sm, echo_pin, 1, false);

    // Устанавливаем частоту PIO 1 МГц (1 такт = 1 мкс)
    // Это удобно: ticks в FIFO будут сразу микросекундами
    float div = (float)clock_get_hz(clk_sys) / 1000000;
    sm_config_set_clkdiv(&c, div);

    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
}

Ultrasonic::Ultrasonic(PIO pio_hw, uint trig_pin, uint echo_pin)
{
    pio = pio_hw;
    // Загружаем программу в память PIO
    offset = pio_add_program(pio, &ultrasonic_program);
    sm = pio_claim_unused_sm(pio, true);

    // Конфигурируем пины (Trig - выход, Echo - вход)
    ultrasonic_program_init(pio, sm, offset, trig_pin, echo_pin);
}

float Ultrasonic::getDistance()
{
    if (pio_sm_is_rx_fifo_empty(pio, sm))
    {
        return -1.0f; // Данные еще не готовы
    }

    // Читаем сырые тики из FIFO
    uint32_t ticks = pio_sm_get(pio, sm);

    // Пересчитываем тики в сантиметры (зависит от частоты PIO)
    // Дистанция = (время * скорость звука) / 2
    float distance = ticks * 2.0f * 0.017f;
    if (distance > 350.0f)
        distance = 350.0f;
    return distance;
}
void Ultrasonic::TriggerUltrasonic() { pio_sm_put(pio, sm, 1); /* триггер измерения*/ }
float Ultrasonic::DistanceUltrasonic(){ return _last_dis;}

void Ultrasonic::update(float dt)
{
    timer -= dt;
    if (timer > 0)
        return;

    timer = UP_TIME;

    if (!busy)
    {
        TriggerUltrasonic();
        busy = true;
    }

    float d = getDistance();
    if (d >= 0)
    {
        _last_dis = d;
        busy = false;
    }
}