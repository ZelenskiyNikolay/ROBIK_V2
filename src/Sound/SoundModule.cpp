#include "SoundModule.h"

SoundModule::SoundModule() {}

// Статический оберточный метод
void SoundModule::on_pwm_irq()
{
    SoundModule::getInstance().handle_interrupt();
}

void SoundModule::Init()
{
    pinMode(_audio_en_pin, OUTPUT);
    digitalWrite(_audio_en_pin, HIGH); // Выключен

    gpio_set_function(_audio_pin, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(_audio_pin);

    // Настройка несущей частоты ШИМ ~62.5 кГц (чтобы ухо не слышало писка)
    pwm_set_wrap(slice_num, 255);
    pwm_set_clkdiv(slice_num, 30.517f); // Делитель для стабильности

    // Включаем прерывания
    pwm_set_irq_enabled(slice_num, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_irq);
    irq_set_enabled(PWM_IRQ_WRAP, true);

    pwm_set_enabled(slice_num, true);

    audioBridge.En_Pin = _audio_en_pin;
}
void SoundModule::Init(int Audio_pin, int en_pin)
{
    _audio_en_pin = en_pin;
    _audio_pin = Audio_pin;
    Init();
}

void SoundModule::handle_interrupt()
{
    pwm_clear_irq(slice_num);
    if (!audioBridge.isPlaying)
        return;

    static uint32_t idx = 0;
    static bool playA = true;

    uint8_t sample = 128; // Тишина

    if (playA)
    {
        if (audioBridge.readyA)
        {
            sample = audioBridge.bufferA[idx++];
            if (idx >= AudioBridge::BUF_SIZE)
            {
                idx = 0;
                audioBridge.readyA = false; // Сигнал для Core 0
                playA = false;              // Переключаемся на B
            }
        }
    }
    else
    {
        if (audioBridge.readyB)
        {
            sample = audioBridge.bufferB[idx++];
            if (idx >= AudioBridge::BUF_SIZE)
            {
                idx = 0;
                audioBridge.readyB = false; // Сигнал для Core 0
                playA = true;               // Переключаемся на A
            }
        }
    }

    pwm_set_gpio_level(_audio_pin, sample);
}
