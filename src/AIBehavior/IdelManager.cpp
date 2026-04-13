#include "IdelManager.h"

IdelManager::IdelManager() {}

void IdelManager::begin()
{
    timer1 = TIMER_MOV;
    timer2 = TIMER_SPEAK;
}

void IdelManager::Update(float dt)
{
    if (!IsIdel())
    {
        timer1 = TIMER_MOV;
        timer2 = TIMER_SPEAK;
        return;
    }

    timer1 -= dt;
    timer2 -= dt;

    if (timer1 < 0)
    {
        Serial.println("Событие IDEL MOV произошло !!!");
        timer1 = random(TIMER_MOV * 0.3, TIMER_MOV * 2);
        Serial.print(" Следующее через :");
        Serial.print(timer1 / 1000);
        Serial.println(" секунд.");
        
        int action = random(0, 6);
        float tmp;
        switch (action)
        {
        case 0: // f
            tmp = RND_DIST();
            Serial.print("Движение вперед на оборота: ");
            Serial.println(tmp);
            MovementModule::getInstance().NewMov(MotionState::FORWARD, tmp, tmp);
            break;
        case 1: // b
            tmp = RND_DIST();
            Serial.print("Движение назад на оборота: ");
            Serial.println(tmp);
            MovementModule::getInstance().NewMov(MotionState::BACKWARD, tmp, tmp);
            break;
        case 2: // L
            tmp = RND_DIST();
            Serial.print("Движение влево на оборота: ");
            Serial.println(tmp);
            MovementModule::getInstance().NewMov(MotionState::TURN_LEFT, tmp, tmp);
            break;
        case 3: // R
            tmp = RND_DIST();
            Serial.print("Движение вправо на оборота: ");
            Serial.println(tmp);
            MovementModule::getInstance().NewMov(MotionState::TURN_RIGHT, tmp, tmp);
            break;
        case 4: // L
            Serial.println("Движение влево на 90' ");
            MovementModule::getInstance().NewMov(MotionState::TURN_LEFT90);
            break;
        case 5: // R
            Serial.println("Движение вправо на 90' ");
            MovementModule::getInstance().NewMov(MotionState::TURN_RIGHT90);
            break;
        }
        return;
    }
    if (timer2 < 0)
    {
        Serial.println("Событие IDEL SPEAK произошло !!!");
        timer2 = random(TIMER_SPEAK * 0.5, TIMER_SPEAK * 1.5);
        Serial.print(" Следующее через :");
        Serial.print((timer2 / 1000) / 60);
        Serial.println(" минут.");
        return;
    }
}
bool IdelManager::IsIdel()
{
    if (MovementModule::getInstance().isBusy())
        return false;
    return true;
}
float IdelManager::RND_DIST() { return (random(10, 101) / 100.0f); }