#include "USBModule.h"

// Инициализация статики
SdFat* USBModule::_sd_ptr = nullptr;
Adafruit_USBD_MSC USBModule::usb_msc;

bool USBModule::begin(SdFat* sd_ptr) {
    if (!sd_ptr) return false;
    _sd_ptr = sd_ptr;

    // Настройка имени устройства (как оно будет видно в Windows)
    usb_msc.setID("Robik-2", "SD Flash", "1.0");
    usb_msc.setReadWriteCallback(msc_read_cb, msc_write_cb, msc_flush_cb);
    
    // Получаем количество секторов через метод card()
    uint32_t sectorCount = _sd_ptr->card()->sectorCount();
    if (sectorCount == 0) return false;

    usb_msc.setCapacity(sectorCount, 512);
    usb_msc.setUnitReady(true);
    
    return usb_msc.begin();
}

bool USBModule::isConnected() {
    return tud_mounted();
}

void USBModule::update() {
    // Для ядра Earlephilhower стек USB обрабатывается в фоне, 
    // но если будут лаги, можно добавить сюда вызовы TinyUSB_Device_Task()
}

int32_t USBModule::msc_read_cb(uint32_t lba, void* buffer, uint32_t bufsize) {
    if (!_sd_ptr) return -1;
    // Используем readSectors через внутреннюю карту
    return _sd_ptr->card()->readSectors(lba, (uint8_t*)buffer, bufsize / 512) ? (int32_t)bufsize : -1;
}

int32_t USBModule::msc_write_cb(uint32_t lba, uint8_t* buffer, uint32_t bufsize) {
    if (!_sd_ptr) return -1;
    return _sd_ptr->card()->writeSectors(lba, (uint8_t*)buffer, bufsize / 512) ? (int32_t)bufsize : -1;
}

void USBModule::msc_flush_cb(void) {
    if (_sd_ptr) _sd_ptr->card()->syncDevice(); // Синхронизация кэша карты
}