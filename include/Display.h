#pragma once
#include <vector>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include "Sequencer.h"
#include "Colors.h"
#include "bitmaps.h"
#include <functional>

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128 // Change this to 96 for 1.27" OLED.
//typedef void (*callback_function)(void);
typedef std::function<void(uint8_t)> callback;
typedef std::function<void(int, Sequencer&)> callbackwseq;
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

        // Input banks.. this is how we figure out how to label the pots, etc?
        // Likely will not use all 3 for all screens, but might as well make them all.
        std::vector<char*> inputBankGlobal_1;
        std::vector<char*> inputBankGlobal_2;
        std::vector<char*> inputBankGlobal_3;
        std::vector<char*> inputBankStep_1;
        std::vector<char*> inputBankStep_2;
        std::vector<char*> inputBankStep_3;

        std::vector<char*> inputBankGlobal_current;
        std::vector<char*> inputBankStep_current;

        std::vector<callback> inputFunctionBankGlobal_1;
        std::vector<callback> inputFunctionBankGlobal_2;
        std::vector<callback> inputFunctionBankGlobal_3;
        std::vector<callback> inputFunctionBankStep_1;
        std::vector<callback> inputFunctionBankStep_2;
        std::vector<callback> inputFunctionBankStep_3;

        int8_t inputBankGlobal_IDX = 0;
        int8_t inputBankStep_IDX = 0;
        void   drawTitleBarMnu(void);
        void   initInputBankGlobal(uint8_t bank, char* knob1 = "-", char* knob2 = "-", char* knob3 = "-", 
                                    char* knob4 = "-", char* slider1 = "-", char* slider2 = "-");
        void   initInputBankStep(uint8_t bank, char* knob1 = "-", char* knob2 = "-", char* knob3 = "-", 
                                    char* knob4 = "-", char* slider1 = "-", char* slider2 = "-");

        void   initInputFunctionsGlobal(uint8_t bank, callback knob1_func, callback knob2_func, 
                                                      callback knob3_func, callback knob4_func, 
                                                      callback slider1_func, callback slider2_func);

        void   initInputFunctionsStep(uint8_t bank,   callback knob1_func, callback knob2_func, 
                                                      callback knob3_func, callback knob4_func, 
                                                      callback slider1_func, callback slider2_func);

        void setNextInputBankGlobal(void);   // sets the current input bank vector. Doesnt loop around.
        void setNextInputBankStep(void);
        void setPrevInputBankGlobal(void);
        void setPrevInputBankStep(void);

        void runInputFunctionGlobal(uint8_t input_idx);
        void runInputFunctionStep(uint8_t input_idx, int step_number);
};

/*
*************************** Functions **********************************
************************************************************************
*/
void initOled(void);    
void initScreens(void);                                                          // Sets up OLED
void nextScreen(void);       // sets current screen to next screen, loops arond
void prevScreen(void);       // sets current screen to prev screen looops around
void drawInfoBar(char * text, int16_t displayVal = -1);
void drawTitleBar(char * text, uint16_t color);
void drawCurrentTitleBar(uint8_t menu_idx);
void infoBarSetStatus(bool status);
void eraseInfoBar(void);
void checkInfoBar(void);
void draw_sliders(int8_t valA, int8_t valB);                                      // Draw sliders on the screen
void update_sliders(int8_t valA, int8_t valB);
void update_sliders_thin(int8_t valA, int8_t valB);
void update_slider_label(uint8_t sliderNumber, char* label);
void clear_slider_label(uint8_t sliderNumber);
void update_pot_val(uint8_t potNumber, uint8_t potVal);                // Draw pot values on bottom of the screen
void clear_pot_val(uint8_t potNumber);                                  // Erase pot values on bottom of screen.
void clear_pot_label(uint8_t potNumber);
void update_pot_label(uint8_t potNumber, char* label);
void drawNoteSymbol(uint16_t x, uint16_t y, uint8_t size = 1, int color = RED_2); // Draw note symbol at a certain location.
void drawCurrentTitleBar(void);                                
void update_pot_label_IDX(uint8_t idx, char* label);           
void update_pot_display_val_IDX(uint8_t idx);  
void update_slider_label_IDX(uint8_t idx, char* label);
void update_slider_display_val_IDX(uint8_t idx);

// Simple helper functions. They display text based on current screen / input label vector
void update_pot_1_label_global(void);
void update_pot_2_label_global(void);
void update_pot_3_label_global(void);
void update_pot_4_label_global(void);
void update_slider_1_label_global(void);
void update_slider_2_label_global(void);
void update_pot_1_label_step(void);
void update_pot_2_label_step(void);
void update_pot_3_label_step(void);
void update_pot_4_label_step(void);
void update_slider_1_label_step(void);
void update_slider_2_label_step(void);

void update_pot_1_val_global(void);
void update_pot_2_val_global(void);
void update_pot_3_val_global(void);
void update_pot_4_val_global(void);
void update_slider_1_val_global(void);
void update_slider_2_val_global(void);
void update_pot_1_val_step(void);
void update_pot_2_val_step(void);
void update_pot_3_val_step(void);
void update_pot_4_val_step(void);
void update_slider_1_val_step(void);
void update_slider_2_val_step(void);

void run_input_change_function_step(uint8_t input_idx, uint8_t step_number);
void run_input_change_function_global(uint8_t input_idx);

/*
***************** Helper / Testing Functions *******************
*/
void FUNderline(uint8_t startX, uint8_t startY, uint8_t length, uint16_t color);
uint32_t get_neopix_color_by_idx(int16_t colorsetIdx, int16_t colorShadeIdx);     // Interface for other modules to get specific colors
void VOIDCALLBACK(int idx=0);
void z_showAllColors(void);
void z_displayUpdateTest(int x);
void z_funNoteAnimation(uint8_t xtraX = 0, uint8_t xtraY = 0, uint8_t size = 1);
void z_drawMascotBitmap(void);
void z_drawScreenZones(void);
void z_callback_tester1(int idx);
void z_callback_tester2(int idx);
