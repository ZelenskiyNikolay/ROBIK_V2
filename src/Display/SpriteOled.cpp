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
       
    default:
        break;
    }
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
    display->drawLine(78, 6,108, 2,SSD1306_WHITE);
    display->drawLine(78, 7,108, 3,SSD1306_WHITE);

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