#include "USBModule.h"

SdFat* USBModule::_sd_ptr = nullptr;
Adafruit_USBD_MSC USBModule::usb_msc;

USBModule::USBModule() {}

USBModule::~USBModule() {
    stop(); // При удалении объекта гасим всё
}

bool USBModule::begin(SdFat* sd_ptr) {
    if (!sd_ptr) return false;
    _sd_ptr = sd_ptr;

    usb_msc.setID("Robik-2", "SD Flash", "1.1"); // Версия 1.1!
    usb_msc.setReadWriteCallback(msc_read_cb, msc_write_cb, msc_flush_cb);
    
    uint32_t sectorCount = _sd_ptr->card()->sectorCount();
    usb_msc.setCapacity(sectorCount, 512);
    usb_msc.setUnitReady(true);
    
    return usb_msc.begin();
}

void USBModule::stop() {
    usb_msc.setUnitReady(false); // Говорим ПК, что диск извлечен
    TinyUSBDevice.detach();      // Физически отключаемся от шины
    _sd_ptr = nullptr;
}

bool USBModule::isConnected() {
    return tud_mounted();
}

bool USBModule::isCableConnected() {
    return tud_connected();  // ← физическое подключение
}

bool USBModule::isMounted() {
    return tud_mounted();              // ← диск открыт ПК
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