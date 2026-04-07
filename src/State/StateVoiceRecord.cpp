#include "StateVoiceRecord.h"

StateVoiceRecord::StateVoiceRecord(DisplayOled &disp)
    : display(&disp), sprite(&disp), mySDStream(), recorder(mySDStream)
{
}

void StateVoiceRecord::enter()
{
    display->clear();
    timer = 0;
}
bool _wasRecording;
void StateVoiceRecord::update(float dt)
{
    if (IsRecording)
    {
        RecTime -= dt;
        if (RecTime < 0)
        {
            IsRecording = false;
            RecTime = 10000;
        }
    }
    // Если запись активна
    if (recordBridge.isRecord || recordBridge.readyA || recordBridge.readyB)
    {
        recorder.tick(); // Скидываем буферы, пока они прилетают
    }
    else if (_wasRecording) // Если запись только что закончилась (флаг упал)
    {
        recorder.stop(); // Финализируем WAV заголовок
        _wasRecording = false;
        Serial.println("WAV Save Complete");
    }

    IrLogic();

    if (draw_MAE)
        Draw_MAE(dt);
    else
        Draw(dt);
}

#define MAE_PRINT_UPDATE 500
void StateVoiceRecord::Draw_MAE(float dt)
{
    if (!recorder.IS_MAE_READY())
        return;
    timer -= dt;
    if (timer > 0)
        return;
    timer = MAE_PRINT_UPDATE;

    display->clear();
    display->drawText("MAE Level:", 0, 0, 1);

    int X = 0;
    for (int i = 0; i < MAE_BUF_SIZE; i++)
    {
        int h = recorder.MAE[i] / 100;
        if (h > 50)
            h = 50;
        if (h < 1)
            h = 1;
        display->drawLine(X, 64, X, 64 - h);
        X++;
    }
    display->drawLine(0, 13, 81, 13);
    display->drawLine(81, 13, 81, 64);
}
void StateVoiceRecord::Draw(float dt)
{
    timer -= dt;
    if (timer > 0)
        return;

    timer = DISPLAY_UP_TIME;
    if (!IsRecording)
    {
        display->clear();

        display->drawText("Voice ", 0, 0, 2);
        display->drawText("  Recorder", 0, 20, 2);
        display->drawText("press * for MAE print", 0, 40, 1);
        display->drawText("press OK for record!", 0, 48, 1);
        display->drawText("press # for exit.", 0, 56, 1);
    }
    else
    {
        display->clear();
        display->drawText("   Speak ", 0, 0, 2);
        display->drawText("Record!", 0, 25, 3);
    }
}
void StateVoiceRecord::IrLogic()
{
    ButtonIR tmp = IRSensor::getInstance().GetSensorState();
    switch (tmp)
    {
    case ButtonOk:
        IsRecording = true;
        timer = 0;

        char filename[32];
        snprintf(filename, sizeof(filename), "/rec%lu.wav", millis());

        // 1. Сначала подготавливаем файл
        if (recorder.start(filename, 16000))
        {
            // 2. Сбрасываем мост
            recordBridge.readyA = false;
            recordBridge.readyB = false;
            recordBridge.totalSamples = 0;

            // 3. Задаем лимит и пускаем Ядро 1
            recordBridge.targetSamples = 160000; // Твоя четкая размерность
            recordBridge.isRecord = true;

            _wasRecording = true;
            Serial.printf("Start capture to: %s\n", filename);
        }
        break;
    case ButtonStar:
        draw_MAE = !draw_MAE;

        Serial.print("MAE: ");
        for (int i = 0; i < MAE_BUF_SIZE; i++)
        {
            Serial.print(recorder.MAE[i]);
            Serial.print(", ");
        }
        Serial.println("... END MAE");

        break;
        // Serial.println("Буфер А:");
        // for (int i = 0; i < 2048; i++)
        // {
        //     Serial.print(recordBridge.bufferA[i]);
        //     Serial.print(", ");
        // }

        // Serial.println();

        // Serial.println("Буфер B:");
        // for (int i = 0; i < 2048; i++)
        // {
        //     Serial.print(recordBridge.bufferB[i]);
        //     Serial.print(", ");
        // }

    case ButtonHash:
        EventBus::push({EVENT_CHANGE_STATE, STATE_START});
        break;

    default:
        break;
    }
}