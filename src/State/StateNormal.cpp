#include "StateNormal.h"

StateNormal::StateNormal(DisplayOled &disp)
    : display(&disp), sprite(&disp)
{
}

void StateNormal::enter()
{
    display->clear();
    timer = 0;
    isDrawingBattery = true;

    VoiceControl::getInstance().begin();
}
void StateNormal::update(float dt)
{
    IrLogic();
    Draw(dt);
    VoiceControl::getInstance().Update();
}


uint8_t temp[SAMPLE_COUNT];

bool HiSpeed = true;
void StateNormal::IrLogic()
{
    ButtonIR tmp = IRSensor::getInstance().GetSensorState();
    switch (tmp)
    {
    case Button1:
        SoundManager::getInstance().Play("Sound/Hello/Dima.wav");
        break;
    case Button2:
        SoundManager::getInstance().Play("Sound/Hello/Aliona.wav");
        break;
    case Button3:
        SoundManager::getInstance().Play("Sound/Hello/Mama.wav");
        break;
    case ButtonHash:
        EventBus::push({EVENT_CHANGE_STATE, STATE_START});
        break;

    case Button4:
        MovementModule::getInstance().NewMov(MotionState::TURN_LEFT);
        break;
    case Button6:
        MovementModule::getInstance().NewMov(MotionState::TURN_RIGHT);
        break;

    case Button0:
        HiSpeed = !HiSpeed;
        MovementModule::getInstance().MoveSpeed(HiSpeed);
        break;

    case ButtonUp:
        MovementModule::getInstance().NewMov(MotionState::FORWARD, 0.5f, 0.5f);
        break;
    case ButtonDown:
        MovementModule::getInstance().NewMov(MotionState::BACKWARD, 0.5f, 0.5f);
        break;
    case ButtonLeft:
        MovementModule::getInstance().NewMov(MotionState::TURN_LEFT90);
        break;
    case ButtonRight:
        MovementModule::getInstance().NewMov(MotionState::TURN_RIGHT90);
        break;

    case ButtonOk:
        VoiceControl::getInstance().Record_Comand();
        break;
    case ButtonStar:
        draw_MAE = !draw_MAE;
        if (draw_MAE)
        {
            Serial.print("MAE: ");
            for (int i = 0; i < MAE_BUF_SIZE; i++)
            {
                Serial.print(VoiceControl::getInstance().MAE[i]);
                Serial.print(", ");
            }
            Serial.println("... END MAE");

            for (int i = 0; i < SAMPLE_COUNT; i++)
            {
                temp[i] = static_cast<uint8_t>((VoiceControl::getInstance().sample_buffer[i] + 32768) >> 8);
            }
            SoundManager::getInstance().Play(temp, SAMPLE_COUNT);
        }
        break;

    default:
        break;
    }
}

void StateNormal::Draw(float dt)
{
    timer -= dt;
    if (IsOpen)
    {
        if (timer < 0)
        {
            IsOpen = false;
            timer = Close_Eyes;
            sprite.Draw(Emotions::BLINK);
            return;
        }
    }
    else
    {
        if (timer < 0)
        {
            IsOpen = true;
            timer = Open_Eyes;

            sprite.Draw(Emotions::NORMAL);
            return;
        }
    }
}
