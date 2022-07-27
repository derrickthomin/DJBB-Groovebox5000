// Houses logic to get inputs, and update any objects that need the updates
#include "Inputs.h"
#include "Sequencer.h"
#include "Display.h"
#include "Audiostuff.h"

#define NUM_STEP_BUTTONS     16
#define NUM_TWISTPOTS        4
#define NUM_SLIDEPOTS        2

#define MUX_CONTROL_S0       2
#define MUX_CONTROL_S1       3
#define MUX_CONTROL_S2       4
#define MUX_1_DATA_PIN       5               // DIGITAL - 1st 8 sequence buttons
#define MUX_2_DATA_PIN       6               // DIGITAL - 2nd 8 sequence buttons
#define MUX_3_DATA_PIN       16              // ANALOG  - Handles twist pots + joystick + joystick click
#define SLIDEPOT_LEFT_PIN    25              // Left of the 2 slide pots
#define SLIDEPOT_RIGHT_PIN   24
#define PLAYBUTTON_PIN       31             

// ********** SET TO TRUE TO PRINT TEST STUFF **************

const bool testing = true;               // Set to false to get rid of serial print stuff

// ********** SET TO TRUE TO PRINT TEST STUFF **************

const uint8_t  selectPins[3]         = {MUX_CONTROL_S0, MUX_CONTROL_S1, MUX_CONTROL_S2};
uint8_t        slidePotPins[2]       = {SLIDEPOT_LEFT_PIN, SLIDEPOT_RIGHT_PIN};
const uint8_t  analogChangeThres     = 1;       // Dont count as changed unless it changed by this much. (note this is applied after mapping to 0 - 100)
const uint16_t debounceMillis        = 10;      // If button is on for this long, it's on forreal
const uint8_t  analogBigChangeThresh = 30;      // Big swing in analog value (out of 100)

// Sequencer Buttons
bool seqButtonNow[16]                = {0};     // Literally what is it this instant
bool seqButtonState[16]              = {0};     // What do we calculate the state to be
bool seqButtonStatePrev[16]          = {0};     // What was the last calculated state
bool seqButtonsNewQuickpress[16]     = {0};     // Track if there was a new press + release
elapsedMillis seqButtonOntime[16]    = {0};     // How long have buttone been pressed   
                         
// Joystick & pots
uint8_t  knobAndJoystickNow[8]       = {0};
uint8_t  knobAndJoystickPrev[8]      = {0};
bool     knobAndJoystickNewPress[8]  = {0};     // Did the value change enough to count as a new press?
uint8_t  sliderPotsNow[2]            = {0};
int8_t   slidePotBigchangeCounter[2] = {0};     // Used to determine if user rapidly changed value for a bit. Usually used as a param reset trigger.
uint8_t  sliderPotsPrev[2]           = {0};
bool     sliderPotsNewPress[2]       = {0};
uint16_t joystickGuiLastX;
uint16_t joystickGuiLastY;

// Play button
bool     playButtoNow, playButtonState, playButtonQuickpress, 
         playButtonDoublePress, playButtonPrev, playButtonStatePrev = false;
uint32_t playButtonLastQuickpressMillis = millis();                      // Track the last quick press - for detecting double presses
uint32_t lastInputChangeMillis = millis();

elapsedMillis playButtonOntime = 0;

/*
    ***************************** Input Checking **************************

      Figure out if we have any new button presses, if pots have changed, etc.

    ************************************************************************
*/

bool checkAllInputs(void)                // Check the state of sequencer + update step objects
{
    bool haveAnyChanged = false;
    // Loop through all eight pins & update input arrays
    for (uint8_t pin=0; pin<8; pin++){

        selectMuxPin(pin);    
        delayMicroseconds(5);

        // 1. First save off prev button states
        seqButtonStatePrev[pin]     = seqButtonState[pin];
        seqButtonStatePrev[pin + 8] = seqButtonState[pin + 8];
        knobAndJoystickPrev[pin]    = knobAndJoystickNow[pin];

        // 2.  Now take new readings
        seqButtonNow[pin]           = !digitalRead(MUX_1_DATA_PIN);   // Invert b/c input pullup
        seqButtonNow[pin + 8]       = !digitalRead(MUX_2_DATA_PIN);
        knobAndJoystickNow[pin]     = map(analogRead(MUX_3_DATA_PIN), 0, 1023, 0, 100);

        // 2.5  Catch any new press + releases
        if (!seqButtonNow[pin] && seqButtonStatePrev[pin] && seqButtonOntime[pin] < INPUT_LONGPRESS_MILLIS) seqButtonsNewQuickpress[pin] = true;
        if (!seqButtonNow[pin + 8] && seqButtonStatePrev[pin + 8] && seqButtonOntime[pin + 8] < INPUT_LONGPRESS_MILLIS) seqButtonsNewQuickpress[pin + 8] = true;

        // 3. Reset button timer if it's off now
        if (!seqButtonNow[pin])     seqButtonOntime[pin] = 0;
        if (!seqButtonNow[pin + 8]) seqButtonOntime[pin + 8] = 0;

        // 4. Determine if the press is high enough to count as a press, and set the button state
        (seqButtonOntime[pin] > debounceMillis) ? seqButtonState[pin] = true : seqButtonState[pin] = false;
        (seqButtonOntime[pin + 8] > debounceMillis) ? seqButtonState[pin + 8] = true : seqButtonState[pin + 8] = false;

        switch (pin)
        {
          case 3: // unused. Make sure it's always false.
            knobAndJoystickNewPress[pin] = false;  
            break;
          
          case 6: // Joystick button
            if ((knobAndJoystickNow[pin] == 0 && knobAndJoystickPrev[pin] !=0))
            {
              knobAndJoystickNewPress[pin] = true;
            } else {
              knobAndJoystickNewPress[pin] = false;
            }
            break;

          default: // Everyting else on mux2 (knobs & joystick x + y)
            if (abs(knobAndJoystickNow[pin] - knobAndJoystickPrev[pin]) > analogChangeThres)
            {
              knobAndJoystickNewPress[pin] = true;
             } else {
              knobAndJoystickNewPress[pin] = false;
              knobAndJoystickNow[pin] = knobAndJoystickPrev[pin];
             }
            break;
        }

        if (knobAndJoystickNewPress[pin]) haveAnyChanged = true;
    }

    if (testing && haveAnyChanged) z__printArrayToSerial(8, knobAndJoystickNewPress);
  
    // Check Slide pots
    for (uint8_t i = 0; i < 2; i++)
    {
      sliderPotsPrev[i] = sliderPotsNow[i];
      sliderPotsNow[i] = map(analogRead(slidePotPins[i]), 0, 1023, 0, 100);
      if (abs(sliderPotsNow[i] - sliderPotsPrev[i]) > analogChangeThres){
        sliderPotsNewPress[i] = true;
      } else {
        sliderPotsNewPress[i] = false;
        sliderPotsNow[i] = sliderPotsPrev[i]; // to prevent messing up slowww pot changes. 
      }

      // Catch when user is wildly changing value (as another input type we can use for wahtever)
      (abs(sliderPotsNow[i] - sliderPotsPrev[i]) > analogBigChangeThresh) ? slidePotBigchangeCounter[i] += 2 : slidePotBigchangeCounter[i]--;
      if (slidePotBigchangeCounter[i] < 0) slidePotBigchangeCounter[i] = 0;
      if (slidePotBigchangeCounter[i] > 15) slidePotBigchangeCounter[i] = 15;
      if (sliderPotsNewPress[i]) haveAnyChanged = true;
    }

    if (testing && haveAnyChanged)  z__printArrayToSerial(2, slidePotBigchangeCounter);
  
    // Check the play button
    playButtonStatePrev         = playButtonState;
    playButtoNow                = !digitalRead(PLAYBUTTON_PIN);

    if (!playButtoNow && playButtonStatePrev && playButtonOntime < INPUT_LONGPRESS_MILLIS && playButtonOntime > debounceMillis)    // Button was released, and was not held for too long
      {
        playButtonQuickpress = true;
        if (millis() - playButtonLastQuickpressMillis < 500) playButtonDoublePress = true;
        playButtonLastQuickpressMillis = millis();
      }

    (playButtoNow && playButtonOntime > debounceMillis) ? playButtonState = true : playButtonState = false;
    if (!playButtoNow) playButtonOntime = 0;

    if (haveAnyChanged) lastInputChangeMillis = millis();
    int32_t millisDiff = millis() - lastInputChangeMillis;
    if (millisDiff > INPUT_CHG_DISP_TIME) {
        refreshInputDisp();
        lastInputChangeMillis = millis() + 86400000;                 // TLDR; dont refresh again until another knob changes
    }
  
    return haveAnyChanged; 
}

/*
*******************************  ~~ Input Processing  ~~  **************************

        - Do stuff based on new inputs
            - Route based on current screen / button pressed, etc.
        - Reset input states so we can listen for the next new presss

************************************************************************************
*/

void processInputs(Sequencer& seq)
{
  bool seqButtonHeld = false;        

  // -- Process Sequecer buttons -- //
  for (uint8_t i = 0; i < seq.getNumSteps(); i++)
  {
    //   1)  new quck press - set or remove a step
    if (seqButtonsNewQuickpress[i]) 
    {
      seq.stepFlipStateAtIndex(i);
      (seq.getStepStateAtIndex(i)) ? strip.setPixelColor(i, seq.getStepColorAtIndex(i)) : strip.setPixelColor(i  , 0);
      seqButtonsNewQuickpress[i] = false;            
    }

    //   2)  Button is being held... listen for other knob twists n such
    if (seqButtonOntime[i] > INPUT_LONGPRESS_MILLIS)
    {
      seqButtonHeld = true;
      drawInfoBar("Editing Step: ", i);
      draw_step_held_arrow_at_IDX(i, BLUE_5);

      // Button held and Pot/Knob 1 Changed (input idx: 0)
      if (knob1Changed()){
        int8_t input_idx = 0;
        update_input_disp_dotline(input_idx);
        update_input_label_IDX_step(input_idx, true);
        run_input_change_function_step(seq, input_idx, i);
      }

      // Button held and Pot/Knob 2 Changed (input idx: 1)
      if (knob2Changed()){
        uint8_t input_idx = 1;
        update_input_disp_dotline(input_idx);
        update_input_label_IDX_step(input_idx, true);
        run_input_change_function_step(seq, input_idx, i);
      }

      // Button held and Pot/Knob 3 Changed (input idx: 2)
      if (knob3Changed()){
        uint8_t input_idx = 2;

        update_input_disp_dotline(input_idx);
        update_input_label_IDX_step(input_idx, true);
        run_input_change_function_step(seq, input_idx, i);
      }
      
      // Button held and Pot/Knob 4 Changed (input idx: 3)
      if (knob4Changed()){
        uint8_t input_idx = 3;

        update_input_disp_dotline(input_idx);
        update_input_label_IDX_step(input_idx, true);
        run_input_change_function_step(seq, input_idx, i);
      }

      // Button held and Slider A Changed (input idx: 4)
      if (sliderAChanged()){
        uint8_t input_idx = 4;

        update_slider_display_val_IDX(input_idx);
        run_input_change_function_step(seq, input_idx, i);
      }
 
      // Button held and Slider B Changed (input idx: 5)
      if (sliderBChanged()){
        uint8_t input_idx = 5;
        
        update_slider_display_val_IDX(input_idx);
        run_input_change_function_step(seq, input_idx, i);
      }
    }
  }

  // ---- Process Play Button ----- //
  if (playButtonQuickpress)
  { 
    drawCurrentTitleBar();
    Serial.println("pressed play");
    seq.flipPlayState();       // Play or stop
    if (seq.getPlayingState()) seq.resetCounter();    // If going from stopped to playing, reset counter.
    playButtonQuickpress = false;
  }

  if (playButtonDoublePress)
  {
    Serial.println("play button doublepress");
    seq.reset();
    playButtonDoublePress = false;
  }

  if (playButtonOntime > INPUT_LONGPRESS_MILLIS)            // Holding play button... go to master settings
  {
    Serial.println("play button hold");
  }

  if (seqButtonHeld) return;                         // Bail here if a button is being held.. these inputs were already handled above.

  // ---- Process Sliders ------- //
  if (sliderAResetTrigger()) seq.resetSwing();       // djt - need to do this conditionally.. but hardcoding to swing for now

  // ----- Process Pots --------- //
  if (knob1Changed())                 
  {
    uint8_t input_idx = 0;
    update_input_disp_dotline(input_idx);
    update_input_label_IDX_global(input_idx, true);
    run_input_change_function_global(input_idx);
  }
  if (knob2Changed())                 
  {
    uint8_t input_idx = 1;
    update_input_disp_dotline(input_idx);
    update_input_label_IDX_global(input_idx, true);
    run_input_change_function_global(input_idx);
  }
  if (knob3Changed())                 
  {
    uint8_t input_idx = 2;
    update_input_disp_dotline(input_idx);
    update_input_label_IDX_global(input_idx, true);
    run_input_change_function_global(input_idx);
  }
  if (knob4Changed())
  {
    uint8_t input_idx = 3;
    update_input_disp_dotline(input_idx);
    update_input_label_IDX_global(input_idx, true);
    run_input_change_function_global(input_idx);
  }

  // ------ Process Joystick ----- //

  if (joystickXChanged())
  {
    ;
  }
  if (joystickYChanged())
  {
    ;
  }
  if (joystickXChanged() || joystickYChanged())
  {
    uint16_t x = map(int(joystickXVal()),0,100,0,SCREEN_WIDTH);
    uint16_t y = map(int(joystickYVal()),0,100,0,SCREEN_WIDTH);
    drawNoteSymbol(joystickGuiLastX, joystickGuiLastY,2,BLACK);
    drawNoteSymbol(x,y,2,RED_5);
    joystickGuiLastX = x;
    joystickGuiLastY = y;
  }
}

/*
************************** Helper Functions ************************

                         internal-y functions 

********************************************************************
*/

void inputSetup(void)
{
  for (int i=0; i<3; i++)             // Set select pins as outputs
  {
    pinMode(selectPins[i], OUTPUT);
    digitalWrite(selectPins[i], HIGH);
  }
    pinMode(MUX_1_DATA_PIN, INPUT_PULLUP); 
    pinMode(MUX_2_DATA_PIN, INPUT_PULLUP);
    pinMode(MUX_3_DATA_PIN, INPUT); 

    pinMode(SLIDEPOT_LEFT_PIN, INPUT);
    pinMode(SLIDEPOT_RIGHT_PIN, INPUT);
    pinMode(PLAYBUTTON_PIN, INPUT_PULLUP);

}

void selectMuxPin(uint8_t pin)          // Switch all mux inputs
{
  for (int i=0; i<3; i++)
  {
    if (pin & (1<<i))
      digitalWrite(selectPins[i], HIGH);
    else
      digitalWrite(selectPins[i], LOW);
  }
}

// Knobs
bool     knob1Changed          (void){return knobAndJoystickNewPress[0];}         // Returns true if twist pot 1 changed, false otherwise
bool     knob2Changed          (void){return knobAndJoystickNewPress[1];}         // Returns true if twist pot 2 changed, false otherwise
bool     knob3Changed          (void){return knobAndJoystickNewPress[2];}         // Returns true if twist pot 3 changed, false otherwise
bool     knob4Changed          (void){return knobAndJoystickNewPress[4];}         // Returns true if twist pot 4 changed, false otherwise
uint8_t  knob1Val              (void){return knobAndJoystickNow[0];}                  // Returns Current value of Knob 1
uint8_t  knob2Val              (void){return knobAndJoystickNow[1];}                  // Returns Current value of Knob 2
uint8_t  knob3Val              (void){return knobAndJoystickNow[2];}                  // Returns Current value of Knob 3
uint8_t  knob4Val              (void){return knobAndJoystickNow[4];}                  // Returns Current value of Knob 4

// Joystick inputs
bool     joystickYChanged     (void){return knobAndJoystickNewPress[7];}     // Returns true if Y value of joystick changed
bool     joystickXChanged     (void){return knobAndJoystickNewPress[5];}     // Returns true if X value of joystick changed
bool     joystickButtonChanged(void){return knobAndJoystickNewPress[6];}  

uint8_t  joystickYVal         (void){return knobAndJoystickNow[7];}              // Returns Current val of Joystick Y
uint8_t  joystickXVal         (void){return knobAndJoystickNow[5];}              // Returns Current val of Joystick X
uint8_t  joystickButtonVal    (void){return knobAndJoystickNow[6];} 
uint8_t  joystickXValPrev     (void){return knobAndJoystickPrev[7];}
uint8_t  joystickYValPrev     (void){return knobAndJoystickPrev[5];}

// Slider inputs
bool     sliderAChanged       (void){return sliderPotsNewPress[0];}            // Returns true if slider A changed
bool     sliderBChanged       (void){return sliderPotsNewPress[1];}            // Returns true if slider B changed
uint8_t  sliderAVal           (void){return sliderPotsNow[0];}                     // Returns current val of Slider A
uint8_t  sliderBVal           (void){return sliderPotsNow[1];}                     // Returns current val of Slider B

/*
                                   Input IDX Info:

        I shoulda used one big array to store all current / prev input values, but did not.
        Because of this, we have to do some mappping for use in other functions that just
        accept an input index and need to do something. Shuld re-think this later..

        IDX - Input
        -----------
        0   - Pot / Knob 1
        1   - Pot / Knob 2
        2   - Pot / Knob 3
        3   - Pot / Knob 4
        4   - Slider A / 1
        5   - Slider A / 2
        6   - Joystick X
        7   - Joystick Y
        8   - Joystick Button
        9   - Play button
*/

uint8_t getInputValueByIDX(uint8_t idx)
{
  switch (idx)
  {
  case 0: return knob1Val();
  case 1: return knob2Val();
  case 2: return knob3Val();
  case 3: return knob4Val();
  case 4: return sliderAVal();
  case 5: return sliderBVal();
  case 6: return joystickXVal();
  case 7: return joystickYVal();
  case 8: return joystickButtonVal();
  case 9: return playButtonState;
  default: break;
  }
}

bool sliderAResetTrigger(void)                                          // Returns true if user is hella wiggling Slider A
{
  if (slidePotBigchangeCounter[0] > 6) 
  {
    slidePotBigchangeCounter[0] > 0;
    return true;
  }
  return false;
}   

bool sliderBResetTrigger(void)                                        // Returns true if user is hella wiggling Slider B
{
  if (slidePotBigchangeCounter[1] > 6) 
  {
    slidePotBigchangeCounter[1] > 0;
    return true;
  }
  return false;
}                     

/*
********************* Utility / Debug Functions *******************

  Stuff to help with debugging, testing, etc. 
  Not used in actual functionality

********************************************************************
*/

// Utility function to help debug array values
void z__printArrayToSerial(uint8_t size, uint16_t array[])
{
    //Serial.println("---\t---\t---\t---\t---\t---\t---\t---");
    Serial.println();
    for(uint8_t i = 0; i < size; i++)
    {
    Serial.print(String (array[i]) + "\t");
    }
}

void z__printArrayToSerial(uint8_t size, bool array[])
{
    //Serial.println("---\t---\t---\t---\t---\t---\t---\t---");
    Serial.println();
    for(uint8_t i = 0; i < size; i++)
    {
    Serial.print(String (array[i]) + "\t");
    }
}

// Utility function to help debug array values
void z__printArrayToSerial(uint8_t size, int16_t array[])
{
    //Serial.println("---\t---\t---\t---\t---\t---\t---\t---");
    Serial.println();
    for(uint8_t i = 0; i < size; i++)
    {
    Serial.print(String (array[i]) + "\t");
    }
}

// Utility function to help debug array values
void z__printArrayToSerial(uint8_t size, int8_t array[])
{
    //Serial.println("---\t---\t---\t---\t---\t---\t---\t---");
    Serial.println();
    for(uint8_t i = 0; i < size; i++)
    {
    Serial.print(String (array[i]) + "\t");
    }
}
