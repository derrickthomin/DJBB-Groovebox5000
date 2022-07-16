#pragma once
#include <vector>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include "Sequencer.h"
#include "Colors.h"
#include "bitmaps.h"

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128 // Change this to 96 for 1.27" OLED.

/*
**************************** Classes **********************************
************************************************************************
*/
class Screen
{
    public:
    Screen(char* Title, uint16_t TitleColor);
    char*    title;
    uint16_t titleColor;
    uint8_t  actionAreaWidth;
    uint8_t  actionAreaHeight;
    void     drawTitleBarMnu(void);
};

/*
*************************** Functions **********************************
************************************************************************
*/

void initOled(void);    
void initScreens(void);                                                          // Sets up OLED
void drawInfoBar(char * text, int16_t displayVal = -1);
void drawTitleBar(char * text, int16_t color);
void drawCurrentTitleBar(uint8_t menu_idx);
void infoBarSetStatus(bool status);
void eraseInfoBar(void);
void checkInfoBar(void);
void draw_sliders(int8_t valA, int8_t valB);                                      // Draw sliders on the screen
void update_sliders(int8_t valA, int8_t valB);
void update_sliders_thin(int8_t valA, int8_t valB);
void draw_pot_val_bottomscreen(uint8_t potNumber, uint8_t potVal);                // Draw pot values on bottom of the screen
void clear_pot_bottom_screen(uint8_t potNumber);                                  // Erase pot values on bottom of screen.
void drawNoteSymbol(uint16_t x, uint16_t y, uint8_t size = 1, int color = RED_2); // Draw note symbol at a certain location.
void drawCurrentTitleBar();

/*
***************** Helper / Testing Functions *******************
*/
void FUNderline(uint8_t startX, uint8_t startY, uint8_t length, uint16_t color);
uint32_t get_neopix_color_by_idx(int16_t colorsetIdx, int16_t colorShadeIdx);     // Interface for other modules to get specific colors
void z_showAllColors(void);
void z_displayUpdateTest(int x);
void z_funNoteAnimation(uint8_t xtraX = 0, uint8_t xtraY = 0, uint8_t size = 1);
void z_drawMascotBitmap(void);
