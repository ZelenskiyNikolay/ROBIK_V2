#include "MicRecorder.h"

RecordBridge recordBridge;

void MicRecorder::on_adc_fifo_irq() {
    MicRecorder::getInstance().handle_interrupt();
}

void MicRecorder::Init(int adc_pin) {
    _adc_input = adc_pin - 26;
    adc_init();
    adc_gpio_init(adc_pin);
    adc_select_input(_adc_input);

    adc_fifo_setup(true, false, 1, false, false);
    adc_set_clkdiv(2999); // 16 кГц

    irq_set_exclusive_handler(ADC_IRQ_FIFO, on_adc_fifo_irq);
    irq_set_enabled(ADC_IRQ_FIFO, true);
    adc_irq_set_enabled(true);

    adc_run(true); // АЦП молотит ВСЕГДА, прерывание само разберется
}

void MicRecorder::handle_interrupt() {
    // Если в FIFO пусто — выходим сразу
    if (adc_fifo_is_empty()) return;

    // Читаем сэмпл в любом случае, чтобы очистить FIFO
    uint16_t raw_sample = adc_fifo_get();

    // ГЛАВНОЕ: Проверка флага и счетчика из моста
    if (!recordBridge.isRecord || recordBridge.targetSamples == 0) {
        return; 
    }

    int16_t sample = (int16_t)((raw_sample - 2048) << 4);

    static uint32_t idx = 0;
    static bool recordA = true;

    if (recordA) {
        if (!recordBridge.readyA) {
            recordBridge.bufferA[idx++] = sample;
            if (idx >= RecordBridge::BUF_SIZE) {
                idx = 0;
                recordBridge.readyA = true; // Маяк для Core 0
                recordA = false;
            }
        }
    } else {
        if (!recordBridge.readyB) {
            recordBridge.bufferB[idx++] = sample;
            if (idx >= RecordBridge::BUF_SIZE) {
                idx = 0;
                recordBridge.readyB = true; // Маяк для Core 0
                recordA = true;
            }
        }
    }

    // Автоматический декремент — когда станет 0, запись сама "заглохнет"
    recordBridge.targetSamples--;
    
    // Если это был последний сэмпл — сбрасываем флаг активности
    if (recordBridge.targetSamples == 0) {
        recordBridge.isRecord = false;
    }
}