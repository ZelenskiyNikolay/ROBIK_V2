#include "VoiceControl.h"

VoiceControl::VoiceControl() {}

void VoiceControl::begin()
{
    memset(sample_buffer, 0, sizeof(sample_buffer));
}

void VoiceControl::Count_MAE(int16_t *buffer, size_t size)
{
    MAE_redy = false;
    uint32_t values = 0;
    uint16_t samples_count = 0;
    for (size_t i = 0; i < size; i++)
    {
        values += abs(buffer[i]);
        samples_count++;
    }
    MAE[MAE_count] = (uint16_t)(values / samples_count);
    MAE_count++;
}

void VoiceControl::Record_Comand()
{
    Serial.println("Start record Comand ");
    // 2. Сбрасываем мост
    recordBridge.readyA = false;
    recordBridge.readyB = false;
    recordBridge.totalSamples = 0;

    // 3. Задаем лимит и пускаем Ядро 1
    recordBridge.targetSamples = 16000; // Запуск записина 1 сек
    recordBridge.isRecord = true;
    writePoint = 0;
    _wasRecording = true;
}
void VoiceControl::Update()
{
    if (!_wasRecording)
        return;

    if (recordBridge.readyA)
    {
        processBuffer(recordBridge.bufferA);
        Count_MAE(recordBridge.bufferA, RecordBridge::BUF_SIZE);
        colect(recordBridge.bufferA);
        recordBridge.readyA = false;
    }

    if (recordBridge.readyB)
    {
        processBuffer(recordBridge.bufferB);
        Count_MAE(recordBridge.bufferB, RecordBridge::BUF_SIZE);
        colect(recordBridge.bufferB);
        recordBridge.readyB = false;
    }
    if (!recordBridge.isRecord && !recordBridge.readyA && !recordBridge.readyB)
    { // запись сохранена в буфер
        if (_wasRecording)
        {
            _wasRecording = false;
            Comand_Ready = true;
        }
    }
    if (Comand_Ready)
    {
        
        Comand_Ready = false;
        Serial.println("Запись закончена....");
    }
}
void VoiceControl::colect(int16_t *buffer)
{
    for (size_t i = 0; i < recordBridge.BUF_SIZE; i++)
    {
        sample_buffer[writePoint] = buffer[i];
        writePoint++;
        if (writePoint >= SAMPLE_COUNT)
            return;
    }
}

void VoiceControl::processBuffer(int16_t *buffer)
{
    for (size_t i = 0; i < recordBridge.BUF_SIZE; i++)
    {
        int32_t sample = buffer[i];
        // Вырезаем шум
        if (abs(sample) < _threshold)
        {
            sample = sample >> 2; // Зануляем шум /4
        }
        else
        {
            sample = sample << 2; //*4
        }

        if (sample > 32767)
            sample = 32767;
        if (sample < -32768)
            sample = -32768;
        buffer[i] = (int16_t)sample;
    }
}