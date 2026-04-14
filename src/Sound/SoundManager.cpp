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

void SoundManager::Play(const char *filename)
{
    _isSD = true;
    // 1. Если уже что-то поет — глушим
    if (audioBridge.isPlaying)
    {
        audioBridge.isPlaying = false;
        // Даем Ядру 1 время выйти из прерывания, если нужно
        delay(1);
    }

    // 2. Пытаемся открыть файл через наш новый стрим
    if (_sdStream.open(filename))
    {
        _activeStream = &_sdStream; // Указываем, что сейчас поем с SD

        // 3. Пропускаем WAV заголовок (44 байта)
        uint8_t header[44];
        _activeStream->read(header, 44);

        // 4. Предзагрузка буферов (важно для бесшовного старта)
        if (_activeStream->isAvailable())
        {
            uint32_t rA = _activeStream->read(audioBridge.bufferA, AudioBridge::BUF_SIZE);
            audioBridge.readyA = (rA > 0);
        }
        if (_activeStream->isAvailable())
        {
            uint32_t rB = _activeStream->read(audioBridge.bufferB, AudioBridge::BUF_SIZE);
            audioBridge.readyB = (rB > 0);
        }

        // 5. Поехали!
        audioBridge.isPlaying = true;
        if (_en_pin != 0)
            digitalWrite(_en_pin, LOW);

        Serial.print("Playing from SD: ");
        Serial.println(filename);
    }
    else
    {
        Serial.print("Failed to open: ");
        Serial.println(filename);
    }
}
void SoundManager::Play(const uint8_t *data, uint32_t size, bool header)
{
    _isSD = false;
    _source = data;
    _totalSize = size;
    if (header)
        _cursor = HEADER_SIZE;
    else
        _cursor = 0;

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
    {
        if (digitalRead(_en_pin) == LOW)
            Stop();

        return;
    }

    if (_isSD)
    {
        if (_activeStream == nullptr)
            return;
        // --- БЛОК ДЛЯ SD-КАРТЫ ---
        if (!audioBridge.readyA && _sdStream.isAvailable())
        {
            uint32_t r = _sdStream.read(audioBridge.bufferA, AudioBridge::BUF_SIZE);
            if (r > 0)
                audioBridge.readyA = true;
            else
                Stop();
        }
        if (!audioBridge.readyB && _sdStream.isAvailable())
        {
            uint32_t r = _sdStream.read(audioBridge.bufferB, AudioBridge::BUF_SIZE);
            if (r > 0)
                audioBridge.readyB = true;
            else
                Stop();
        }
    }
    else
    {
        // --- ТВОЙ СТАРЫЙ БЛОК ДЛЯ МАССИВА (Flash) ---
        if (!audioBridge.readyA && _cursor < _totalSize)
        {
            uint32_t toCopy = min(AudioBridge::BUF_SIZE, _totalSize - _cursor);
            memcpy(audioBridge.bufferA, _source + _cursor, toCopy);
            _cursor += toCopy;
            audioBridge.readyA = true;
        }
        if (!audioBridge.readyB && _cursor < _totalSize)
        {
            uint32_t toCopy = min(AudioBridge::BUF_SIZE, _totalSize - _cursor);
            memcpy(audioBridge.bufferB, _source + _cursor, toCopy);
            _cursor += toCopy;
            audioBridge.readyB = true;
        }
        if (_cursor >= _totalSize && !audioBridge.readyA && !audioBridge.readyB)
        {
            Stop();
        }
    }
}
void SoundManager::Stop()
{
    audioBridge.isPlaying = false;
    audioBridge.readyA = false;
    audioBridge.readyB = false;
    digitalWrite(_en_pin, HIGH); // Гасим усилитель
}

void SoundManager::Random_Play(const char *dirname)
{
    char filename[64];
    snprintf(filename, sizeof(filename), "Sound/%s/config", dirname);

    Serial.print("Собран путь: ");
    Serial.println(filename);
    int var = SDModule::getInstance().ReadValFromFile(filename);
    if (var > 0)
    {
        Serial.print("Прочитано var: ");
        Serial.println(var);
        snprintf(filename, sizeof(filename), "Sound/%s/var%d.wav", dirname, random(1, var + 1));
        Play(filename);
        Serial.print("Играем файл: ");
        Serial.println(filename);
    }
}