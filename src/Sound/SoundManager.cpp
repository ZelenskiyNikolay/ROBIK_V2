#include "SoundManager.h"

AudioBridge audioBridge;

SoundManager::SoundManager() {}

void SoundManager::Init()
{
    // Ждем, пока Ядро 1 проинициализируется и запишет пин в мост
    while (audioBridge.En_Pin == 0)
    {
        tight_loop_contents(); // Ждем "отмашку" от Ядра 1
    }

    // Подхватываем пин из моста в локальную переменную класса
    _en_pin = audioBridge.En_Pin;

    // Теперь менеджер знает, чем управлять, не имея жестких дефайнов
}

void SoundManager::Play(const uint8_t *data, uint32_t size)
{
    _source = data;
    _totalSize = size;
    _cursor = HEADER_SIZE;

    audioBridge.isPlaying = false; // На всякий случай стопаем

    // Сразу забиваем оба буфера «на холодную»
    if (_cursor < _totalSize)
    {
        uint32_t toCopy = min(AudioBridge::BUF_SIZE, _totalSize - _cursor);
        memcpy(audioBridge.bufferA, _source + _cursor, toCopy);
        _cursor += toCopy;
        audioBridge.readyA = true;
    }

    if (_cursor < _totalSize)
    {
        uint32_t toCopy = min(AudioBridge::BUF_SIZE, _totalSize - _cursor);
        memcpy(audioBridge.bufferB, _source + _cursor, toCopy);
        _cursor += toCopy;
        audioBridge.readyB = true;
    }

    audioBridge.isPlaying = true;
    digitalWrite(_en_pin, LOW); // Включаем усилитель (твой AUDIO_EN_PIN)
}

bool SoundManager::Is_Playing()
{
    return audioBridge.isPlaying;
}

void SoundManager::update()
{
    if (!audioBridge.isPlaying)
        return;

    // Если Ядро 1 освободило буфер А
    if (!audioBridge.readyA && _cursor < _totalSize)
    {
        uint32_t toCopy = min(AudioBridge::BUF_SIZE, _totalSize - _cursor);
        memcpy(audioBridge.bufferA, _source + _cursor, toCopy);
        _cursor += toCopy;
        audioBridge.readyA = true;
    }

    // Если Ядро 1 освободило буфер B
    if (!audioBridge.readyB && _cursor < _totalSize)
    {
        uint32_t toCopy = min(AudioBridge::BUF_SIZE, _totalSize - _cursor);
        memcpy(audioBridge.bufferB, _source + _cursor, toCopy);
        _cursor += toCopy;
        audioBridge.readyB = true;
    }

    // Конец файла
    if (_cursor >= _totalSize && !audioBridge.readyA && !audioBridge.readyB)
    {
        Stop();
    }
}

void SoundManager::Stop()
{
    audioBridge.isPlaying = false;
    audioBridge.readyA = false;
    audioBridge.readyB = false;
    digitalWrite(_en_pin, HIGH); // Гасим усилитель
}