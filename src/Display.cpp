#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include "ssd1351.h"
#include <SPI.h>
#include <Audio.h>
#include "Display.h"
#include <vector>
#include "Inputs.h"

#define SCLK_PIN 13
#define MOSI_PIN 11
#define DC_PIN   30
#define CS_PIN   26
#define RST_PIN  -1

Adafruit_SSD1351 oled = Adafruit_SSD1351(CS_PIN, DC_PIN, RST_PIN);  

const           std::vector<std::vector<uint16_t>> colors{reds, blues, greens, pinks, yellows, oranges, purples};  // All colors
const           std::vector<std::vector<uint32_t>> all_colors_neopix {reds_neopix, blues_neopix, greens_neopix, pinks_neopix, yellows_neopix, oranges_neopix, purples_neopix};
bool            inStepContext = false; 
bool            inStepContextPrev = false;
char *          prevInfoText;
int16_t         prevInfoVal;
uint32_t        lastStepContextMillis = millis();          // when did we last show a new info bar
int16_t         currentScreenIDX = 0;
Screen*         currentScreen;                         // Pointer to the current screen
/*
*************************** Helper variables **************************

              Cache off some values that we will use a lot

************************************************************************
*/
#pragma region
const uint8_t screen_middle     = SCREEN_HEIGHT/2; 
const uint8_t screen_qtr_2_x    = SCREEN_HEIGHT/4;
const uint8_t screen_qtr_3_x    = screen_qtr_2_x * 2;
const uint8_t screen_qtr_4_x    = screen_qtr_2_x * 3;
const uint8_t screen_bot_info_y = 95;                 // Below here we can show pot values, etc.
const uint8_t infoBarHeight     = 17;
const uint8_t infobarY          = infoBarHeight - 1;
const uint8_t infobarX          = 3;
const uint8_t infoBarLineWidth  = SCREEN_WIDTH - infobarX * 2;

// Menu Stuff
const uint8_t mnu_title_left_pad = 5;
const uint8_t mnu_title_colorblock_width = 15;

// Pot Stuff
const uint8_t pot_x_st          = 5;  // First pot position
const uint8_t pot_line_width    = (SCREEN_WIDTH / 6);
const uint8_t pot_dot_radius    = 3;
const uint8_t pot_line_y        = screen_bot_info_y + (SCREEN_HEIGHT - screen_bot_info_y)/2;
const uint8_t pot_val_y         = pot_line_y + pot_dot_radius + 10;
const uint8_t pot_label_y       = 98;
const uint8_t pot_label_ht      = 8;
  
// Slider stuff
const uint16_t sliderWidth           = 13;    // in pixels - How wide the slider is
const uint16_t sliderHeight          = 60;    // in pixels - How tall the slider is
const uint8_t  sliderSpacing         = 15;
const uint8_t  sliderThinWidth       = 3;
const uint8_t  sliderThinHeight      = 30;
const uint8_t  slider_val_label_y    = SCREEN_HEIGHT - sliderThinHeight - 10;
const uint8_t  slider_label_x1       = 30;
const uint8_t  slider_label_x2       = SCREEN_WIDTH/2;
const uint8_t  slider_labelbar_width = (SCREEN_WIDTH - (2*slider_label_x1))/2;

// DJT - not used yet..
const uint8_t  text_height = 6;
// Title Bar
const uint8_t  title_bar_y_start  = 0;
const uint8_t  title_bar_y_end    = 20;
const uint8_t  title_bar_x_start  = 0;
const uint8_t  title_bar_x_end    = SCREEN_WIDTH;
const uint8_t  title_bar_width    = title_bar_x_end - title_bar_x_start;
const uint8_t  title_bar_height   = title_bar_y_end - title_bar_y_start;
// Action Zone 
const uint8_t  action_zone_y_start = title_bar_y_end;
const uint8_t  action_zone_y_end   = 90;
const uint8_t  action_zone_x_start = 0;
const uint8_t  action_zone_x_end   = SCREEN_WIDTH;
const uint8_t  action_zone_width   = action_zone_x_end - action_zone_x_start;
const uint8_t  action_zone_height  = action_zone_y_end - action_zone_y_start;
// Info one
const uint8_t  info_zone_y_start = action_zone_y_end + 1;
const uint8_t  info_zone_y_end   = 128;
const uint8_t  info_zone_x_start = 0;
const uint8_t  info_zone_x_end   = SCREEN_WIDTH;
const uint8_t  info_zone_width   = info_zone_x_end - info_zone_x_start;
const uint8_t  info_zone_height  = info_zone_y_end - info_zone_y_start;
// sliders
const uint8_t  slider_label_zone_height        = 8;
const uint8_t  slider_label_zone_y_start       = info_zone_y_start;
const uint8_t  slider_label_zone_y_end         = slider_label_zone_y_start + slider_label_zone_height;
const uint8_t  slider_label_zone_text_y        = slider_label_zone_y_end - 5;
const uint8_t  slider_label_zone_text_x_left   = 0;
const uint8_t  slider_label_zone_text_x_right  = SCREEN_WIDTH /2;
const uint8_t  slider_val_width                = 4;
const uint8_t  slider_val_y_start              = slider_label_zone_y_end + 5;
const uint8_t  slider_val_y_end                = SCREEN_HEIGHT;
const uint8_t  slider_val_x_left               = 0;
const uint8_t  slider_val_x_right              = SCREEN_WIDTH - slider_val_width;
// Pots
const uint8_t  info_pot_x_start    = 6;
const uint8_t  info_pot_y_start    = slider_label_zone_y_end + 1; 
const uint8_t  info_pot_dot_radius = 3;
const uint8_t  info_pot_width      = (SCREEN_WIDTH - (2 * info_pot_x_start))/4;
const uint8_t  info_pot_line_y     = info_pot_y_start + (2 * info_pot_dot_radius) + 5;
const uint8_t  info_pot_label_y    = SCREEN_HEIGHT - 5;
const uint8_t  info_pot_line_width = info_pot_width - info_pot_dot_radius * 2;
// pot 1
const uint8_t  info_pot1_x_start   = info_pot_x_start;
const uint8_t  info_pot1_x_end     = info_pot1_x_start + info_pot_width - 1;
const uint8_t  info_pot1_line_x_start   = info_pot1_x_start + info_pot_dot_radius;
// pot 2
const uint8_t  info_pot2_x_start   = info_pot1_x_end;
const uint8_t  info_pot2_x_end     = info_pot2_x_start + info_pot_width - 1;
const uint8_t  info_pot2_line_x_start   = info_pot2_x_start + info_pot_dot_radius;
// pot 3
const uint8_t  info_pot3_x_start = info_pot2_x_end;
const uint8_t  info_pot3_x_end   = info_pot3_x_start + info_pot_width - 1;
const uint8_t  info_pot3_line_x_start   = info_pot3_x_start + info_pot_dot_radius;
// pot 4
const uint8_t  info_pot4_x_start = info_pot3_x_end;
const uint8_t  info_pot4_x_end   = info_pot4_x_start + info_pot_width - 1;
const uint8_t  info_pot4_line_x_start   = info_pot4_x_start + info_pot_dot_radius;
// Sequencer display related
const uint8_t seq_disp_y  = 60;
const uint8_t stepWidth = (SCREEN_WIDTH / 16) - 3;
const uint8_t stepDispSpacing = 3;

#pragma endregion
std::vector<Screen> screens;

/*
*************************** Initialization **************************
*********************************************************************
*/

/*                   INITIALIZE SCREENS HERE (in init screens)

        Step 1: Add to the screens vector by pushing back new Screen objects
        Step 2: Initialize the input banks
                - Global = what to call the inputs when no seq button is being pressed
                - Step   = what to call the inputs when the sequencer button is pressed
        Step 3: Initialize the input callback functions
                - Same deal as above... different ones for global vs step based since
                  you likely want to do somehting different for each.
                - You can use VOIDCALLBACK as a placeholder if you don't want to assign a func to an input.

*/
void initOled(void)
{
    oled.begin(16000000);
    oled.fillScreen(BLACK);
    oled.setRotation(2);
    drawCurrentTitleBar();
}

void initScreens(void)
{
    // Screen # 1
    screens.push_back(Screen("Step Edit", ORANGE_5));

    screens[0].addInputFunctionBankGlobal (setHeadphoneVolume, cb_seq_set_bpm, VOIDCALLBACK, VOIDCALLBACK, VOIDCALLBACK);
    screens[0].addInputLabelBankGlobal    ("vol", "bpm");

    // screens[0].addInputFunctionBankStep   (cb_set_volume, cb_set_drumLen, cb_set_swing, cb_set_drumMix2, cb_set_drumFreq, cb_set_drumPMod);
    // screens[0].addInputLabelBankStep      ("vol", "len","swng","2x","freq","p-mod");

    screens[0].addInputFunctionBankStep   (cb_set_volume, cb_set_attack, cb_set_decay, cb_set_swing, cb_set_ratchet, VOIDCALLBACKSTEP);
    screens[0].addInputLabelBankStep      ("vol", "atk","rel","swng","rtcht"," - ");

    screens[0].changeBankGlobal           (-1);   // Acts kind of like an initializer.. current bank members not set before this
    screens[0].changeBankStep             (-1);

    // Screen #2
    screens.push_back(Screen("Screen 2", YELLOW_5));
    // add input inits later
    //
    //
    // Screen #3
    screens.push_back(Screen("Screen 3", PURPLE_5));
    // add input inits later 
    //
    //
    currentScreen = &screens[currentScreenIDX];
}

void drawCurrentTitleBar(void)
{
    screens[currentScreenIDX].drawTitleBarMnu();
}

void drawInfoBar(const char * text, int16_t displayVal)
{
    lastStepContextMillis = millis();   // Reset timer... still hodling the button or whatever.

    if (prevInfoText == text && prevInfoVal == displayVal && inStepContext) return;  

    eraseInfoBar();
    oled.setTextColor(WHITE);
    oled.setCursor(6, (infoBarHeight/2 - 1));
    oled.print(text);
    oled.print(displayVal);
    prevInfoText = text;
    FUNderline(infobarX, infobarY, infoBarLineWidth, YELLOW_5);
    prevInfoVal = displayVal;
    inStepContext = true;
}

void drawTitleBar(const char * text, uint16_t color)
{
    uint8_t x = mnu_title_colorblock_width + mnu_title_left_pad;
    eraseInfoBar();
    oled.drawRoundRect(2,2,SCREEN_WIDTH-4, infoBarHeight, 3, color);
    oled.fillRoundRect(2,2, mnu_title_colorblock_width, infoBarHeight, 3, color);
    oled.setTextColor(WHITE);
    oled.setCursor(x, infoBarHeight/2);
    oled.print(text);
}

void eraseInfoBar(void)
{
    oled.fillRect(0,0, SCREEN_WIDTH, infoBarHeight + 3, BLACK);
}

void checkDisplayContext(void)
{
    if (inStepContext && (millis() - lastStepContextMillis > INFOBAR_DISP_TIME))
    {
        lastStepContextMillis = millis();
        drawCurrentTitleBar();
        update_input_display_ALL_global();
        inStepContext = false;
    }

    if (inStepContext && !inStepContextPrev)  // Switched from global to step.. do stuff
    {
        // DJT UPDATE ME - call func to update all inpt labels.
        update_input_display_ALL_step();
    }

    inStepContextPrev = inStepContext;
}

void refreshInputDisp(void)
{
    (inStepContext) ? update_input_display_ALL_step() : update_input_display_ALL_global();
}


// DJT - update this to deal with displaying the number value temporarily.. maybe just in a common zone of the screen? But not important.
void update_input_disp_dotline(uint8_t idx)
{
    uint8_t  val = getInputValueByIDX(idx);
    uint8_t  x = 0;
    uint8_t  y =  info_pot_line_y;
    uint8_t  dot_x;
    uint16_t color = yellows[idx];

    switch (idx){
    case 0:
        x = info_pot1_line_x_start;
        break;
    case 1:
         x = info_pot2_line_x_start;
        break;
    case 2:
        x = info_pot3_line_x_start;
        break;
    case 3:
        x = info_pot4_line_x_start;
        break;
    default:
        return;
    }
    dot_x = x + map(val, 0, 100, 0, pot_line_width);
    oled.fillRect(x - pot_dot_radius - 1, y - pot_dot_radius - 1, info_pot_width, pot_dot_radius * 2 + 2, BLACK);
    oled.fillCircle(dot_x, info_pot_line_y, pot_dot_radius, color);
    oled.drawFastHLine(x, y, info_pot_line_width, color);
}

void update_slider_display_val_IDX(uint8_t idx)
{
    uint8_t val = getInputValueByIDX(idx);
    uint8_t  x = 0;
    uint8_t  y = map(val, 0, 100, SCREEN_HEIGHT, slider_val_y_start);

    uint16_t color;
    if (idx == 1) {
        x = 0;
        color = GREEN_4;
    }
    if (idx == 2) {
        x = slider_val_x_right;
        color = BLUE_4;
    }
    oled.fillRect(x, slider_val_y_start, slider_val_width, SCREEN_HEIGHT, BLACK);
    oled.fillRect(x, y, slider_val_width, SCREEN_HEIGHT - y, color);
}

void update_input_label_IDX_step(uint8_t idx, bool showVal)
{
    uint8_t x = 0;
    uint8_t y =  info_pot_label_y - text_height;
    uint16_t color = yellows[idx];
    const char* label;

    if (showVal){
        char num_char[10];
        itoa(getInputValueByIDX(idx), num_char, 10);
        label = num_char;
    } else {
        label = currentScreen -> inputLabelBankStep_current[idx];
    }

    switch (idx)
    {
    case 0:
        x = info_pot1_x_start;
        break;
    case 1:
         x = info_pot2_x_start;
        break;
    case 2:
        x = info_pot3_x_start;
        break;
    case 3:
        x = info_pot4_x_start;
        break;

    default:
        return;
        break;
    }
    oled.fillRect(x, y, info_pot_width, text_height + 2, BLACK);
    oled.setCursor(x+2, y);
    oled.setTextColor(color);
    oled.println(label);
    //oled.drawRect(x, y, info_pot_width, text_height, BLUE_2);
}
void update_input_label_IDX_global(uint8_t idx, bool showVal)
{
    uint8_t x = 0;
    uint8_t y =  info_pot_label_y - text_height;
    uint16_t color = yellows[idx];
    const char* label;

    if (showVal){
        char num_char[10];
        itoa(getInputValueByIDX(idx), num_char, 10);
        label = num_char;
    } else {
        label = currentScreen->inputLabelBankGlobal_current[idx];
    }

    switch (idx)
    {
    case 0:
        x = info_pot1_x_start;
        break;
    case 1:
         x = info_pot2_x_start;
        break;
    case 2:
        x = info_pot3_x_start;
        break;
    case 3:
        x = info_pot4_x_start;
        break;

    default:
        return;
        break;
    }
    oled.fillRect(x, y, info_pot_width, text_height + 2, BLACK);
    oled.setCursor(x+2, y);
    oled.setTextColor(color);
    oled.println(label);
    //oled.drawRect(x, y, info_pot_width, text_height, BLUE_2);
}

void update_slider_label_IDX(uint8_t idx, char* label)
{
    uint8_t x;
    uint8_t y =  slider_label_zone_text_y;
    uint16_t color;
    if (idx == 1) {
        x = slider_label_zone_text_x_left;
        color = GREEN_5;
    }
    if (idx == 2) {
        x = slider_label_zone_text_x_right;
        uint16_t boundx;
        uint16_t boundy;
        uint16_t boundw;
        uint16_t boundh;
        oled.getTextBounds(label, x, y, &boundx, &boundy, &boundw, &boundh); 
        x = SCREEN_WIDTH - boundw;
        color = BLUE_5;
    }
    oled.setCursor(x,y);
    oled.setTextColor(color);
    oled.print(label);
}

void update_input_display_ALL_step(void)
{
    for (uint8_t i = 0; i < 6; i++)
    {
        update_input_disp_dotline(i);
        update_input_label_IDX_step(i);
    }
}

void update_input_display_ALL_global(void)
{
    for (uint8_t i = 0; i < 6; i++)
    {
        update_input_disp_dotline(i);
        update_input_label_IDX_global(i);
    }

    clear_step_arrows_ALL();
}

void nextScreen(void)
{
    currentScreenIDX++;
    if (currentScreenIDX > (screens.size() - 1)) currentScreenIDX = 0;
    currentScreen = &screens[currentScreenIDX];
}

void prevScreen(void) 
{
    currentScreenIDX--;
    if (currentScreenIDX < 0) currentScreenIDX = screens.size() - 1;
    currentScreen = &screens[currentScreenIDX];
}
/*
***************************   SCREEN   **********************************

                     Each represents a "screen". 

************************************************************************
*/

Screen::Screen(const char* Title, uint16_t TitleColor)
{
    title = Title;
    titleColor = TitleColor;
    // inputLabelBanksGlobal = {};
    // std::vector<std::vector<char*>>    inputLabelBanksStep;      
    // std::vector<std::vector<callback>> inputCallbackBanksGlobal;
    // std::vector<std::vector<callback>> inputCallbackBanksStep;
}

// functions
void Screen::drawTitleBarMnu(void)                    // Draws the top title bar
{
    drawTitleBar(title, titleColor);
}

void Screen::changeBankGlobal(int8_t dir = 1)
{
    (dir > 0) ? inputBankGlobal_IDX++ : inputBankGlobal_IDX--;
    if (inputBankGlobal_IDX > inputLabelBanksGlobal.size()) inputBankGlobal_IDX = inputLabelBanksGlobal.size() - 1;
    if (inputBankGlobal_IDX < 0) inputBankGlobal_IDX = 0;

    // Now set the current banks
    inputLabelBankGlobal_current = inputLabelBanksGlobal[inputBankGlobal_IDX];
    inputFunctionBankGlobal_current = inputFuncionBanksGlobal[inputBankGlobal_IDX];

    //switch (inputBankGlobal_IDX){
    // case 0:
    //     inputBankGlobal_current = inputBankGlobal_1;
    //     break;
    // case 1:
    //     inputBankGlobal_current = inputBankGlobal_2;
    //     break;
    // case 2:
    //     inputBankGlobal_current = inputBankGlobal_3;
    //     break;
    // default:
    //     inputBankGlobal_current = inputBankGlobal_1;
    //     break;
    }

void Screen::changeBankStep(int8_t dir)
{
    (dir > 0) ? inputBankStep_IDX++ : inputBankStep_IDX--;
    if (inputBankStep_IDX > inputLabelBanksStep.size()) inputBankStep_IDX = inputLabelBanksStep.size() - 1;
    if (inputBankStep_IDX < 0) inputBankStep_IDX = 0;

    // Now set the current banks
    inputLabelBankStep_current = inputLabelBanksStep[inputBankStep_IDX];
    inputFunctionBankStep_current = inputFunctionBanksStep[inputBankStep_IDX];
}

void  Screen::addInputLabelBankGlobal(const char* knob1, const char*knob2, const char*knob3, 
                                      const char* knob4, const char* slider1, const char* slider2)
{
    inputLabelBanksGlobal.push_back({knob1, knob2, knob3, knob4, slider1, slider2});

}

void  Screen::addInputLabelBankStep(const char* knob1, const char* knob2, const char*knob3, 
                                    const char* knob4, const char* slider1, const char* slider2)
{
    inputLabelBanksStep.push_back({knob1, knob2, knob3, knob4, slider1, slider2});
}

void Screen::addInputFunctionBankGlobal(callback knob1_func, callback knob2_func, 
                                        callback knob3_func, callback knob4_func, 
                                        callback slider1_func, callback slider2_func)
{
    inputFuncionBanksGlobal.push_back({knob1_func, knob2_func, knob3_func, knob4_func, 
                                        slider1_func, slider2_func});
}

void Screen::addInputFunctionBankStep(callbackStep knob1_func, callbackStep knob2_func, 
                                     callbackStep knob3_func, callbackStep knob4_func, 
                                     callbackStep slider1_func, callbackStep slider2_func)
{
    inputFunctionBanksStep.push_back({knob1_func, knob2_func, knob3_func, knob4_func, 
                                 slider1_func, slider2_func});
}

void Screen::runInputFunctionGlobal(uint8_t input_idx)  // Given an input index (1 - 6), and the current screen / bank, run a function.
{
    int16_t val = getInputValueByIDX(input_idx);
    inputFunctionBankGlobal_current[input_idx](val);
}

void Screen::runInputFunctionStep(Sequencer& seq, uint8_t input_idx, uint8_t stepNumber)
{
    uint8_t val = getInputValueByIDX(input_idx);
    Serial.println("Still working...");
    Serial.println(val);
    Serial.println(input_idx);
    Serial.println(stepNumber);
    inputFunctionBankStep_current[input_idx](seq, stepNumber, val);
}

void run_input_change_function_global(uint8_t input_idx)
{
    currentScreen->runInputFunctionGlobal(input_idx);
}

void run_input_change_function_step(Sequencer& seq, uint8_t input_idx, uint8_t stepNumber)
{
    currentScreen->runInputFunctionStep(seq, input_idx, stepNumber);
}

/*
***************************  ~~ Displaying Sequencer ~~  **************************


************************************************************************************
*/

void draw_cur_seq_oled(void)
{
    uint8_t x = stepDispSpacing;
    uint16_t color = colors[Sequencer::getCurrentSequencer()->colorSetIDX][4];
    for (uint8_t i = 0; i < 16; i++)
    {
        if (Sequencer::getCurrentSequencer()->getStepStateAtIndex(i))
        {
            oled.fillRect(x, seq_disp_y, stepWidth, stepWidth, color);
        }

        oled.drawFastHLine(x, seq_disp_y + stepWidth + 1, stepWidth, color);   // Underlines
        x += stepDispSpacing + stepWidth;
    }
}

// Draw the current step, erase the last'n
void advance_cur_seq_step_oled(void)
{
    uint8_t x = get_step_disp_x_pos(Sequencer::getCurrentSequencer() -> getCurrentStepIDX());
    uint8_t prevx = get_step_disp_x_pos(Sequencer::getCurrentSequencer() -> getPrevStepIDX());
    uint16_t color = colors[Sequencer::getCurrentSequencer()->colorSetIDX][4];
    oled.drawFastHLine(x, seq_disp_y + stepWidth + 2, stepWidth, color);
    oled.drawFastHLine(x, seq_disp_y + stepWidth + 3, stepWidth, color);
    oled.drawFastHLine(prevx, seq_disp_y + stepWidth + 2, stepWidth, BLACK);
    oled.drawFastHLine(prevx, seq_disp_y + stepWidth + 3, stepWidth, BLACK);
}

// Draws a step on the oled (filled in square) or erases.
void draw_erase_step_at_IDX_oled(uint8_t idx, bool drawErase)
{
    uint16_t color;
    uint8_t x = get_step_disp_x_pos(idx);

    (drawErase) ? color = colors[Sequencer::getCurrentSequencer()->colorSetIDX][4] : color = BLACK;
    oled.fillRect(get_step_disp_x_pos(idx), seq_disp_y, stepWidth, stepWidth, color);
}

// Deal with the fact that we can only display 16 steps on the screen.
// Returns: X position of the left side of the step IDX passed in
uint8_t get_step_disp_x_pos(uint8_t step_IDX)
{
    if (step_IDX  > 15) get_step_disp_x_pos(step_IDX - 16);    // Want a number between 0 - 15.. recursively get there

    //uint8_t x = stepDispSpacing + (stepWidth + stepDispSpacing) * step_IDX;
    return stepDispSpacing + (stepWidth + stepDispSpacing) * step_IDX;
}

// Given the current step position, calculate the previous. Useful for clearing prev step display.
// uint8_t get_prev_step_disp_x_pos(uint8_t step_IDX)
// {
//     if (step_IDX  > 15) get_step_disp_x_pos(step_IDX - 16);    // Want a number between 0 - 15.. recursively get there

//     uint8_t prev_x;
//     if (step_IDX == 0){  // On the first step displayed.
//         prev_x = stepDispSpacing + ((stepWidth + stepDispSpacing) * 15);
//     } else {
//         prev_x = x - stepWidth - stepDispSpacing;
//     } 
//     return prev_x;
// }

// Draw a straight arrow.
void draw_arrow(uint8_t tip_x, uint8_t tip_y, uint8_t len, uint16_t color, uint8_t dir)
{
    const uint8_t arrow_width = 4;

    switch (dir)
    {
    case 0:     // Up
        oled.fillTriangle(tip_x, tip_y, tip_x - arrow_width, tip_y + arrow_width, tip_x + arrow_width, tip_y + arrow_width, color);
        oled.drawFastVLine(tip_x, tip_y, len, color);
        break;

    case 1:     // Right
        oled.fillTriangle(tip_x, tip_y, tip_x - arrow_width, tip_y - arrow_width, tip_x - arrow_width, tip_y + arrow_width, color);
        oled.drawFastHLine(tip_x - len, tip_y, len, color);
        break;

    case 2:     // Down 
        oled.fillTriangle(tip_x, tip_y, tip_x - arrow_width, tip_y - arrow_width, tip_x + arrow_width, tip_y - arrow_width, color);
        oled.drawFastVLine(tip_x, tip_y - len, len, color);
        break;

    case 3:     // Left
        oled.fillTriangle(tip_x, tip_y, tip_x + arrow_width, tip_y - arrow_width, tip_x + arrow_width, tip_y + arrow_width, color);
        oled.drawFastHLine(tip_x, tip_y, len, color);
        break;
    default:
        break;
    }
}
// Draw the current step, erase the last'n
void draw_step_held_arrow_at_IDX(uint8_t idx, uint16_t color)
{
    uint8_t x = stepDispSpacing + (stepWidth + stepDispSpacing) * idx + (stepWidth/2);
    uint16_t color_ = colors[Sequencer::getCurrentSequencer()->colorSetIDX][4];
    uint8_t prevx;
    if (idx == 0){  // On the first step displayed.
        prevx = stepDispSpacing + ((stepWidth + stepDispSpacing) * 15);
    } else {
        prevx = x - stepWidth - stepDispSpacing;
    }      
    draw_arrow(prevx, seq_disp_y-10, 10, BLACK, 2);
    draw_arrow(x, seq_disp_y - 10, 10, color, 2);
}

void clear_step_arrows_ALL(void)
{
    for (uint8_t i = 0; i < 16; i++)
    {
        draw_step_held_arrow_at_IDX(i, BLACK);
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

void drawNoteSymbol(uint16_t x, uint16_t y, uint8_t size, int color)
{
    uint16_t radius = size * 2;
    uint8_t lineX_start = x + radius;
    uint8_t line_height = 10 * size;

    oled.fillCircle(x, y, radius, color);
    oled.drawFastVLine(lineX_start, y - line_height, line_height, color);
}

void VOIDCALLBACK(int idx)
{
    Serial.println("Void Callback called. No action taken.");
}

void VOIDCALLBACKSTEP(Sequencer& seq, uint8_t idx, uint8_t step)
{
    Serial.println("Void Callback STEP called. No action taken.");
}

// DJT - DO DIS DO DIXSSSSS
void convertToDisplayval(char* outStr[6], uint16_t val)
{
    //
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

void  z_funNoteAnimation(uint8_t xtraX, uint8_t xtraY, uint8_t size)
{
    uint8_t currentX = 10 + xtraX;
    uint8_t currentY = 20 + xtraY;
    uint8_t coloridx = 0;
    uint8_t colorAryIDX = 0;   // trakc the color bank

    for (uint8_t i = 0; i < 300; i ++)
    {
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

void z_drawScreenZones(void)
{
    // Title bar zone
    uint16_t color = PURPLE_5;
    oled.drawRect(title_bar_x_start, title_bar_y_start, title_bar_width
    , title_bar_height, color);

    // Action zone
    oled.drawRect(action_zone_x_start, action_zone_y_start, action_zone_width
    , action_zone_height, color);

    // Info zone
    oled.drawRect(info_zone_x_start, info_zone_y_start, info_zone_width
    , info_zone_height, color);

    color = PURPLE_4;
    // Slider label zone
    oled.drawRect(0, slider_label_zone_y_start, SCREEN_WIDTH, slider_label_zone_height, color);
    delay(200);

    color = PURPLE_3;
    // Pot 1
    oled.drawRect(info_pot1_x_start, info_pot_y_start, info_pot_width
    , info_zone_height, color);
        delay(200);

    // Pot 2
    oled.drawRect(info_pot2_x_start, info_pot_y_start, info_pot_width
    , info_zone_height, color);
        delay(200);

    // Pot 3
    oled.drawRect(info_pot3_x_start, info_pot_y_start, info_pot_width
    , info_zone_height, color);
        delay(200);

    // Pot 4
    oled.drawRect(info_pot4_x_start, info_pot_y_start, info_pot_width
    , info_zone_height, color);
        delay(200);
    
    color = YELLOW_5;

    // Line representing label area of pot vals.
    oled.drawFastHLine(0, info_pot_label_y, 128, color);

    // Line representing label area of sliders
    oled.drawFastHLine(0, slider_label_zone_text_y, 128, color);

    color = YELLOW_3;
    // Line representing pot value line (with tha dot)
    oled.drawFastHLine(0, pot_line_y, 128, color);
}
//void callback_set_attack(int attack)
void z_callback_tester1(int idx)
{
    Serial.println("callback worked??");
    Serial.println(idx);
}

void z_callback_tester2(int idx)
{
    Serial.println("callback 2 worked??");
        Serial.println(idx);
}

void z_callback_tester_step(Sequencer& seq, uint8_t step_number, uint8_t val)
{
    Serial.println("Step callback worked??????");
    Serial.println(step_number);
}
