#include "StateSearchBase.h"

StateSearchBase::StateSearchBase(DisplayOled &disp)
    : display(&disp), sprite(&disp)
{
}
void StateSearchBase::enter()
{
    display->clear();
    display->NeedUpdate = true;
    timer = 0;
}
void StateSearchBase::update(float dt)
{
    IrLogic();

    MovementModule::getInstance().update(dt);

    if (RotateSouth)
        RotateToSouth(dt);
    if (moveEast)
        MoveToEastDiscrete(dt);
    if (ToSouthConfirm)
        SouthConfirm(dt);
    if (AutoSearch)
        AutoSearchBase(dt);

    Draw(dt);
}
float timer1 = 0;
void StateSearchBase::AutoSearchBase(float dt)
{
    if (BatteryModule::getInstance().IsChargeConect())
    {
        MovementModule::getInstance().StopMov();
        EventBus::push({EVENT_CHANGE_STATE, STATE_START});
    }
    timer1 -= dt;
    if (timer1 < 0)
    {
        Serial.print("Steep: ");
        Serial.print(Steep);
        timer1 = 1000;
    }
    autoTimer -= dt;
    if (autoTimer > 0 || MovementModule::getInstance().isBusy())
        return;
    // Поворот на юг
    if (Steep == 0)
    {
        RotateSouth = true;
        autoTimer = 1000;
        Steep++;
        return;
    }
    if (Steep == 1)
    {
        Steep++;
        return;
    }
    // Выравнивание о край
    if (Steep == 2)
    {
        MovementModule::getInstance().NewMov(MotionState::EDGE_ALIG);
        Steep++;
        autoTimer = 3000;
        return;
    }
    if (Steep == 3)
    {
        MovementModule::getInstance().NewMov(MotionState::EDGE_ALIG);
        autoTimer = 3000;
        Steep++;
        return;
    }
    // Откат назад
    if (Steep == 4)
    {
        MovementModule::getInstance().NewMov(MotionState::BACKWARD, 0.2, 0.2);
        autoTimer = 1000;
        Steep++;
        return;
    }
    // Проверка ориентации на юг
    if (Steep == 5)
    {
        ToSouthConfirm = true;
        Steep++;
        autoTimer = 5000;
        return;
    }
    // Смотрим результат проверки и идем дальше или повторяем поиск
    if (Steep == 6)
    {
        if (!Confirm)
        {
            Steep = 0;
            autoTimer = 100;
            return;
        }
        else
        {
            autoTimer = 1000;
            Steep++;
            return;
        }
    }
    if (Steep == 7)
    {
        autoTimer = 2000;
        MovementModule::getInstance().NewMov(MotionState::TURN_LEFT90);
        Steep++;
        return;
    }
    if (Steep == 8)
    {
        autoTimer = 2000;
        moveEast = true;
        timer = 1000;
        cur = 0;
        STEEP = WORKS;
        Steep++;
        return;
    }
    if (STEEP == FINISHED && Steep == 9)
    {
        autoTimer = 2000;
        MovementModule::getInstance().NewMov(MotionState::TURN_RIGHT90);
        Steep++;
        return;
    }
    // Выравнивание о край
    if (Steep == 10)
    {
        MovementModule::getInstance().NewMov(MotionState::EDGE_ALIG);
        Steep++;
        autoTimer = 3000;
        return;
    }
    if (Steep == 11)
    {
        MovementModule::getInstance().NewMov(MotionState::EDGE_ALIG);
        autoTimer = 3000;
        Steep++;
        return;
    }
    if (Steep == 12)
    {
        MovementModule::getInstance().NewMov(MotionState::BACKWARD, 1, 1);
        autoTimer = 1000;
        Steep++;
        return;
    }
    if (Steep == 13)
    {
        if (BatteryModule::getInstance().IsChargeConect())
        {
            MovementModule::getInstance().StopMov();
            EventBus::push({EVENT_CHANGE_STATE, STATE_START});
        }
        else
        {
            Steep = 12;
        }
        return;
    }
}

void StateSearchBase::SouthConfirm(float dt)
{
    timer -= dt;
    if (timer > 0 || MovementModule::getInstance().isBusy())
        return;

    switch (SC)
    {
    case Redy:
    {
        if (distance <= 0)
        {
            SC = Triger;
            break;
        }
        if (itaration < 3)
        {
            measurements[itaration] = distance;
            itaration++;
            timer = 500;
            SC = Triger;
            break;
        }
        else
        {
            int successCount = 0;
            for (int i = 0; i < 3; i++)
            {
                if (measurements[i] > 250)
                {
                    successCount++;
                }
            }
            if (successCount >= 2)
            {
                Confirm = true;
            }
            else
            {
                Confirm = false;
            }
            ToSouthConfirm = false;
            distance = 0;
            SC = Redy;
            itaration = 0;
            memset(measurements, 0, sizeof(measurements));
            STEEP = FINISHED;
        }
        break;
    }

    case Triger:
        MovementModule::getInstance().TriggerUltrasonic();
        SC = Distanse;
        break;

    case Distanse:
        float temp = MovementModule::getInstance().GetDistance();
        if (temp > 0)
        {
            distance = temp;
            SC = Redy;
        }
        else
        {
            SC = Triger;
            timer = 100;
        }
        break;
    }
}
void StateSearchBase::MoveToEastDiscrete(float dt)
{
    timer -= dt;
    if (timer > 0 || MovementModule::getInstance().isBusy())
        return;

    switch (MoveToEast)
    {
    case Redy:
    {
        if (cur <= 0)
        {
            MoveToEast = Triger;
            return;
        }
        int delta = cur - MovementModule::getInstance().EastDistanse;
        if (delta == 0)
        {
            MovementModule::getInstance().StopMov();
            moveEast = false;
            STEEP = FINISHED;
            return;
        }
        timer = 500;
        if (abs(delta) > 5)
        {
            if (delta > 0)
                MovementModule::getInstance().NewMov(MotionState::FORWARD, 0.2, 0.2);
            else
                MovementModule::getInstance().NewMov(MotionState::BACKWARD, 0.2, 0.2);
        }
        else if (abs(delta) > 0)
        {
            if (delta > 0)
                MovementModule::getInstance().NewMov(MotionState::FORWARD, 0.05, 0.05);
            else
                MovementModule::getInstance().NewMov(MotionState::BACKWARD, 0.05, 0.05);
        }
        MoveToEast = Triger;
        break;
    }
    case Triger:
        MovementModule::getInstance().TriggerUltrasonic();
        MoveToEast = Distanse;
        break;
    case Distanse:
        float temp = MovementModule::getInstance().GetDistance();
        if (temp > 0)
        {
            if (temp < 200)
            {
                cur = temp;
                MoveToEast = Redy;
            }
            else
            {
                MoveToEast = Triger;
            }
        }
        else
            MoveToEast = Triger;
        break;
    }
}
void StateSearchBase::IrLogic()
{
    ButtonIR tmp = IRSensor::getInstance().GetSensorState();

    switch (tmp)
    {
    case Button1:
        MovementModule::getInstance().NewMov(MotionState::EDGE_ALIG);
        break;
    case ButtonHash:
        EventBus::push({EVENT_CHANGE_STATE, STATE_NORMAL});
        break;
    case Button2:
        RotateSouth = true;
        timer = 1000;
        break;
    case Button3:
        moveEast = true;
        timer = 1000;
        cur = 0;
        break;
    case Button4:
        MovementModule::getInstance().NewMov(MotionState::BACKWARD, 0.2, 0.2);
        break;
    case Button5:
        ToSouthConfirm = true;
        break;
    case ButtonOk:
        AutoSearch = true;
        Steep = 0;
        STEEP = READY;
        autoTimer = 100;
        break;
    default:
        break;
    }
}

void StateSearchBase::RotateToSouth(float dt)
{
    timer -= dt;
    if (timer < 0 && !MovementModule::getInstance().isBusy())
    {
        float ang = Compass::getInstance().getHeading();
        float delta = MovementModule::getInstance().SouthAngle - ang;
        if (delta > 180)
            delta -= 360;
        if (delta < -180)
            delta += 360;
        if (delta > 0)
        {
            delta += 15;
            // Поворот вправо на (delta / 360.0f)
            MovementModule::getInstance().NewMov(TURN_LEFT, delta / 360.0f, delta / 360.0f);
        }
        else
        {
            delta -= 15;
            // Поворот влево на (abs(delta) / 360.0f)
            MovementModule::getInstance().NewMov(TURN_RIGHT, fabsf(delta) / 360.0f, fabsf(delta) / 360.0f);
        }
        RotateSouth = false;
    }
}
void drawSearchBaseIcon(DisplayOled &display)
{
    display.clear();
    int cx = 64; // Центр экрана по X
    int cy = 32; // Центр экрана по Y

    // 1. Крыша (Треугольник: x0, y0, x1, y1, x2, y2)
    display.drawTriangle(cx, cy - 20, cx - 25, cy, cx + 25, cy, WHITE);

    // 2. Стены (Прямоугольник: x, y, w, h)
    display.drawRect(cx - 20, cy, 40, 25, WHITE);

    // 3. Маяк/Сигнал (Круг и дуги)
    display.fillCircle(cx, cy + 12, 3, WHITE); // Точка-источник
    display.drawCircle(cx, cy + 12, 8, WHITE); // Первая волна

    // Опционально: маска, чтобы дуга была только сверху
    display.fillRect(cx - 10, cy + 13, 20, 10, BLACK);

    display.drawText("SEARCH", cx - 15, cy + 30, 1);
    // 4. Текст или статус
}
void StateSearchBase::Draw(float dt)
{
    drawTimer -= dt;
    if (drawTimer < 0)
    {
        if (AutoSearch)
            drawSearchBaseIcon(*display);
        else
        {
            display->clear();
            display->drawText("LookSouth:", 0, 0, 1);
            char buffer[64];

            sprintf(buffer, "Confirm South: %d", Confirm);
            display->drawText(buffer, 0, 25, 1);

            display->drawText(buffer, 0, 55, 1);

            drawTimer = 500;
        }
    }
}