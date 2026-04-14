#include "IdelManager.h"

IdelManager::IdelManager() {}

void IdelManager::begin()
{
    timer1 = TIMER_MOV;
    timer2 = TIMER_SPEAK;
}

void IdelManager::Update(float dt)
{
    if (MovementModule::getInstance().isBusy())
    {
        timer1 = TIMER_MOV;
        return;
    }
    if (SoundManager::getInstance().Is_Playing())
    {
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

        Serial.println("Записываем очередь движений.. от 1 до 6 движений.");
        int iterations = random(0, 10);
        for (int i = 0; i < iterations; i++)
        {
            int action = random(0, 6);
            float tmp;
            idelMov = true;
            int val;
            switch (action)
            {
            case 0: // f
                tmp = RND_DIST();
                Serial.print("Движение вперед на оборота: ");
                Serial.println(tmp);
                MovementModule::getInstance().AddMovQueue(MotionState::FORWARD, tmp, tmp);
                break;
            case 1: // b
                tmp = RND_DIST();
                Serial.print("Движение назад на оборота: ");
                Serial.println(tmp);
                MovementModule::getInstance().AddMovQueue(MotionState::BACKWARD, tmp, tmp);
                break;
            case 2: // L
                tmp = RND_DIST();
                Serial.print("Движение влево на оборота: ");
                Serial.println(tmp);
                MovementModule::getInstance().AddMovQueue(MotionState::TURN_LEFT, tmp, tmp);
                break;
            case 3: // R
                tmp = RND_DIST();
                Serial.print("Движение вправо на оборота: ");
                Serial.println(tmp);
                MovementModule::getInstance().AddMovQueue(MotionState::TURN_RIGHT, tmp, tmp);
                break;
            case 4: // SET_SPEED
                val = random(0, 100);
                if (val % 2 == 0)
                {
                    MovementModule::getInstance().AddMovQueue(MotionState::SET_SPEED); // Медленно
                    Serial.println("Меняем скорость движения -> Медленно ");
                }
                else
                {
                    MovementModule::getInstance().AddMovQueue(MotionState::SET_SPEED, 1); // Быстро
                    Serial.println("Меняем скорость движения -> Быстро ");
                }
                break;
            case 5:      // PAUSE
                val = random(500, 1000); // Пауза от 0.5 до 3 сек
                MovementModule::getInstance().AddMovQueue(MotionState::WEIT, val);
                Serial.printf("Добавляем паузу в очередь: %.3f сек\n", val / 1000.0);
                break;
            }
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

        SoundManager::getInstance().Random_Play("Idel");
        return;
    }
}
bool IdelManager::IsIdel()
{
    // if(idelMov)
    //     if (!MovementModule::getInstance().isBusy())
    //         idelMov = false;
    // if (!idelMov)
    //     if (MovementModule::getInstance().isBusy())
    //         return false;
    // if (SoundManager::getInstance().Is_Playing())
    //         return false;
    return true;
}
float IdelManager::RND_DIST() { return (random(10, 101) / 100.0f); }