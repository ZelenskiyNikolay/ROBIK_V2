#include "SpriteOled.h"

void SpriteOled ::Draw(Emotions emotions)
{
    switch (emotions)
    {
    case NORMAL:
        drawEyes_Normal();
        break;

    case SLEEPY:
        drawEyes_Sleepy();
        break;

    case ANGRY:
        drawEyes_Angry();
        break;

    case SURPRISED:

        break;

    case HAPPY:

        break;

    case SAD:

        break;

    case LEFT_LOOK:

        break;

    case RIGHT_LOOK:

        break;

    case BLINK:
        drawEyes_Blink();
        break;

    case SHOW_TIME:

        break;
    case MICROPHONE:
        drawMicrophone();
        break;
    default:
        break;
    }
}

void SpriteOled::drawMicrophone()
{
    display->clear();

    int centerX = 64;   // Центр экрана по горизонтали
    int topY = 10;      // Верхняя точка микрофона
    int micWidth = 16;  // Ширина капсулы
    int micHeight = 26; // Высота капсулы

    display->drawCircle(centerX, topY + micWidth / 2, micWidth / 2, SSD1306_WHITE);

    display->drawCircle(centerX, topY + micHeight - micWidth / 2, micWidth / 2, SSD1306_WHITE);

    display->fillRect(centerX - micWidth / 2, topY + micWidth / 2, micWidth, micHeight - micWidth, SSD1306_WHITE);

    int gap = 4;                                        // Промежуток между линиями
    int gridStartY = topY + micWidth / 2 + 1;           // Начало сетки чуть ниже верха
    int gridEndY = topY + micHeight - micWidth / 2 - 1; // Конец чуть выше низа

    for (int y = gridStartY; y < gridEndY; y += gap)
    {
        display->drawLine(centerX - micWidth / 4, gridStartY, centerX - micWidth / 4, gridEndY, SSD1306_BLACK);
        display->drawLine(centerX + micWidth / 4, gridStartY, centerX + micWidth / 4, gridEndY, SSD1306_BLACK);
    }
    int stemWidth = 4;
    int stemHeight = 12;
    int stemTopY = topY + micHeight + 2; // Чуть ниже капсулы
    display->fillRect(centerX - stemWidth / 2, stemTopY, stemWidth, stemHeight, SSD1306_WHITE);

    int baseWidth = 32;
    int baseBottomY = 60; // Почти у нижнего края экрана

    display->fillRect(centerX - baseWidth / 2, baseBottomY, baseWidth, 3, SSD1306_WHITE);
}
void SpriteOled::drawEyes_Normal()
{
    display->clear();

    display->drawCircle(35, 20, 15, SSD1306_WHITE);
    display->drawCircle(93, 20, 15, SSD1306_WHITE);

    display->fillCircle(35, 20, 9, SSD1306_WHITE);
    display->fillCircle(93, 20, 9, SSD1306_WHITE);

    display->fillRect(35, 62, 60, 4, SSD1306_WHITE);
}
void SpriteOled::drawEyes_Sleepy()
{
    display->clear();

    display->fillCircle(93, 20, 9, SSD1306_WHITE);
    display->fillCircle(35, 20, 9, SSD1306_WHITE); // зрачок

    // Сонные веки (масштабированные)
    display->fillRect(20, 9, 30, 10, SSD1306_BLACK); // левый
    display->fillRect(78, 9, 30, 10, SSD1306_BLACK); // правый

    display->drawCircle(35, 20, 15, SSD1306_WHITE); // наружный круг
    display->drawCircle(93, 20, 15, SSD1306_WHITE);

    // Рот (как в Normal)
    display->fillRect(35, 62, 60, 4, SSD1306_WHITE);
}
void SpriteOled::drawEyes_Angry()
{
    display->clear();

    display->drawLine(20, 2, 50, 6, SSD1306_WHITE);
    display->drawLine(20, 3, 50, 7, SSD1306_WHITE);
    display->drawLine(78, 6, 108, 2, SSD1306_WHITE);
    display->drawLine(78, 7, 108, 3, SSD1306_WHITE);

    // Зрачки (сначала)
    display->fillCircle(35, 20, 9, SSD1306_WHITE);
    display->fillCircle(93, 20, 9, SSD1306_WHITE);

    // Наружные круги глаз (сверху)
    display->drawCircle(35, 20, 15, SSD1306_WHITE);
    display->drawCircle(93, 20, 15, SSD1306_WHITE);

    // Злой рот (широкий, как в Normal)
    display->fillRect(35, 60, 60, 4, SSD1306_WHITE);
}

void SpriteOled::drawEyes_Blink()
{
    display->clear();

    // Глаза закрыты
    display->fillRect(20, 18, 30, 4, SSD1306_WHITE); // левый
    display->fillRect(78, 18, 30, 4, SSD1306_WHITE); // правый

    display->fillRect(35, 62, 60, 4, SSD1306_WHITE);
}