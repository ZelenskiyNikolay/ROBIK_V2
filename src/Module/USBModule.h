#pragma once
#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <SdFat.h>

class USBModule {
public:
    USBModule();  // Обычный конструктор
    ~USBModule(); // Деструктор для очистки!

    bool begin(SdFat* sd_ptr); 
    void stop();
    bool isConnected();
    bool isCableConnected();
    bool isMounted();
private:
    static SdFat* _sd_ptr;
    static Adafruit_USBD_MSC usb_msc;

    // Эти функции обязаны быть статическими для TinyUSB
    static int32_t msc_read_cb(uint32_t lba, void* buffer, uint32_t bufsize);
    static int32_t msc_write_cb(uint32_t lba, uint8_t* buffer, uint32_t bufsize);
    static void msc_flush_cb(void);
};