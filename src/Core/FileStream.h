#pragma once
#include <Arduino.h>

class FileStream {
public:
    virtual ~FileStream() {}
    virtual bool open(const char* path) = 0;
    virtual uint32_t read(uint8_t* buffer, uint32_t size) = 0;
    virtual void close() = 0;
    virtual bool isAvailable() = 0;
};