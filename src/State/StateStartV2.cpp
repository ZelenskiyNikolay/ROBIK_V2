#include "StateStartV2.h"

StateStartV2::StateStartV2(DisplayOled &disp)
    : display(&disp), sprite(&disp)
{
}
void Null(void) {}
void StateStartV2::enter()
{
    display->clear();
    timer = 0;

    display->drawText("  ROBIK", 0, 0, 2);
    display->drawText("              v2.6", 0, 20, 1);

    display->drawText("press 1 to Start.", 0, 46, 1);
    display->drawText("press # to menu.", 0, 56, 1);

    menu = &MenuController::getInstance();
    if (!menu->IsInit)
        menu->Init(*display);
    CreateMainManu();

    // Выключаем лишние фазы питания
    phaseControl(ENABLED,ENABLED);

    matrix.Init(*display);
    // radar.begin(25);
    lox.begin(41U,false,&Wire1);
    // phaseControl(ENABLED);
}
void StateStartV2::CreateMainManu()
{
    menu->hide();
    menu->ClearAll();

    menu->Menus[0].addHeader("Main MENU:", 2);
    menu->Menus[0].addItem("Sensors test.", &menu->Menus[1]);
    menu->Menus[0].addItem("Settings.", &menu->Menus[2]);
    menu->Menus[0].addItem("POWER OFF", [this]()
                   { this->powerOff(); }, INFO);
    menu->Menus[0].addItem("Exit MENU.", [this]()
                   { this->exitMenu(); });

    menu->Menus[1].addHeader("  SENSORS TEST:");
    menu->Menus[1].enableInverseHeader();
    menu->Menus[1].addItem("Battary sensor.", [this]()
                   { infoBat(); }, INFO);
    menu->Menus[1].addItem("I2C Scaner.", [this]()
                   { I2C_Scaner(); }, INFO);
    menu->Menus[1].addItem("Compass sensor.", [this]()
                   { сompassTest(); }, INFO);
    menu->Menus[1].addItem("Ultrasonic sensor.", [this]()
                   { ultrasonicTest(); }, INFO);
    menu->Menus[1].addItem("Radar test.", [this]()
                   { radar_Test(); }, INFO);
    menu->Menus[1].addItem("One Measure test.", [this]()
                   { One_Measure_Test(); }, INFO);
    menu->Menus[1].addItem("Time test", [this]()
                   { DrawClock(); }, INFO);
    menu->Menus[1].addItem("Back MENU.", [this]()
                   { back(); });

    menu->Menus[2].addHeader("Settings:", 2);
    menu->Menus[2].enableInverseHeader();
    menu->Menus[2].addItem("Sound Volume", [this]()
                   { SetSoundVolume(); }, EDIT_ITEM);
    menu->Menus[2].addItem("Night mod", [this]()
                   { SetNightMod(); }, EDIT_ITEM);
    menu->Menus[2].addItem("SET Click Sound", [this]()
                   { ClickSounnd(); }, EDIT_ITEM);
    menu->Menus[2].addItem("TEST SERVO", &menu->Menus[3]);
    menu->Menus[2].addItem("Back MENU.", [this]()
                   { back(); });

    menu->Menus[3].addHeader("Servo calibration:");
    menu->Menus[3].enableInverseHeader();
    menu->Menus[3].addItem("Calibrate 0'", [this]()
                   { servo(0); }, EDIT_ITEM);
    menu->Menus[3].addItem("Calibrate 45'", [this]()
                   { servo(45); }, EDIT_ITEM);
    menu->Menus[3].addItem("Calibrate 90'", [this]()
                   { servo(90); }, EDIT_ITEM);
    menu->Menus[3].addItem("Calibrate 135'", [this]()
                   { servo(135); }, EDIT_ITEM);
    menu->Menus[3].addItem("Calibrate 180'", [this]()
                   { servo(180); }, EDIT_ITEM);
    menu->Menus[3].addItem("Back MENU.", [this]()
                   { back(); });

    menu->SetCurentMenu(0);
}
void StateStartV2::update(float dt)
{
    if (!menu->IsActive())
    {
        IrLogic();
        if (!is_Screensaver_Play)
        {
            timer_Screensaver -= dt;
            if (timer_Screensaver < 0)
                is_Screensaver_Play = true;
            Draw(dt);
        }
        else
            screensaver(dt);
    }
    else
    {
        menu->update(dt);
        radar.update(dt);
    }

    MovementModule::getInstance().update(dt);
}
void StateStartV2::screensaver(float dt)
{
    if (is_Screensaver_Play)
        matrix.Update(dt);
}
void StateStartV2::IrLogic()
{
    ButtonIR tmp = IRSensor::getInstance().GetSensorState();
    if (tmp == NOOL)
        return;
    else if (is_Screensaver_Play)
    {
        timer_Screensaver = timeout_Screensaver;
        is_Screensaver_Play = !is_Screensaver_Play;
        matrix.Reset();
    }
    switch (tmp)
    {
    case Button1:
        if (BatteryModule::getInstance().getBatteryPercent() > 80 ||
            BatteryModule::getInstance().IsBatFull() ||
            !BatteryModule::getInstance().IsInPower())
            EventBus::push({EVENT_CHANGE_STATE, STATE_NORMAL});

        break;
    case ButtonHash:
        timer = 0;
        menu->show();
        break;

    default:
        break;
    }
}
void StateStartV2::Draw(float dt)
{
    timer -= dt;
    if (timer < 0)
    {
        if (!menu->IsActive())
        {
            display->clear();

            display->drawText("  ROBIK", 0, 0, 2);
            display->drawText("              v2.6", 0, 20, 1);

            display->drawText("press 1 to Start.", 0, 46, 1);
            display->drawText("press # to menu.", 0, 56, 1);
        }
        timer = 1000;

        byte reg = readRegister(REG_SYS_CONTROL);
        if (!getBit(reg, 2))
        {
            phaseControl(ENABLED);
        }
    }
}