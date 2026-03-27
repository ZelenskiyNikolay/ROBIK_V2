#pragma once
#include "Core/FileStream.h"
#include <SdFat.h>
#include "Module/SDModule.h"

class SDStream : public FileStream {
public:
    bool open(const char* path) override {
        if (_file) _file.close();
        SdFat* card = SDModule::getInstance().getCard();
        if (!card) return false;
        return _file.open(card, path, O_READ);
    }

    uint32_t read(uint8_t* buffer, uint32_t size) override {
        if (!_file.isOpen()) return 0;
        return _file.read(buffer, size);
    }

    void close() override {
        if (_file.isOpen()) _file.close();
    }

    bool isAvailable() override {
        return _file.isOpen() && _file.available() > 0;
    }

private:
    File32 _file;
};