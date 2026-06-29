#include <Arduino.h>
#include "hardware/pwm.h"

class MyServo
{
private:
    uint _pin;
    uint _slice_num;
    uint _chan;
    uint32_t _minMicros;
    uint32_t _maxMicros;
    bool _attached = false;

    float timer = 0.0f;
    bool isRotateComplite = true;

    int _degreesNow;

    // Частота процессора Pico по умолчанию — 125 МГц.
    // При делителе clkdiv = 64.0f, частота ШИМ-таймера = 1 953 125 Гц.
    // Для периода 20 мс (50 Гц) точное значение TOP = (20000 * 1.953125) - 1 = 39061.5f -> 39062.
    const float REAL_PWM_COEF = 1.95315f; 

    uint32_t microsToLevel(uint32_t microseconds)
    {
        // Честный перевод микросекунд в тики таймера без округлений в нули
        return (uint32_t)((float)microseconds * REAL_PWM_COEF);
    }

public:
    int GetDegreesNow(){return _degreesNow;}
    bool IsRotateComplite() { return isRotateComplite; }
    void UpdateServo(float dt)
    {
        if (timer > 0 && !isRotateComplite)
        {
            timer -= dt;

            if(timer<300)
                stop();
            return;
        }
        isRotateComplite = true;
    }
    bool attach(uint pin, uint32_t minMicros, uint32_t maxMicros)
    {
        _pin = pin;
        _minMicros = minMicros;
        _maxMicros = maxMicros;
        // Стандартные лимиты Arduino для SG90: 544 мкс и 2400 мкс
        // Твои калиброванные лимиты: 600 мкс и 2350 мкс (передавай их при вызове)
        _minMicros = minMicros;
        _maxMicros = maxMicros;

        _slice_num = pwm_gpio_to_slice_num(_pin);
        _chan = pwm_gpio_to_channel(_pin);

        gpio_set_function(_pin, GPIO_FUNC_PWM);

        pwm_config config = pwm_get_default_config();
        pwm_config_set_clkdiv(&config, 64.0f);
        pwm_config_set_wrap(&config, 39062); // Период ровно 20мс

        pwm_init(_slice_num, &config, true);

        _attached = true;
        isRotateComplite = true;
        return true;
    }

    // Поворот на угол в градусах (0 - 180)
    void write(int degrees)
    {
        if (degrees < 0)
            degrees = 0;
        if (degrees > 180)
            degrees = 180;

        if (!_attached)
            play();
        
        // Линейно интерполируем градусы в длительность импульса в микросекундах
        // uint32_t micros = _minMicros + ((_maxMicros - _minMicros) * degrees) / 180;
        uint32_t micros = _minMicros + ((_maxMicros - _minMicros) * (uint32_t)degrees) / 180;
        // Переводим микросекунды в уровень сравнения ШИМ и записываем в регистр
        pwm_set_chan_level(_slice_num, _chan, microsToLevel(micros));

        _degreesNow = degrees;
        timer = 400;
        isRotateComplite = false;
    }

    // Альтернативный метод: запись напрямую в микросекундах (для точной калибровки)
    void write_mic(uint32_t microseconds)
    {
        if (microseconds < _minMicros)
            microseconds = _minMicros;
        if (microseconds > _maxMicros)
            microseconds = _maxMicros;
        if (!_attached)
            play();
        pwm_set_chan_level(_slice_num, _chan, microsToLevel(microseconds));
        
        timer = 800;
        isRotateComplite = false;
    }

    void stop()
    {
        if (!_attached)// || isRotateComplite)
            return;
        // Полностью выключаем аппаратный слайс ШИМ для этого пина
        pwm_set_enabled(_slice_num, false);

        // Дополнительно прижимаем пин к земле, чтобы на линии не было наводок
        gpio_set_function(_pin, GPIO_FUNC_SIO);
        gpio_set_dir(_pin, GPIO_OUT);
        gpio_put(_pin, 0);

        _attached = false;
    }

    // СТАРТ генерации ШИМ (Аналог attach / возобновление работы)
    void play()
    {
        if (_attached) return;
        // Возвращаем пину функцию аппаратного ШИМ
        gpio_set_function(_pin, GPIO_FUNC_PWM);
        // Включаем аппаратный слайс ШИМ
        pwm_set_enabled(_slice_num, true);

        _attached = true;
    }
};