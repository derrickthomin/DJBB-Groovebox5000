// Houses logic to get inputs, and update any objects that need the updates
#include "Inputs.h"
#include "Sequencer.h"
#include "Display.h"
#include "Audiostuff.h"

#define NUM_STEP_BUTTONS  16
#define NUM_TWISTPOTS     4
#define NUM_SLIDEPOTS     2

#define MUX_CONTROL_S0    2
#define MUX_CONTROL_S1    3
#define MUX_CONTROL_S2    4
#define MUX_1_DATA_PIN    5               // DIGITAL - 1st 8 sequence buttons
#define MUX_2_DATA_PIN    6               // DIGITAL - 2nd 8 sequence buttons
#define MUX_3_DATA_PIN    16              // ANALOG  - Handles twist pots + joystick + joystick click
#define SLIDEPOT_LEFT_PIN 25              // Left of the 2 slide pots
#define SLIDEPOT_RIGHT_PIN 24
#define PLAYBUTTON_PIN     31             // Mushy little buton 

// ********** SET TO TRUE TO PRINT TEST STUFF **************

const bool testing = true;               // Set to false to get rid of serial print stuff

// ********** SET TO TRUE TO PRINT TEST STUFF **************

const uint8_t  selectPins[3]         = {MUX_CONTROL_S0, MUX_CONTROL_S1, MUX_CONTROL_S2};
uint8_t        slidePotPins[2]       = {SLIDEPOT_LEFT_PIN, SLIDEPOT_RIGHT_PIN};
const uint8_t  analogChangeThres     = 1;       // Dont count as changed unless it changed by this much. (note this is applied after mapping to 0 - 100)
const uint16_t debounceMillis        = 10;      // If button is on for this long, it's on forreal
const uint16_t longPressMillis       = 750;     // How many millisecondsd count as a long button press
const uint8_t  analogBigChangeThresh = 30;      // Big swing in analog value (out of 100)

// Sequencer Buttons
bool seqButtonNow[16]                = {0};     // Literally what is it this instant
bool seqButtonState[16]              = {0};     // What do we calculate the state to be
bool seqButtonStatePrev[16]          = {0};     // What was the last calculated state
bool seqButtonsNewQuickpress[16]     = {0};     // Track if there was a new press + release
elapsedMillis seqButtonOntime[16]    = {0};     // How long have buttone been pressed   
                         
// Joystick & pots
uint16_t knobAndJoystickNow[8]       = {0};
uint16_t knobAndJoystickPrev[8]      = {0};
bool     knobAndJoystickNewPress[8]  = {0};     // Did the value change enough to count as a new press?
uint16_t sliderPotsNow[2]            = {0};
int8_t   slidePotBigchangeCounter[2] = {0};     // Used to determine if user rapidly changed value for a bit. Usually used as a param reset trigger.
uint16_t sliderPotsPrev[2]           = {0};
bool     sliderPotsNewPress[2]       = {0};

// Play button
bool playButtoNow, playButtonState, playButtonQuickpress, playButtonDoublePress = false;
bool playButtonPrev, playButtonStatePrev = false;
elapsedMillis playButtonOntime = 0;
uint32_t playButtonLastQuickpressMillis = millis();                      // Track the last quick press - for detecting double presses
uint16_t joystickGuiLastX;
uint16_t joystickGuiLastY;

/*
    ***************************** Input Checking **************************

      Figure out if we have any new button presses, if pots have changed, etc.

    ************************************************************************
*/

bool checkAllInputs(void)                // Check the state of sequencer + update step objects
{
    bool haveAnyChanged = false;

    // Loop through all eight pins & update input arrays
    for (uint8_t pin=0; pin<8; pin++)
    {
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
        if (!seqButtonNow[pin] && seqButtonStatePrev[pin] && seqButtonOntime[pin] < longPressMillis) seqButtonsNewQuickpress[pin] = true;
        if (!seqButtonNow[pin + 8] && seqButtonStatePrev[pin + 8] && seqButtonOntime[pin + 8] < longPressMillis) seqButtonsNewQuickpress[pin + 8] = true;

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
              knobAndJoystickNow[pin] = knobAndJoystickPrev[pin];
            }
            break;

          default: // Everyting else on mux2 (knobs & joystick x + y)
            (abs(knobAndJoystickNow[pin] - knobAndJoystickPrev[pin]) > analogChangeThres) ? knobAndJoystickNewPress[pin] = true : knobAndJoystickNewPress[pin] = false;
            break;
        }

        if (knobAndJoystickNewPress[pin]) haveAnyChanged = true;
    }

    if (testing && haveAnyChanged)
    {
       z__printArrayToSerial(8, knobAndJoystickNewPress);
    }

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

    if (!playButtoNow && playButtonStatePrev && playButtonOntime < longPressMillis && playButtonOntime > debounceMillis)    // Button was released, and was not held for too long
      {
        playButtonQuickpress = true;
        if (millis() - playButtonLastQuickpressMillis < 500) playButtonDoublePress = true;
        playButtonLastQuickpressMillis = millis();
      }

    (playButtoNow && playButtonOntime > debounceMillis) ? playButtonState = true : playButtonState = false;
    if (!playButtoNow) playButtonOntime = 0;

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
    if (seqButtonOntime[i] > longPressMillis)
    {
      seqButtonHeld = true;
      drawInfoBar("Editing Step: ", i);
      if (sliderAChanged())
      {
        int8_t sliderAValMapped = map(sliderAVal(), 0, 100, -50, 50);
        int8_t sliderBValMapped = map(sliderBVal(), 0, 100, -50, 50);
        update_sliders(sliderAValMapped, sliderBValMapped);
        seq.setSwingAtIndex(i, sliderAValMapped);
      }
      if (sliderBChanged())
      {
        Serial.println("slider b changed");
        int8_t sliderAValMapped = map(sliderAVal(), 0, 100, -50, 50);
        int8_t sliderBValMapped = map(sliderBVal(), 0, 100, -50, 50);
        uint16_t  envelopeVal = ((sliderBVal()+1) * 10);
        update_sliders(sliderAValMapped, sliderBValMapped);
        seq.setStepDecayAtIndex(i, envelopeVal);
      }

      if (knob2Changed())
      {
        seq.setStepAttackAtIndex(i, knob2Val());
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

  if (playButtonOntime > longPressMillis)            // Holding play button... go to master settings
  {
    Serial.println("play button hold");
    // if (knob1Changed()) setHeadphoneVolume(knob1Val());
  }

  if (seqButtonHeld) return;                         // Bail here if a button is being held.. these inputs were already handled above.

  // ---- Process Sliders ------- //
  if (sliderAResetTrigger()) seq.resetSwing();       // djt - need to do this conditionally.. but hardcoding to swing for now

  // ----- Process Pots --------- //
  if (knob1Changed())                 
  {
    setHeadphoneVolume(knob1Val());                  // Knob 1 == volume by default
    draw_pot_val_bottomscreen(1, knob1Val());
  }
  if (knob2Changed())                 
  {
    draw_pot_val_bottomscreen(2, knob2Val());
  }
  if (knob3Changed())                 
  {
    draw_pot_val_bottomscreen(3, knob3Val());
  }
  if (knob4Changed())                 
  {
    draw_pot_val_bottomscreen(4, knob4Val());
  }


  // ------ Process Joystick ----- //
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
bool     knob1Changed(void){return knobAndJoystickNewPress[0];}         // Returns true if twist pot 1 changed, false otherwise
bool     knob2Changed(void){return knobAndJoystickNewPress[1];}         // Returns true if twist pot 2 changed, false otherwise
bool     knob3Changed(void){return knobAndJoystickNewPress[2];}         // Returns true if twist pot 3 changed, false otherwise
bool     knob4Changed(void){return knobAndJoystickNewPress[4];}         // Returns true if twist pot 4 changed, false otherwise
uint8_t  knob1Val(void){return knobAndJoystickNow[0];}                  // Returns Current value of Knob 1
uint8_t  knob2Val(void){return knobAndJoystickNow[1];}                  // Returns Current value of Knob 2
uint8_t  knob3Val(void){return knobAndJoystickNow[2];}                  // Returns Current value of Knob 3
uint8_t  knob4Val(void){return knobAndJoystickNow[4];}                  // Returns Current value of Knob 4

// Joystick inputs
bool     joystickYChanged(void){return knobAndJoystickNewPress[7];}     // Returns true if Y value of joystick changed
bool     joystickXChanged(void){return knobAndJoystickNewPress[5];}     // Returns true if X value of joystick changed
uint8_t  joystickYVal(void){return knobAndJoystickNow[7];}              // Returns Current val of Joystick Y
uint8_t  joystickXVal(void){return knobAndJoystickNow[5];}              // Returns Current val of Joystick X
uint8_t  joystickXValPrev(void){return knobAndJoystickPrev[7];}
uint8_t  joystickYValPrev(void){return knobAndJoystickPrev[5];}

// Slider inputs
bool     sliderAChanged(void){return sliderPotsNewPress[0];}            // Returns true if slider A changed
bool     sliderBChanged(void){return sliderPotsNewPress[1];}            // Returns true if slider B changed
uint16_t sliderAVal(void){return sliderPotsNow[0];}                     // Returns current val of Slider A
uint16_t sliderBVal(void){return sliderPotsNow[1];}                     // Returns current val of Slider B


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
