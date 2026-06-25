#pragma once
#include <Arduino.h>

inline unsigned char PROGMEM Zero_bitmap[] = {
    0b00111100, //  ****
    0b00100100, //  *  *
    0b00100100, //  *  *
    0b00100100, //  *  *
    0b00100100, //  *  *
    0b00100100, //  *  *
    0b00100100, //  *  *
    0b00111100  //  ****
};
inline unsigned char PROGMEM One_bitmap[] = {
    0b00111100, //   ****
    0b00011000, //    **
    0b00011000, //    **
    0b00011000, //    **
    0b00011000, //    **
    0b00011000, //    **
    0b00011000, //    **
    0b00111100  //   ****
};
inline unsigned char PROGMEM One_Mini_bitmap[] = {
    0b11100000, // ***
    0b01000000, //  *
    0b01000000, //  *
    0b11100000, // ***
};
inline unsigned char PROGMEM Zero_Mini_bitmap[] = {
    0b11100000, // ***
    0b10100000, // * *
    0b10100000, // * *
    0b11100000, // ***
};