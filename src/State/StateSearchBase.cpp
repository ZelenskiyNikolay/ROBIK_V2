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

    if (lookSouth)
        LookSouthNew(dt);
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
void StateSearchBase::AutoSearchBase(float dt)
{
    // autoTimer -= dt;
    // if (autoTimer > 0 || SafetyModule::getInstance().isBusy())
    //     return;
    // // Поиск юга
    // if (STEEP == READY && Steep == 0)
    // {
    //     maxDistance = 0;
    //     bestStep = 0;
    //     corner = 0;
    //     currentStep = 0;
    //     timer = 1000;
    //     lookSouth = true;
    //     Look = Triger;
    //     Steep++;
    //     return;
    // }
    // if (STEEP == FINISHED && Steep == 1)
    // {
    //     Steep++;
    //     return;
    // }
    // // Поворот на юг
    // if (STEEP == FINISHED && Steep == 2)
    // {
    //     Steep++;
    //     return;
    // }
    // if (STEEP == FINISHED && Steep == 3)
    // {
    //     autoTimer = 2000;
    //     Steep++;
    //     return;
    // }
    // // Выравнивание обкрай
    // if (STEEP == FINISHED && Steep == 4)
    // {
    //     STEEP = WORKS;
    //     autoTimer = 2000;
    //     edgeAlign = true;
    //     Steep++;
    //     return;
    // }
    // if (STEEP == FINISHED && Steep == 5)
    // {
    //     autoTimer = 2000;
    //     Steep++;
    //     return;
    // }
    // // Проверка ориентации на юг
    // if (STEEP == FINISHED && Steep == 6)
    // {
    //     ToSouthConfirm = true;
    //     Steep++;
    //     return;
    // }
    // if (STEEP == FINISHED && Steep == 7)
    // {
    //     autoTimer = 5000;
    //     Steep++;
    //     return;
    // }
    // // Смотрим результат проверки и идем дальше или повторяем поиск
    // if (STEEP == FINISHED && Steep == 8)
    // {
    //     if (!Confirm)
    //     {
    //         AutoSearch = true;
    //         Steep = 0;
    //         STEEP = READY;
    //         autoTimer = 100;
    //         return;
    //     }
    //     else
    //     {
    //         autoTimer = 1000;
    //         Steep++;
    //         STEEP = FINISHED;
    //         return;
    //     }
    // }
    // // Выравниваем об край
    // if (STEEP == FINISHED && Steep == 9)
    // {
    //     autoTimer = 2000;
    //     edgeAlign = true;
    //     Steep++;
    //     return;
    // }
    // if (STEEP == FINISHED && Steep == 10)
    // {
    //     autoTimer = 2000;
    //     Steep++;
    //     return;
    // }
    // // Поворот на восток и выравнивание по калиброванному значению
    // if (STEEP == FINISHED && Steep == 11)
    // {
    //     STEEP = WORKS;
    //     autoTimer = 2000;
    //     SafetyModule::getInstance().NewMov(MotionState::TURN_LEFT90);
    //     moveEast = true;
    //     timer = 1000;
    //     cur = 0;
    //     Steep++;
    //     return;
    // }
    // if (STEEP == FINISHED && Steep == 12)
    // {
    //     autoTimer = 1000;
    //     Steep++;
    //     return;
    // }
    // // поворот задом к базе мы прямо напротив базы
    // if (STEEP == FINISHED && Steep == 13)
    // {
    //     SafetyModule::getInstance().NewMov(MotionState::TURN_RIGHT90);
    //     autoTimer = 2000;
    //     timer = 1000;
    //     Steep++;
    //     return;
    // }
    // if (STEEP == FINISHED && Steep == 14)
    // {
    //     autoTimer = 5000;
    //     Steep++;
    //     return;
    // }
    // // Пару раз выравниваемся об край
    // if (STEEP == FINISHED && Steep == 15)
    // {
    //     STEEP = WORKS;
    //     autoTimer = 1000;
    //     edgeAlign = true;
    //     Steep++;
    //     return;
    // }
    // if (STEEP == FINISHED && Steep == 16)
    // {
    //     autoTimer = 2000;
    //     Steep++;
    //     return;
    // }
    // if (STEEP == FINISHED && Steep == 17)
    // {
    //     STEEP = WORKS;
    //     autoTimer = 1000;
    //     edgeAlign = true;
    //     Steep++;
    //     return;
    // }
    // if (STEEP == FINISHED && Steep == 18)
    // {
    //     autoTimer = 2000;
    //     Steep++;
    //     return;
    // }
    // // Финальное движение назад
    // if (STEEP == FINISHED && Steep == 19)
    // {
    //     autoTimer = 5000;
    //     SafetyModule::getInstance().NewMov(MotionState::BACKWARD, 20, 20);
    //     Steep++;
    //     return;
    // }
    // if (STEEP == FINISHED && Steep == 20)
    // {
    //     autoTimer = 1000;
    //     Steep++;
    //     return;
    // }
    // if (STEEP == FINISHED && Steep == 21)
    // {
    //     autoTimer = 500;
    //     SafetyModule::getInstance().NewMov(MotionState::BACKWARD, 3, 3);
    //     Steep++;
    //     return;
    // }
    // if (STEEP == FINISHED && Steep == 22)
    // {
    //     autoTimer = 1000;
    //     Steep++;
    //     return;
    // }
    // if (STEEP == FINISHED && Steep == 23)
    // {
    //     if (PowerModule::getInstance().State == POWER_EXTERNAL)
    //     {
    //         // Steep++;
    //         // return;
    //         EventBus::push({EVENT_CHANGE_STATE, STATE_START});
    //     }
    //     else
    //     {
    //         Steep = 21;
    //         return;
    //     }
    // }
}
void StateSearchBase::SouthConfirm(float dt)
{}
//     timer -= dt;
//     if (timer > 0 || SafetyModule::getInstance().isBusy())
//         return;

//     switch (SC)
//     {
//     case Redy:
//     {
//         if (distance <= 0)
//         {
//             SC = Triger;
//             break;
//         }
//         if (itaration < 3)
//         {
//             measurements[itaration] = distance;
//             itaration++;
//             timer = 500;
//             SC = Triger;
//             break;
//         }
//         else
//         {
//             int successCount = 0;
//             for (int i = 0; i < 3; i++)
//             {
//                 if (measurements[i] > 250)
//                 {
//                     successCount++;
//                 }
//             }
//             if (successCount >= 2)
//             {
//                 Confirm = true;
//             }
//             else
//             {
//                 Confirm = false;
//             }
//             ToSouthConfirm = false;
//             distance = 0;
//             SC = Redy;
//             itaration = 0;
//             memset(measurements, 0, sizeof(measurements));
//             STEEP = FINISHED;
//         }
//         break;
//     }

//     case Triger:
//         SafetyModule::getInstance().TriggerUltrasonic();
//         SC = Distanse;
//         break;

//     case Distanse:
//         float temp = SafetyModule::getInstance().GetDistance();
//         if (temp > 0)
//         {
//             distance = temp;
//             SC = Redy;
//         }
//         else
//         {
//             SC = Triger;
//             timer = 100;
//         }
//         break;
//     }
// }
void StateSearchBase::MoveToEastDiscrete(float dt)
{}
//     timer -= dt;
//     if (timer > 0 || SafetyModule::getInstance().isBusy())
//         return;

//     switch (MoveToEast)
//     {
//     case Redy:
//     {
//         if (cur <= 0)
//         {
//             MoveToEast = Triger;
//             return;
//         }
//         int delta = cur - GlobalSettings::getInstance().Point.east;
//         if (delta == 0)
//         {
//             SafetyModule::getInstance().StopMov();
//             moveEast = false;
//             STEEP = FINISHED;
//             return;
//         }
//         timer = 500;
//         if (abs(delta) > 5)
//         {
//             if (delta > 0)
//                 SafetyModule::getInstance().NewMov(MotionState::FORWARD, 5, 5);
//             else
//                 SafetyModule::getInstance().NewMov(MotionState::BACKWARD, 5, 5);
//         }
//         else if (abs(delta) > 0)
//         {
//             if (delta > 0)
//                 SafetyModule::getInstance().NewMov(MotionState::FORWARD, 2, 2);
//             else
//                 SafetyModule::getInstance().NewMov(MotionState::BACKWARD, 2, 2);
//         }
//         MoveToEast = Triger;
//         break;
//     }
//     case Triger:
//         SafetyModule::getInstance().TriggerUltrasonic();
//         MoveToEast = Distanse;
//         break;
//     case Distanse:
//         float temp = SafetyModule::getInstance().GetDistance();
//         if (temp > 0)
//         {
//             if (temp < 200)
//             {
//                 cur = temp;
//                 MoveToEast = Redy;
//             }
//             else
//             {
//                 MoveToEast = Triger;
//             }
//         }
//         else
//             MoveToEast = Triger;
//         break;
//     }
// }
void StateSearchBase::LookSouthNew(float dt)
{}
//     timer -= dt;
//     if (timer < 0 && !SafetyModule::getInstance().isBusy())
//     {
//         switch (Look)
//         {
//         case Redy:
//             if (currentStep > 11)
//             {
//                 lookSouth = false;
//                 STEEP = FINISHED;
//                 timer = 1000;
//                 break;
//             }
//             timer = 1000;
//             SafetyModule::getInstance().NewMov(MotionState::TURN_LEFT, ticks90Left / 3, 0);
//             corner += 30;
//             currentStep++;
//             Look = Triger;
//             break;
//         case Triger:
//             SafetyModule::getInstance().TriggerUltrasonic();
//             Look = Distanse;
//             break;
//         case Distanse:
//             float temp = SafetyModule::getInstance().GetDistance();
//             if (temp > 0)
//             {
//                 if (temp > maxDistance)
//                 {
//                     maxDistance = temp;
//                     bestStep = currentStep;
//                     Look = Redy;
//                 }
//                 else
//                 {
//                     Look = Redy;
//                 }

//                 if (temp > 250)
//                 {
//                     lookSouth = false;
//                     STEEP = FINISHED;
//                     timer = 1000;
//                     return;
//                 }

//             }
//             else
//                 Look = Triger;

//             break;
//         }
//     }
// }
void StateSearchBase::LookSouth(float dt)
{}
//     timer -= dt;
//     if (timer < 0 && !SafetyModule::getInstance().isBusy())
//     {
//         switch (Look)
//         {
//         case Redy:
//             if (currentStep > 11)
//             {
//                 lookSouth = false;
//                 STEEP = FINISHED;
//                 timer = 1000;
//                 break;
//             }
//             timer = 1000;
//             SafetyModule::getInstance().NewMov(MotionState::TURN_LEFT, ticks90Left / 3, 0);
//             corner += 30;
//             currentStep++;
//             Look = Triger;
//             break;
//         case Triger:
//             SafetyModule::getInstance().TriggerUltrasonic();
//             Look = Distanse;
//             break;
//         case Distanse:
//             float temp = SafetyModule::getInstance().GetDistance();
//             if (temp > 0)
//             {
//                 if (temp > maxDistance)
//                 {
//                     maxDistance = temp;
//                     bestStep = currentStep;
//                     Look = Redy;
//                 }
//                 else
//                 {
//                     Look = Redy;
//                 }
//             }
//             else
//                 Look = Triger;

//             break;
//         }
//     }
// }
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

        break;
    case Button3:
        
        break;

    default:
        break;
    }
}
//     // ButtonIR tmp = ir.GetSensorState();
//     // switch (tmp)
//     // {
//     // case Button1:
//     //     SafetyModule::getInstance().NewMov(MotionState::TURN_LEFT, 0, 0);
//     //     break;
//     // case ButtonUp:
//     //     SafetyModule::getInstance().NewMov(MotionState::FORWARD, 20, 20);
//     //     break;
//     // case Button3:
//     //     SafetyModule::getInstance().NewMov(MotionState::TURN_RIGHT, 0, 0);
//     //     break;
//     // case ButtonLeft:
//     //     SafetyModule::getInstance().NewMov(MotionState::TURN_LEFT90);
//     //     break;
//     // case ButtonRight:
//     //     SafetyModule::getInstance().NewMov(MotionState::TURN_RIGHT90);
//     //     break;
//     // case Button9:
//     //     SafetyModule::getInstance().NewMov(MotionState::TURN_LEFT90);
//     //     moveEast = true;
//     //     timer = 1000;
//     //     cur = 0;
//     //     break;
//     // case ButtonDown:
//     //     SafetyModule::getInstance().NewMov(MotionState::BACKWARD, 3, 3);
//     //     break;

//     // case Button8:
//     //     angle = bestStep * 30;
//     //     RotateSouth = true;
//     //     break;

//     // case Button0:
//     //     edgeAlign = true;
//     //     break;

//     // case ButtonHash:
//     //     ToSouthConfirm = true;
//     //     break;

//     // case Button7:
//     //     maxDistance = 0;
//     //     bestStep = 0;
//     //     corner = 0;
//     //     currentStep = 0;
//     //     timer = 1000;
//     //     lookSouth = true;
//     //     Look = Triger;
//     //     break;

//     // case Button5:
//     //     AutoSearch = true;
//     //     Steep = 0;
//     //     STEEP = READY;
//     //     autoTimer = 100;
//     //     break;

//     // default:
//     //     break;
//     // }
// }
void StateSearchBase::RotateToSouth(float dt)
{}
//     timer -= dt;
//     if (timer < 0 && !SafetyModule::getInstance().isBusy())
//     {
//         if (angle <= 0)
//         {
//             RotateSouth = false;
//             STEEP = FINISHED;
//             return;
//         }
//         if (angle >= 360)
//         {
//             RotateSouth = false;
//             STEEP = FINISHED;
//             timer = 500;
//             return;
//         }
//         if (angle < 180)
//         {
//             if (angle > 90)
//             {
//                 timer = 1000;
//                 SafetyModule::getInstance().NewMov(MotionState::TURN_LEFT90);
//                 angle -= 90;
//             }
//             else
//             {
//                 timer = 1000;
//                 SafetyModule::getInstance().NewMov(MotionState::TURN_LEFT, ticks90Left / 3, 0);
//                 angle -= 30;
//             }
//         }
//         else
//         {
//             int temp = 360 - angle;
//             if (temp > 90)
//             {
//                 timer = 1000;
//                 SafetyModule::getInstance().NewMov(MotionState::TURN_RIGHT90);
//                 angle += 90;
//             }

//             else
//             {
//                 timer = 1000;
//                 SafetyModule::getInstance().NewMov(MotionState::TURN_RIGHT, 0, ticks90Left / 3);
//                 angle += 30;
//             }
//         }
//     }
// }
void StateSearchBase::Draw(float dt)
{
    drawTimer -= dt;
    if (drawTimer < 0)
    {
        display->clear();
        display->drawText("LookSouth:", 0, 0, 1);
        char buffer[64];

        sprintf(buffer, "Confirm South: %d", Confirm);
        display->drawText(buffer, 0, 25, 1);

        sprintf(buffer, "MaxDis: %d cm St:%d", maxDistance, currentStep);
        display->drawText(buffer, 0, 45, 1);
        sprintf(buffer, "BestSt: %d Corn: %d", bestStep, corner);
        display->drawText(buffer, 0, 55, 1);

        drawTimer = 500;
    }
}