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
#include <vector>

#define SCLK_PIN 13
#define MOSI_PIN 11
#define DC_PIN   30
#define CS_PIN   26
#define RST_PIN  -1

Adafruit_SSD1351 oled = Adafruit_SSD1351(CS_PIN, DC_PIN, RST_PIN);  

// Test new color lib
//typedef ssd1351::LowColor Color;
//auto oled = ssd1351::SSD1351<Color, ssd1351::SingleBuffer, 128, 128>();

const std::vector<std::vector<uint16_t>> colors{reds, blues, greens, pinks, yellows, oranges, purples};  // All colors
const std::vector<std::vector<uint32_t>> all_colors_neopix {reds_neopix, blues_neopix, greens_neopix, pinks_neopix, yellows_neopix, oranges_neopix, purples_neopix};
bool infoBarActive = false; 
char * prevInfoText;
int16_t prevInfoVal;
uint32_t lastInfobarMillis =         millis();   // when did we last show a new info bar
const uint16_t infoBarDispTime = 700;           // How many millis until the banner goes awasy

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
const uint8_t infoBarHeight     = 15;
  

// Slider stuff
const uint16_t sliderWidth = 13;    // in pixels - How wide the slider is
const uint16_t sliderHeight= 60;    // in pixels - How tall the slider is
const uint8_t  sliderSpacing = 15;

// Draw slider A outline
const uint16_t sliderA_x = screen_middle - sliderWidth - (sliderSpacing/2);    
const uint16_t sliderA_y = 10;
uint16_t       sliderAline_y = sliderA_y + (sliderHeight/2);        // Default to the middle

// Draw slider B outline
const uint16_t sliderB_x = sliderA_x + sliderSpacing + sliderWidth;
const uint16_t sliderB_y = sliderA_y;
uint16_t       sliderBline_y = sliderB_y +(sliderHeight/2);

// Track where numbers go
const uint8_t screen_slider_val_xl = sliderA_x - 20 - 20;
const uint8_t screen_slider_val_xr = sliderB_x + sliderWidth + 5 + 20;  
const uint8_t screen_slider_val_yl = sliderA_y + sliderHeight/2;
const uint8_t screen_slider_val_yr = sliderB_y + sliderHeight/2;   

// Tracking variables.... need to know previous screen values to erase efficeintly
uint8_t valA_prev;
uint8_t valB_prev;
uint8_t valALineY_prev;
uint8_t valBLineY_prev;
uint8_t pointerLineA_x1_prev;
uint8_t pointerLineA_y1_prev;
uint8_t pointerLineA_x2_prev;
uint8_t pointerLineA_y2_prev;

uint8_t pointerLineB_x1_prev;
uint8_t pointerLineB_y1_prev;
uint8_t pointerLineB_x2_prev;
uint8_t pointerLineB_y2_prev;

/*
*************************** Functions **************************
****************************************************************
*/

void initOled(void)
{
    //oled.begin();
    oled.begin(16000000);
    oled.fillScreen(BLACK);
    oled.setRotation(2);
    // z_showAllColors();
    // oled.drawCircle(50,50,10,BLUE_3);
    //oled.updateScreen();
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

void update_sliders(int8_t valA, int8_t valB)
{
    // Map the pot values to pixel y value
    uint8_t valALineY = sliderA_y + sliderHeight - map(valA, -50, 50, 0, sliderHeight);  
    uint8_t valBLineY =  sliderB_y + sliderHeight - map(valB, -50, 50, 0, sliderHeight);

    // draw the line representing the pot value
    oled.drawFastHLine(sliderA_x - 5, valALineY_prev, sliderWidth + 5, BLACK);
    oled.drawFastHLine(sliderB_x, valBLineY_prev, sliderWidth + 5, BLACK);
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

    oled.drawLine(pointerLineA_x1_prev, pointerLineA_y1_prev, pointerLineA_x2_prev, pointerLineA_y2_prev, BLACK);
    oled.drawLine(pointerLineB_x1_prev, pointerLineB_y2_prev, pointerLineB_x2_prev, pointerLineB_y1_prev, BLACK);
    oled.drawLine(pointerLineA_x1, pointerLineA_y1, pointerLineA_x2, pointerLineA_y2, BLUE_4);
    oled.drawLine(pointerLineB_x1, pointerLineB_y2, pointerLineB_x2, pointerLineB_y1, GREEN_4);

    // Draw the numbers
    oled.fillRect(screen_slider_val_xl, screen_slider_val_yl, 20, 10, BLACK);
    oled.fillRect(screen_slider_val_xr, screen_slider_val_yr, 20, 10, BLACK);

    oled.setTextSize(1);

    oled.setTextColor(BLUE_5);
    oled.setCursor(screen_slider_val_xl, screen_slider_val_yl);
    oled.println(valA);

    oled.setTextColor(GREEN_5);
    oled.setCursor(screen_slider_val_xr, screen_slider_val_yr);
    oled.println(valB);

    // Save of prev values for erasing
    valA_prev = valA;
    valB_prev = valB;
    valALineY_prev = valALineY;
    valBLineY_prev = valBLineY;
    pointerLineA_x1_prev = pointerLineA_x1;
    pointerLineA_y1_prev = pointerLineA_y1;
    pointerLineA_x2_prev = pointerLineA_x2;
    pointerLineA_y2_prev = pointerLineA_y2;
    pointerLineB_x1_prev = pointerLineB_x1;
    pointerLineB_y1_prev = pointerLineB_y1;
    pointerLineB_x2_prev = pointerLineB_x2;
    pointerLineB_y2_prev = pointerLineB_y2;

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
    //oled.updateScreen();
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

void drawInfoBar(char * text, int16_t displayVal = -1)
{
    lastInfobarMillis = millis();   // Reset timer... still hodling the button or whatever.

    if (prevInfoText == text && prevInfoVal == displayVal) return;  

    oled.fillRoundRect(2,2,SCREEN_HEIGHT-4, infoBarHeight, 3, BLACK);
    oled.fillRoundRect(2,2,SCREEN_HEIGHT-4, infoBarHeight, 3, PURPLE_5);
    oled.setTextColor(BLACK);
    oled.setCursor(6, infoBarHeight/2);
    oled.print(text);
    oled.print(displayVal);
    infoBarSetStatus(true);
    prevInfoText = text;
    prevInfoVal = displayVal;
}

void eraseInfoBar(void)
{
    oled.fillRoundRect(2,2,SCREEN_HEIGHT-4, infoBarHeight, 3, BLACK);
}

void infoBarSetStatus(bool status)
{
    infoBarActive = status;
}

// check if info bar needs cleared
void checkInfoBar(void)
{
    if (millis() - lastInfobarMillis > infoBarDispTime)
    {
        lastInfobarMillis = millis();
        eraseInfoBar();
    }
}

/*
*************************** Utility / Testing **************************

                      Functions for testing n stuff

************************************************************************
*/
uint32_t get_neopix_color_by_idx(int16_t colorsetIdx, int16_t colorShadeIdx)
{
    uint32_t color = all_colors_neopix[colorsetIdx][colorShadeIdx];
    return color;
}

void z_showAllColors(void)
{
    // reds, blues, greens, pinks, yellows, oranges, purples
    // Loop array of all colors
    uint8_t rec_width = 12;
    uint8_t rec_height = 10;
    uint8_t y_min = 5;
    for (uint16_t i = 0; i < all_colors_neopix.size(); i++)
    {
        uint16_t y = y_min + i * 18;
        oled.setCursor(2, y);
        oled.setTextSize(1);
        oled.setTextColor(all_colors_neopix[i][4]);
        oled.println("Color: ");
        // Loop each color variation
        for (uint16_t j = 0; j < all_colors_neopix[i].size(); j++)
        {
            uint16_t color = all_colors_neopix[i][j];
            uint16_t x = 25 + (j + 1) * (rec_width + 4);
            oled.fillRect(x, y, rec_width, rec_height, color);
            delay(50);
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

