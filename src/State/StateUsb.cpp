#include "StateUsb.h"

StateUsb::StateUsb(DisplayOled &disp) : display(&disp) {}

void StateUsb::enter()
{
    display->clear();
    display->drawText(" USB MODE", 0, 0, 2);
    display->drawText("Connected to PC", 0, 30, 1);
    display->drawText("Press # to Exit", 0, 40, 1);
    display->drawText("Eject Cable to Exit", 0, 50, 1);

    digitalWrite(OLED_CS, HIGH);

    delay(100);

    usb = new USBModule();
    usb->begin(SDModule::getInstance().getCard());

    TinyUSBDevice.detach();
    delay(50);
    TinyUSBDevice.attach();
}

void StateUsb::update(float dt)
{
    tud_task(); // КРИТИЧЕСКИ ВАЖНО: без этого стек не обновит статус

    bool currentConnected = tud_ready();

    if (!isMounted)
    {
        if (currentConnected)
            isMounted = true;
        return;
    }

    // Если связь была, но пропала
    if (!currentConnected)
    {
        busy_wait_us(10000);
        tud_task();

        if (!tud_ready())
        {
            usb->stop();
            watchdog_reboot(0, 0, 0);
        }
    }
    IrLogic();
}

void StateUsb::IrLogic()
{
    ButtonIR tmp = IRSensor::getInstance().GetSensorState();
    switch (tmp)
    {
    case ButtonHash:
        usb->stop();
        watchdog_reboot(0, 0, 0);
        break;
    default:
        break;
    }
}