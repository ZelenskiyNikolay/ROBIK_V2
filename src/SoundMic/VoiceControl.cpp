#include "VoiceControl.h"
#include "edge-impulse-sdk/classifier/ei_run_classifier.h"
#include "model-parameters/model_metadata.h"

VoiceControl::VoiceControl() {}

void VoiceControl::begin()
{
    memset(sample_buffer, 0, sizeof(sample_buffer));
}

static int get_audio_data(size_t offset, size_t length, float *out_ptr)
{
    for (size_t i = 0; i < length; i++)
    {
        out_ptr[i] = (float)VoiceControl::getInstance().sample_buffer[offset + i];
    }
    return 0;
}

VoiceCmd VoiceControl::Get_Voice_Comand()
{
    if (!Is_New_Comand)
        return VoiceCmd::UNKNOWN;

    VoiceCmd temp = lastCommand;
    lastCommand = NONE;
    Is_New_Comand = false;
    return temp;
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
        Serial.println("Запись закончена. Начинаю распознавание...");

        // Настройка сигнала для Edge Impulse
        signal_t signal;
        signal.total_length = SAMPLE_COUNT; // Твои 16000
        signal.get_data = &get_audio_data;

        ei_impulse_result_t result = {0};

        // ЗАПУСК НЕЙРОНКИ
        EI_IMPULSE_ERROR res = run_classifier(&signal, &result, false);

        if (res == EI_IMPULSE_OK)
        {
            float confidence = 0.0f;
            int best_index = -1;

            // Ищем команду с максимальным весом
            for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++)
            {
                if (result.classification[ix].value > confidence)
                {
                    confidence = result.classification[ix].value;
                    best_index = ix;
                }
            }

            // Если уверенность выше 80%, сохраняем команду
            if (best_index != -1 && confidence > 0.80f)
            {
                const char *comand = result.classification[best_index].label;
                Serial.printf("Команда поймана: %s (уверенность %.2f)\n", comand, confidence);

                switch (best_index)
                {
                case 0: // background
                case 4: // silence
                    lastCommand = NONE;
                    break;
                case 1: // hello
                    lastCommand = HELLO;
                    break;
                case 2: // home
                    lastCommand = HOME;
                    break;
                case 3: // quiet
                    lastCommand = SILENCE;
                    break;
                case 5: // time
                    lastCommand = TIME;
                    break;
                case 6: // wakeUp
                    lastCommand = WAKE_UP;
                    break;
                default:
                    lastCommand = UNKNOWN;
                    break;
                }
                Is_New_Comand = true;
            }
            else
            {
                lastCommand = UNKNOWN;
                Is_New_Comand = false;
                Serial.println("Команда не распознана (низкая уверенность)");
            }
        }
        else
        {
            Is_New_Comand = false;
            Serial.printf("Ошибка классификатора: %d\n", res);
        }
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