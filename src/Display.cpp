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

const std::vector<std::vector<uint16_t>> colors{reds, blues, greens, pinks, yellows, oranges, purples};  // All colors
const std::vector<std::vector<uint32_t>> all_colors_neopix {reds_neopix, blues_neopix, greens_neopix, pinks_neopix, yellows_neopix, oranges_neopix, purples_neopix};
bool infoBarActive = false; 
char * prevInfoText;
int16_t prevInfoVal;
uint32_t lastInfobarMillis = millis();          // when did we last show a new info bar
const uint16_t infoBarDispTime = 700;           // How many millis until the banner goes awasy
int16_t currentScreenIDX = 0;
/*
***************************   Menus   **********************************

                    Each represents a "screen". 

            Each should include:
                - A total refresh (draw the whole thing

************************************************************************
*/

/*
    Default menu (for now... ). This menu is where you can hold buttons to 
    change p-lock params, etc.
*/
// class Screen
// {
//     char*    title;
//     uint16_t titleColor;
//     uint8_t  actionAreaWidth;
//     uint8_t  actionAreaHeight;

// };
//std::vector<Screen> screens;

Screen::Screen(char* Title, uint16_t TitleColor)
{
    title = Title;
    titleColor = TitleColor;
}

// functions
void Screen::drawTitleBarMnu(void)                    // Draws the top title bar
{
    drawTitleBar(title, titleColor);
}
std::vector<Screen> screens;

/*
*************************** Helper variables **************************

              Cache off some values that we will use a lot

************************************************************************
*/
const uint8_t screen_middle = SCREEN_HEIGHT/2; 
const uint8_t screen_qtr_2_x = SCREEN_HEIGHT/4;
const uint8_t screen_qtr_3_x = screen_qtr_2_x * 2;
const uint8_t screen_qtr_4_x = screen_qtr_2_x * 3;
const uint8_t screen_bot_info_y = 95;                 // Below here we can show pot values, etc.
const uint8_t infoBarHeight     = 17;
const uint8_t infobarY          = infoBarHeight - 1;
const uint8_t infobarX          = 3;
const uint8_t infoBarLineWidth  = SCREEN_WIDTH - infobarX * 2;

// Menu Stuff
const uint8_t mnu_title_left_pad = 5;
const uint8_t mnu_title_colorblock_width = 15;

// Pot Stuff
const uint8_t pot_x_st       = 5;  // First pot position
const uint8_t pot_line_width = (SCREEN_WIDTH / 6);
const uint8_t pot_dot_radius = 3;
const uint8_t pot_line_y     = screen_bot_info_y + (SCREEN_HEIGHT - screen_bot_info_y)/2;
const uint8_t pot_val_y      = pot_line_y + pot_dot_radius + 5;
const uint8_t pot_label_y    = 98;
const uint8_t pot_label_ht   = 8;
  
// Slider stuff
const uint16_t sliderWidth = 13;    // in pixels - How wide the slider is
const uint16_t sliderHeight= 60;    // in pixels - How tall the slider is
const uint8_t  sliderSpacing = 15;
const uint8_t  sliderThinWidth = 3;
const uint8_t  sliderThinHeight = 30;
const uint8_t  slider_val_label_y = SCREEN_HEIGHT - sliderThinHeight - 10;
const uint8_t  slider_label_x1    = 30;
const uint8_t  slider_label_x2    = SCREEN_WIDTH/2;
const uint8_t  slider_labelbar_width = (SCREEN_WIDTH - (2*slider_label_x1))/2;

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

// Create new screens here.
void initScreens(void)
{
    screens.push_back(Screen("Step Edit", ORANGE_5));
    screens.push_back(Screen("Screen 2", YELLOW_5));
    screens.push_back(Screen("Screen 3", PURPLE_5));
}

void initOled(void)
{
    //oled.begin();
    oled.begin(16000000);
    oled.fillScreen(BLACK);
    oled.setRotation(2);
    drawCurrentTitleBar();
    //z_drawMascotBitmap();
}

void drawCurrentTitleBar(void)
{
    screens[currentScreenIDX].drawTitleBarMnu();
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

void update_sliders_thin(int8_t valA, int8_t valB)
{
    // clear prev
    oled.fillRect(0, SCREEN_HEIGHT - sliderThinHeight, sliderThinWidth, SCREEN_HEIGHT, BLACK);
    oled.fillRect(SCREEN_WIDTH - sliderThinWidth, SCREEN_HEIGHT - sliderThinHeight, sliderThinWidth, SCREEN_HEIGHT, BLACK);
    oled.fillRect(0, slider_val_label_y, 20, 10, BLACK);
    oled.fillRect(SCREEN_WIDTH - 18, slider_val_label_y, 20, 10, BLACK);
    //oled.fillRect(SCREEN_WIDTH - sliderThinWidth, SCREEN_HEIGHT - sliderThinHeight, sliderThinWidth, SCREEN_HEIGHT, BLACK);

    // Draw the rectangles
    uint8_t valAMapped = map(valA ,0,100,0,sliderThinHeight);
    uint8_t valBMapped = map(valB ,0,100,0,sliderThinHeight);

    oled.fillRect(0, SCREEN_HEIGHT - valAMapped, sliderThinWidth, valAMapped, GREEN_2);
    oled.fillRect(SCREEN_WIDTH - sliderThinWidth, SCREEN_HEIGHT - valBMapped, sliderThinWidth, valBMapped, BLUE_2);

    //Draw the numbers
    oled.setTextSize(1);
    oled.setTextColor(GREEN_5);
    oled.setCursor(0, slider_val_label_y);
    oled.println(valA);

    oled.setTextColor(BLUE_5);
    oled.setCursor(SCREEN_WIDTH - 18, slider_val_label_y);
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
void update_pot_val(uint8_t potNumber, uint8_t potVal)
{
    if (potNumber < 1) potNumber = 1;
    if (potNumber > 4) potNumber = 4;

    clear_pot_val(potNumber); 
    uint32_t pot_color = yellows[potNumber];

    // Set coordinates for pot 1
    uint8_t x    = pot_x_st;
    uint8_t y    = screen_bot_info_y + (SCREEN_HEIGHT - screen_bot_info_y)/2;
    uint8_t xDot = map(potVal, 0, 100, 0, pot_line_width);

    // Now modify x for pots that are not 1
    if (potNumber > 1){
        potNumber = potNumber -1;
        x = x + (screen_qtr_2_x * potNumber);
    }
    oled.drawFastHLine(x, y, pot_line_width, pot_color);
    oled.fillCircle(x + xDot,y,pot_dot_radius, pot_color);
    oled.setCursor(x, pot_val_y);
    oled.setTextColor(pot_color);
    oled.print(potVal);
}

void update_pot_label(uint8_t potNumber, char* label)
{
    if (potNumber < 1) potNumber = 1;
    if (potNumber > 4) potNumber = 4;
    clear_pot_label(potNumber); 

    uint32_t color = yellows[potNumber];
    uint8_t  x     = pot_x_st - pot_dot_radius + 2;
    if (potNumber > 1){
        potNumber = potNumber -1;
        x = x + (screen_qtr_2_x * potNumber);
    }
    oled.setCursor(x, pot_label_y);
    oled.setTextColor(color);
    oled.println(label);
}

void clear_pot_label(uint8_t potNumber)
{
    uint8_t x = pot_x_st - pot_dot_radius + 2;
    if (potNumber > 1){
        potNumber = potNumber -1;
        x = x + (screen_qtr_2_x * potNumber);
    }
    oled.fillRect(x, pot_label_y, pot_line_width + pot_dot_radius + 1, pot_label_ht, BLACK);
}

// Erase existing pot drawing
void clear_pot_val(uint8_t potNumber)
{
    if (potNumber < 1) potNumber = 1;
    if (potNumber > 4) potNumber = 4;

    // Set coordinates for pot 1
    uint8_t x = pot_x_st - (pot_dot_radius);
    uint8_t y = pot_line_y - pot_dot_radius - 1;
    uint8_t width = pot_line_width + (pot_dot_radius * 2) + 1;
    uint8_t height = SCREEN_HEIGHT - y;

    // Now modify x for pots that are not 1
    if (potNumber > 1){   
        potNumber = potNumber -1;
        x = x + (screen_qtr_2_x * potNumber);
    }
    oled.fillRect(x, y, width, height, BLACK);
   //oled.drawRect(x, y, width, height, PURPLE_4);
}

void update_slider_label(uint8_t sliderNumber, char* label)
{
    clear_slider_label(sliderNumber);

    if (sliderNumber == 1){
        oled.setCursor(slider_label_x1, slider_val_label_y);
        oled.setTextColor(GREEN_5);
        oled.print(label);
    }

    if (sliderNumber == 2){
        oled.setCursor(slider_label_x2, slider_val_label_y);
        oled.setTextColor(BLUE_5);
        oled.print(label);
    }
}

void clear_slider_label(uint8_t sliderNumber)
{
    uint8_t x;
    if (sliderNumber ==1) x = slider_label_x1;
    if (sliderNumber ==2) x = SCREEN_WIDTH/2;

    oled.fillRect(x, slider_val_label_y, slider_labelbar_width, 10, BLACK);
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

    if (prevInfoText == text && prevInfoVal == displayVal && infoBarActive) return;  

    eraseInfoBar();
    oled.setTextColor(WHITE);
    oled.setCursor(6, (infoBarHeight/2 - 1));
    oled.print(text);
    oled.print(displayVal);
    prevInfoText = text;
    FUNderline(infobarX, infobarY, infoBarLineWidth, YELLOW_5);
    prevInfoVal = displayVal;
    infoBarSetStatus(true);
}

void drawTitleBar(char * text, uint16_t color)
{
    uint8_t x = mnu_title_colorblock_width + mnu_title_left_pad;
    eraseInfoBar();
    oled.drawRoundRect(2,2,SCREEN_WIDTH-4, infoBarHeight, 3, color);
    oled.fillRoundRect(2,2, mnu_title_colorblock_width, infoBarHeight, 3, color);
    oled.setTextColor(WHITE);
    oled.setCursor(x, infoBarHeight/2);
    oled.print(text);
}

// void drawCurrentTitleBar(void)
// {
//     drawTitleBar(screens[currentScreenIDX].title, screens[currentScreenIDX].titleColor);
// }

void eraseInfoBar(void)
{
    oled.fillRect(0,0, SCREEN_WIDTH, infoBarHeight + 3, BLACK);
}

void infoBarSetStatus(bool status)
{
    infoBarActive = status;
}

// check if info bar needs cleared
void checkInfoBar(void)
{
    if (infoBarActive && (millis() - lastInfobarMillis > infoBarDispTime))
    {
        lastInfobarMillis = millis();
        drawCurrentTitleBar();
        infoBarSetStatus(false);
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
    return   color;
}

void FUNderline(uint8_t startX, uint8_t startY, uint8_t length, uint16_t color)
{
    const uint8_t height = 3;
    const uint8_t spacing = 4;
    uint8_t currentX = startX;
    uint8_t endX = startX + abs(length);
    uint8_t currentY = startY;

    if (endX > SCREEN_WIDTH) endX = SCREEN_WIDTH;

    while (currentX < endX)
    {
        uint8_t nextX;
        uint8_t nextY;
        
        nextX = currentX + spacing;
        (currentY <= startY) ? nextY = currentY + height : nextY = currentY - height;
        if (nextX > endX) break;
        oled.drawLine(currentX, currentY, nextX, nextY, YELLOW_5);
        currentX = nextX;
        currentY = nextY;
        endX++;
    } 
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

void z_drawMascotBitmap(void)
{
    uint8_t x = 0;
    uint8_t y =0;
    while (x < SCREEN_WIDTH - 30)
    {
        y = 0;
        while (y < SCREEN_HEIGHT - 30)
        {
            oled.drawRGBBitmap(x,y,mascot_30px,30,30);
            y += 30;
            delay(50);
        }
        x += 30;
    }
}

