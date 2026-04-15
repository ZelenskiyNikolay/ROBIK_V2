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
#define UP_TIME 1000
float logic_timer = 0;
VoiceCmd VoiceComand;
bool New_Comand = false;

void StateNormal::IdelProcess(float dt)
{
    if (MovementModule::getInstance().isBusy() || SoundManager::getInstance().Is_Playing())
        idelTimer = IDEL_TIMER;
    idelTimer -= dt;
    if (idelTimer < 0)
        EventBus::push({EVENT_CHANGE_STATE, STATE_IDEL});
}

void StateNormal::update(float dt)
{
    IdelProcess(dt);
    MovementModule::getInstance().update(dt);
    if (VoiceControl::getInstance().Is_New_Comand)
        microphone = false;
    IrLogic();
    Draw(dt);
    VoiceControl::getInstance().Update();
    StateLogic(dt);
    if (New_Comand)
    {
        if (VoiceComand == HELLO)
        {
            char filename[64];
            snprintf(filename, sizeof(filename), "Sound/Hello/var%lu.wav", random(1, 6));

            SoundManager::getInstance().Play(filename);
        }
        else if (VoiceComand == TIME)
        {
            New_Comand = false;
            EventBus::push({EVENT_CHANGE_STATE, STATE_CLOCK});
        }

        microphone = false;
        New_Comand = false;
    }
}

void StateNormal::StateLogic(float dt)
{
    logic_timer -= dt;
    if (logic_timer < 0)
    {
        logic_timer = UP_TIME;
        if (VoiceControl::getInstance().Is_New_Comand)
        {
            VoiceComand = VoiceControl::getInstance().Get_Voice_Comand();
            New_Comand = true;
        }
    }
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

    case Button5:
        SoundManager::getInstance().Random_Play("Hello");
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
        timer = 0;
        microphone = true;
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
            memset(VoiceControl::getInstance().MAE, 0, sizeof(VoiceControl::getInstance().MAE));
        }
        break;

    default:
        break;
    }
}

void StateNormal::Draw(float dt)
{
    timer -= dt;
    if (timer > 0)
        return;

    if (microphone)
    {
        timer = 100;
        sprite.Draw(Emotions::MICROPHONE);
        return;
    }

    if (IsOpen)
    {
        IsOpen = false;
        timer = Close_Eyes;
        sprite.Draw(Emotions::BLINK);
        return;
    }
    else
    {
        IsOpen = true;
        timer = Open_Eyes;

        sprite.Draw(Emotions::NORMAL);
        return;
    }
}
