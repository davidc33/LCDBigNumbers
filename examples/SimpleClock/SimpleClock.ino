/*
 *  SimpleClock.cpp
 *
 *  Shows a (fast) running clock with big numbers on a 2004 LCD.
 *
 *  Copyright (C) 2022  Armin Joachimsmeyer
 *  armin.joachimsmeyer@gmail.com
 *
 *  This file is part of LCDBigNumbers https://github.com/ArminJo/LCDBigNumbers.
 *
 *  LCDBigNumbers is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/gpl.html>.
 *
 */

#include <Arduino.h>

/*
 * Choose your display
 */
//#define USE_PARALLEL_2004_LCD // Is default
//#define USE_PARALLEL_1602_LCD
//#define USE_SERIAL_2004_LCD
//#define USE_SERIAL_1602_LCD
#include "LCDBigNumbers.hpp" // Include sources for LCD big number generation

#if defined(USE_PARALLEL_LCD)
LiquidCrystal myLCD(2, 3, 4, 5, 6, 7); // Depends on your actual connections
//LiquidCrystal myLCD(4, 5, 6, 7, 8, 9); // Sample connections starting at pin 4
#else
#define LCD_I2C_ADDRESS 0x27    // Default LCD address is 0x27 for a 20 chars and 4 line / 2004 display
LiquidCrystal_I2C myLCD(LCD_I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS); // LCD_COLUMNS and LCD_ROWS are set by LCDBigNumbers.hpp depending on the defined display
#endif

LCDBigNumbers bigNumberLCD(&myLCD, 3, 3, 1); // Use 3x3 numbers, 1. variant
//LCDBigNumbers bigNumberLCD(&myLCD, 3, 4, 1); // Use 3x4 numbers, 1. variant

void setup() {
    Serial.begin(115200);
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__));

#if defined(USE_PARALLEL_LCD)
    myLCD.begin(LCD_COLUMNS, LCD_ROWS); // LCD_COLUMNS and LCD_ROWS are set by LCDBigNumbers.hpp depending on the defined display
#else
    myLCD.init();
    myLCD.clear();
    myLCD.backlight();
#endif
    bigNumberLCD.begin(); // Creates custom character used for generating big numbers
    myLCD.setCursor(0, 0);
    myLCD.print(F("Clock demo"));
    myLCD.setCursor(0, 1);
    myLCD.print(__DATE__);
    delay(2000);
    myLCD.clear();
}

int Minutes = 48;
int Hours = 11;
unsigned long lastMillisOfClockUpdate = 0;

void loop() {
    if (millis() - lastMillisOfClockUpdate > 1000) { // Fast mode :-), do not wait 60 seconds
        lastMillisOfClockUpdate += 1000;
        /*
         * Print hours
         */
        bigNumberLCD.setBigNumberCursor(2);
        if (Hours < 10) {
            bigNumberLCD.print('0');
        }
        bigNumberLCD.print(Hours);

        /*
         * Do blinking colon
         */
        if (Minutes % 2 == 1) {
            bigNumberLCD.print(F(": "));
        } else {
            bigNumberLCD.print(F("  "));
        }

        /*
         * Print minutes
         */
        if (Minutes < 10) {
            bigNumberLCD.print('0');
        }
        bigNumberLCD.print(Minutes);

        /*
         * Go to next minute
         */
        Minutes++;
        if (Minutes >= 60) {
            Minutes = 0;
            Hours++;
            if (Hours >= 24) {
                Hours = 0;
            }
        }
    }
}
