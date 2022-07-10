/*

Display stuff. Classes / functions related to screen updates / neopixel updates, etc.

*/
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include "ssd1351.h"
#include <SPI.h>
#include <Audio.h>
#include "Display.h"

#define SCLK_PIN 13
#define MOSI_PIN 11
#define DC_PIN   30
#define CS_PIN   26
#define RST_PIN  -1

Adafruit_SSD1351 oled = Adafruit_SSD1351(CS_PIN, DC_PIN, RST_PIN);  
//Adafruit_SSD1351 oled = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, CS_PIN, DC_PIN, MOSI_PIN, SCLK_PIN, RST_PIN); 
std::vector<std::vector<uint16_t>> colors{reds, blues, greens, pinks, yellows, oranges, purples};  // All colors

/*
*************************** Helper variables **************************

              Cache off some values that we will use a lot

************************************************************************
*/

const uint8_t screen_middle = SCREEN_HEIGHT/2; 
const uint8_t screen_qtr_2_x = SCREEN_HEIGHT/4;
const uint8_t screen_qtr_3_x = screen_qtr_2_x * 2;
const uint8_t screen_qtr_4_x = screen_qtr_2_x * 3;
const uint8_t screen_bot_info_y = 80;                 // Below here we can show pot values, etc.
const uint8_t screen_pot_radius = 8;

// Slider stuff
const uint16_t sliderWidth = 13;   // in pixels - How wide the slider is
const uint16_t sliderHeight= 60;   // in pixels - How tall the slider is
const uint8_t  sliderSpacing = 15;

// Draw slider A outline
const uint16_t sliderA_x = screen_middle - sliderWidth - (sliderSpacing/2);    
const uint16_t sliderA_y = 10;
uint16_t       sliderAline_y = sliderA_y + (sliderHeight/2);        // Default to the middle

// Draw slider B outline
const uint16_t sliderB_x = sliderA_x + sliderSpacing + sliderWidth;
const uint16_t sliderB_y = sliderA_y;
uint16_t       sliderBline_y = sliderB_y +(sliderHeight/2);

/*
*************************** Functions **************************
****************************************************************
*/

void initOled(void)
{
    oled.begin(16000000);
    oled.fillScreen(BLACK);
    oled.setRotation(2);
    // djt add a cool splasher
}

void draw_sliders(int8_t valA, int8_t valB)
{
    // SPI.setMOSI(MOSI_PIN);
    // SPI.setSCK(SCLK_PIN);
    oled.fillScreen(BLACK);

    // Map the pot values to pixel y value
    uint8_t valALineY = sliderA_y + sliderHeight - map(valA, -50, 50, 0, sliderHeight);  
    uint8_t valBLineY =  sliderB_y + sliderHeight - map(valB, -50, 50, 0, sliderHeight);
    Serial.println(valALineY);

    // Draw the rectangles
    oled.drawRect(sliderA_x, sliderA_y, sliderWidth, sliderHeight, BLUE_4);
    oled.drawRect(sliderB_x, sliderB_y, sliderWidth, sliderHeight, GREEN_4); 

    // draw the mid line
    oled.drawFastHLine(sliderA_x, sliderA_y + sliderHeight/2, sliderWidth, BLUE_2);
    oled.drawFastHLine(sliderB_x, sliderB_y + sliderHeight/2, sliderWidth, GREEN_2);

    // draw the line representing the pot value
    oled.drawFastHLine(sliderA_x - 5, valALineY, sliderWidth + 5, BLUE_5);
    oled.drawFastHLine(sliderB_x, valBLineY, sliderWidth + 5, GREEN_5);

    // Draw the angled line to the value numebr
    uint8_t pointerLineA_x1 = sliderA_x - 20;
    uint8_t pointerLineA_y1 = sliderA_y + sliderHeight/2;
    uint8_t pointerLineA_x2 = sliderA_x - 5;
    uint8_t pointerLineA_y2 = valALineY;

    uint8_t pointerLineB_x1 = sliderB_x + sliderWidth + 20;
    uint8_t pointerLineB_y1 = valBLineY;
    uint8_t pointerLineB_x2 = sliderB_x + sliderWidth + 5;
    uint8_t pointerLineB_y2 = sliderB_y + sliderHeight/2;

    oled.drawLine(pointerLineA_x1, pointerLineA_y1, pointerLineA_x2, pointerLineA_y2, BLUE_4);
    oled.drawLine(pointerLineB_x1, pointerLineB_y2, pointerLineB_x2, pointerLineB_y1, GREEN_4);

    // Draw the numbers
    oled.setTextSize(1);
    oled.setTextColor(BLUE_5);
    oled.setCursor(pointerLineA_x1 - 15, pointerLineA_y1);
    oled.println(valA);

    oled.setTextColor(GREEN_5);
    oled.setCursor(pointerLineB_x2 + 15, pointerLineB_y2);
    oled.println(valB);

}

// Draws values of the underscreen pots on the bottom portion of the screen
//      Pot number = 1 - 4
void draw_pot_val_bottomscreen(uint8_t potNumber, uint8_t potVal)
{
    if (potNumber < 1) potNumber = 1;
    if (potNumber > 4) potNumber = 4;

    clear_pot_bottom_screen(potNumber); 
    uint32_t pot_color = yellows[potNumber];

    // Set coordinates for pot 1
    uint8_t x = screen_qtr_2_x/2;
    uint8_t y = screen_bot_info_y + (SCREEN_HEIGHT - screen_bot_info_y)/2;

    // Now modify x for pots that are not 1
    if (potNumber > 1)
    {   
        potNumber = potNumber -1;
        x = x + (screen_qtr_2_x * potNumber);
    }

    oled.drawCircle(x, y, screen_pot_radius, pot_color);
    oled.setCursor(x - 5, y + screen_pot_radius + 2);
    oled.setTextColor(pot_color);
    oled.print(potVal);
}

// Erase existing pot drawing
void clear_pot_bottom_screen(uint8_t potNumber)
{
    if (potNumber < 1) potNumber = 1;
    if (potNumber > 4) potNumber = 4;

    // Set coordinates for pot 1
    uint8_t x = 0;
    uint8_t y = screen_bot_info_y;
    uint8_t width = screen_qtr_2_x;
    uint8_t height = SCREEN_HEIGHT - screen_bot_info_y;

    // Now modify x for pots that are not 1
    if (potNumber > 1)
    {   
        potNumber = potNumber -1;
        x = x + (screen_qtr_2_x * potNumber);
    }

    oled.fillRect(x, y, width, height, BLACK);
}

void drawNoteSymbol(uint16_t x, uint16_t y, uint8_t size = 1, int color)
{
    uint16_t radius = size * 2;
    uint8_t lineX_start = x + radius;
    uint8_t line_height = 10 * size;

    oled.fillCircle(x, y, radius, color);
    oled.drawFastVLine(lineX_start, y - line_height, line_height, color);
}
/*
*************************** Utility / Testing **************************

                      Functions for testing n stuff

************************************************************************
*/

void z_showAllColors(void)
{
    // reds, blues, greens, pinks, yellows, oranges, purples
    // Loop array of all colors
    uint8_t rec_width = 12;
    uint8_t rec_height = 10;
    uint8_t y_min = 5;
    for (uint16_t i = 0; i < colors.size(); i++)
    {
        uint16_t y = y_min + i * 18;
        oled.setCursor(2, y);
        oled.setTextSize(1);
        oled.setTextColor(colors[i][4]);
        oled.println("Color: ");
        // Loop each color variation
        for (uint16_t j = 0; j < colors[i].size(); j++)
        {
            uint16_t color = colors[i][j];
            uint16_t x = 25 + (j + 1) * (rec_width + 4);
            oled.fillRect(x, y, rec_width, rec_height, color);
            delay(100);
        }
    }
}

void z_displayUpdateTest(int x)
{
    oled.fillRect(20, 20, 70, 70, BLACK);
    oled.setCursor(50, 50);
    oled.setTextColor(RED_1);
    oled.println(String(x));
}

void  z_funNoteAnimation(uint8_t xtraX = 0, uint8_t xtraY = 0, uint8_t size = 1)
{
    uint8_t currentX = 10 + xtraX;
    uint8_t currentY = 20 + xtraY;
    uint8_t PrevX = currentX;
    uint8_t prevY = currentY;
    uint8_t coloridx = 0;
    uint8_t colorAryIDX = 0;   // trakc the color bank

    for (uint8_t i = 0; i < 300; i ++)
    {
        PrevX = currentX;
        prevY = currentY;
        currentX++;
        currentY++;

        //drawNoteSymbol(PrevX, prevY, size, BLACK);
        drawNoteSymbol(currentX, currentY, size, colors[colorAryIDX][coloridx]);
        coloridx++;

        if (coloridx > 4)
        {
            colorAryIDX++;
            coloridx = 0;
        }

        if (colorAryIDX > colors.size()-1) colorAryIDX = 0;
        delay(200);
}
}

