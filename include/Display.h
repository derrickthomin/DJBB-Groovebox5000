#pragma once
#include <vector>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include "Sequencer.h"
#include "Colors.h"
#include "bitmaps.h"
#include <functional>
#include "settings.h"

typedef std::function<void(uint8_t)> callback;
typedef std::function<void(Sequencer&, uint8_t, uint8_t)> callbackStep;
void VOIDCALLBACK(int idx=0);                                             // Use these as defaults for callback function vectors
void VOIDCALLBACKSTEP(Sequencer& seq, uint8_t idx, uint8_t step);
const           std::vector<std::vector<uint16_t>>;
/*
**************************** Classes **********************************
************************************************************************
*/
class Screen
{
    public:
        Screen(const char* Title, uint16_t TitleColor);
        const char*    title;
        uint16_t titleColor;

        std::vector<std::vector<const char*>>  inputLabelBanksGlobal;
        std::vector<std::vector<const char*>>  inputLabelBanksStep;
        std::vector<std::vector<callback>>     inputFuncionBanksGlobal;
        std::vector<std::vector<callbackStep>> inputFunctionBanksStep;

        std::vector<const char*>               inputLabelBankGlobal_current;
        std::vector<const char*>               inputLabelBankStep_current;
        std::vector<callback>                  inputFunctionBankGlobal_current;
        std::vector<callbackStep>              inputFunctionBankStep_current;

        int8_t inputBankGlobal_IDX =           0;
        int8_t inputBankStep_IDX =             0;
        void   drawTitleBarMnu(void);
        void   addInputLabelBankGlobal (const char* knob1 = " - ", const char* knob2 = " - ", const char*knob3 = " - ", 
                                        const char* knob4 = " - ", const char* slider1 = " - ", const char* slider2 = " - ");
        void   addInputLabelBankStep   (const char* knob1 = " - ", const char* knob2 = " - ", const char* knob3 = " - ", 
                                        const char* knob4 = " - ", const char* slider1 = " - ", const char* slider2 = " - ");

        void   addInputFunctionBankGlobal (callback knob1_func = VOIDCALLBACK, callback knob2_func = VOIDCALLBACK, 
                                           callback knob3_func = VOIDCALLBACK, callback knob4_func = VOIDCALLBACK, 
                                           callback slider1_func = VOIDCALLBACK, callback slider2_func = VOIDCALLBACK);

        void   addInputFunctionBankStep   (callbackStep knob1_func = VOIDCALLBACKSTEP, callbackStep knob2_func = VOIDCALLBACKSTEP, 
                                           callbackStep knob3_func = VOIDCALLBACKSTEP, callbackStep knob4_func = VOIDCALLBACKSTEP, 
                                           callbackStep slider1_func = VOIDCALLBACKSTEP, callbackStep slider2_func = VOIDCALLBACKSTEP);

        void   changeBankGlobal(int8_t dir = 1);   // Pos = forward, Neg = Backward 
        void   changeBankStep  (int8_t dir = 1);   // Pos = forward, Neg = Backward 

        void   runInputFunctionGlobal(uint8_t input_idx);
        void   runInputFunctionStep(Sequencer& seq, uint8_t input_idx, uint8_t stepNumber);
};

/*
*************************** Functions **********************************
************************************************************************
*/
void initOled           (void);    
void initScreens        (void);                                                          // Sets up OLED
void nextScreen         (void);       // sets current screen to next screen, loops arond
void prevScreen         (void);       // sets current screen to prev screen looops around
void drawInfoBar        (const char * text, int16_t displayVal = -1);
void drawTitleBar       (const char * text, uint16_t color);
void drawCurrentTitleBar(uint8_t menu_idx);
void setStepContext     (bool status);
void eraseInfoBar       (void);
void checkDisplayContext       (void);
void draw_sliders       (int8_t valA, int8_t valB);                                      // Draw sliders on the screen
void update_sliders     (int8_t valA, int8_t valB);
void update_sliders_thin(int8_t valA, int8_t valB);
void update_slider_label(uint8_t sliderNumber, char* label);
void clear_slider_label (uint8_t sliderNumber);
void update_pot_val     (uint8_t potNumber, uint8_t potVal);                // Draw pot values on bottom of the screen
void clear_pot_val      (uint8_t potNumber);                                  // Erase pot values on bottom of screen.
void clear_pot_label    (uint8_t potNumber);
void update_pot_label   (uint8_t potNumber, char* label);
void drawNoteSymbol     (uint16_t x, uint16_t y, uint8_t size = 1, int color = RED_2); // Draw note symbol at a certain location.
void drawCurrentTitleBar(void);    

void update_input_label_IDX_step     (uint8_t idx, bool showVal = false); 
void update_input_label_IDX_global   (uint8_t idx, bool showVal = false);             // showVal = show the actual knob val instead of label

void update_input_disp_dotline       (uint8_t idx);  
void update_slider_label_IDX         (uint8_t idx, char* label);
void update_slider_display_val_IDX   (uint8_t idx);

void run_input_change_function_step   (Sequencer& seq, uint8_t input_idx, uint8_t stepNumber);
void run_input_change_function_global (uint8_t input_idx);

void update_input_display_ALL_step(void);
void update_input_display_ALL_global(void);
void refreshInputDisp(void);

/*
***************************  ~~ Displaying Sequencer ~~  **************************


************************************************************************************
*/

void draw_seq_outline(void); 
void draw_cur_seq_step(void);
void draw_step_held_arrow_at_IDX(uint8_t idx, uint16_t color);
void get_disp_seq_step_x_IDX(uint8_t idx);
void clear_step_arrows_ALL(void);

/*
***************** Helper / Testing Functions *******************
*/
void FUNderline     (uint8_t startX, uint8_t startY, uint8_t length, uint16_t color);
void draw_arrow(uint8_t start_x, uint8_t start_y, uint8_t len, uint16_t color, uint8_t dir);                  // 0 = Up, 1 = Right, 2 = Down, 3 = Left
uint32_t get_neopix_color_by_idx(int16_t colorsetIdx, int16_t colorShadeIdx);     // Interface for other modules to get specific colors
void z_showAllColors(void);
void z_displayUpdateTest(int x);
void z_funNoteAnimation(uint8_t xtraX = 0, uint8_t xtraY = 0, uint8_t size = 1);
void z_drawMascotBitmap(void);
void z_drawScreenZones(void);
void z_callback_tester1(int idx);
void z_callback_tester2(int idx);
void z_callback_tester_step(Sequencer& seq, uint8_t step_number, uint8_t val);
