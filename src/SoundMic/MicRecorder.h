#pragma once
#include <Arduino.h>
#include "hardware/adc.h"
#include "hardware/irq.h"
#include "RecordBridge.h"

static uint32_t idx = 0;
static bool recordA = true;

class MicRecorder
{
public:
    static MicRecorder &getInstance()
    {
        static MicRecorder instance;
        return instance;
    }

    // Вызывать один раз на Ядре 1 при старте системы
    void Init(int adc_pin);

    static void on_adc_fifo_irq();

private:
    MicRecorder() {}
    void handle_interrupt();
    int _adc_input;
};