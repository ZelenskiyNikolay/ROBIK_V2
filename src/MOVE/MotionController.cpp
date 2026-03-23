#include "MotionController.h"

MotionController::MotionController()
    : motorL(6, 7, 8, 9), motorR(10, 11, 12, 13)
{
}

void MotionController::begin()
{
    motorL.begin(); // Выполнит pinMode
    motorR.begin();
}

void MotionController::NewMov(MotionState Command, float Left, float Right)
{
    switch (Command)
    {
    case MotionState::IDLE:
        command.type = Command;
        break;
    case MotionState::FORWARD:
        command.targetLeft = roundf(Left * STEPS_PER_REV);
        command.targetRight = roundf(Right * STEPS_PER_REV);
        command.type = Command;
        break;
    case MotionState::BACKWARD:
        command.targetLeft = roundf(Left * STEPS_PER_REV);
        command.targetRight = roundf(Right * STEPS_PER_REV);
        command.type = Command;
        break;
    case MotionState::TURN_LEFT:
        command.targetLeft = Left;
        command.type = Command;
        break;
    case MotionState::TURN_RIGHT:
        command.targetRight = Right;
        command.type = Command;
        break;
    case MotionState::TURN_LEFT90:
        startHeading = Compass::getInstance().getHeading();
        targetHeading = startHeading + 90;
        if (targetHeading >= 360)
            targetHeading -= 360;

        if (Left > 0)
        {
            command.targetLeft = Left * STEPS_90;
            command.targetRight = Left * STEPS_90;
        }
        else
        {
            command.targetLeft = STEPS_90;
            command.targetRight = STEPS_90;
        }
        command.type = Command;
        break;
    case MotionState::TURN_RIGHT90:
        command.type = Command;
        break;
    }
    Serial.print("Move type: ");
    Serial.print(command.type);
    Serial.print(" ||  targetLeft: ");
    Serial.print(command.targetLeft);
    Serial.print(" targetRight: ");
    Serial.println(command.targetRight);
}

int MotionController::update(float dt)
{
    // Serial.print("Move type: ");
    // Serial.print(command.type);
    // Serial.print(" ||  FPS targetLeft: ");
    // Serial.print(command.targetLeft);
    // Serial.print(" targetRight: ");
    // Serial.println(command.targetRight);

    timer -= dt;
    if (command.type == IDLE)
    {
       // motorL.Idel();
        //motorR.Idel();
        return 1;
    }

    if (command.targetLeft <= 0 && command.targetRight <= 0)
    {
        command.targetLeft = 0;
        command.targetRight = 0;
        command.type = IDLE;
        return 1;
    }

    if (timer > 0)
        return -1;
    else
        timer += TIME_STEP;

    switch (command.type)
    {
    case FORWARD:
        ForwardMov();
        break;
    case BACKWARD:
        BackwardMov();
        break;
    // case TURN_LEFT:
    //     if (command.targetLeft > 2)
    //         TurnLeft(dl,dr, command.targetLeft);
    //     else
    //         TurnLeft(dl,dr);
    //     break;
    // case TURN_RIGHT:
    //     if (command.targetRight > 2)
    //         TurnRight(dl,dr, command.targetRight);
    //     else
    //         TurnRight(dl,dr);
    //     break;
    case TURN_LEFT90:
        Turn90Left();
        break;
    // case TURN_RIGHT90:
    //     Turn90Right(dl,dr);
    //     break;
    case IDLE:
        break;
    }
    return -1;
}
float angleDiff(float target, float current)
{
    float diff = target - current;

    if (diff > 180)
        diff -= 360;
    if (diff < -180)
        diff += 360;

    return diff;
}

void MotionController::Turn90Left()
{
    float h = Compass::getInstance().getHeading();
    float error = angleDiff(targetHeading, h);

    if (abs(error) <= 2)
    {
        command.targetRight = 0;
        command.targetLeft = 0;
        command.type = IDLE;
        return;
    }

    motorL.step(-1);
    motorR.step(-1);
}

void MotionController::ForwardMov()
{

    motorL.step(-1);
    motorR.step(1);

    command.targetRight--;
    command.targetLeft--;
}

void MotionController::BackwardMov()
{
    motorL.step(1);
    motorR.step(-1);

    command.targetRight--;
    command.targetLeft--;
}

bool MotionController::isBusy() const
{
    if (command.type == IDLE)
        return false;
    else
        return true;
}