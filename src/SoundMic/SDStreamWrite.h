#pragma once
#include "Core/FileStream.h"
#include <SdFat.h>
#include "Module/SDModule.h"

class SDStreamWrite : public FileStream
{
public:
    bool open(const char *path) override { return open(path, false); }
    bool open(const char *path, bool writeMode = false)
    {
        if (_file)
            _file.close();
        SdFat *card = SDModule::getInstance().getCard();
        if (!card)
            return false;
        if (writeMode)
        {
            return _file.open(card, path, O_WRITE | O_CREAT | O_TRUNC);
        }
        return _file.open(card, path, O_READ);
    }

    uint32_t write(const uint16_t *buffer, uint32_t size)
    {
        if (!_file.isOpen())
            return 0;
        return _file.write(buffer, size);
    }
    uint32_t read(uint16_t *buffer, uint32_t size)
    {
        if (!_file.isOpen())
            return 0;
        return _file.read(buffer, size);
    }
    uint32_t read(uint8_t *buffer, uint32_t size) override
    {
        if (!_file.isOpen())
            return 0;
        return _file.read(buffer, size);
    }
    void close() override
    {
        if (_file.isOpen())
            _file.close();
    }

    bool isAvailable() override
    {
        return _file.isOpen() && _file.available() > 0;
    }
    bool seek(uint32_t pos)
    {
        if (!_file.isOpen())
            return false;
        return _file.seekSet(pos); // Метод SdFat для перемещения каретки
    }

private:
    File32 _file;
};