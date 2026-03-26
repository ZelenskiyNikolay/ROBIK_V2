#pragma once
#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <SdFat.h>

class USBModule {
public:
    static USBModule &getInstance() {
        static USBModule instance;
        return instance;
    }

    // Передаем указатель на сам объект SdFat
    bool begin(SdFat* sd_ptr); 
    bool isConnected();
    void update();

private:
    USBModule() {}
    static SdFat* _sd_ptr; // Храним указатель на основной объект
    static Adafruit_USBD_MSC usb_msc;

    static int32_t msc_read_cb(uint32_t lba, void* buffer, uint32_t bufsize);
    static int32_t msc_write_cb(uint32_t lba, uint8_t* buffer, uint32_t bufsize);
    static void msc_flush_cb(void);
};