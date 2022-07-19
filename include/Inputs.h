#pragma once

#include "Sequencer.h"
#include <Arduino.h>

bool checkAllInputs(void);
bool checkSeqButtons(void);
void selectMuxPin(uint8_t pin);
void z__printArrayToSerial(uint8_t size, uint16_t array[]);
void z__printArrayToSerial(uint8_t size, int8_t array[]);
void z__printArrayToSerial(uint8_t size, bool array[]);
void inputSetup(void);
void processInputs(Sequencer& seq);


/*
*********** Helper Functions ***************
          internal-y functions 
******************************************* 
*/
uint8_t getInputValueByIDX(uint8_t idx);

bool knob1Changed(void);
bool knob2Changed(void);
bool knob3Changed(void);
bool knob4Changed(void);
uint8_t knob1Val(void);
uint8_t knob2Val(void);
uint8_t knob3Val(void);
uint8_t knob4Val(void);

bool joystickYChanged(void);
bool joystickXChanged(void);
bool joystickButtonChanged(void);

uint8_t joystickYVal(void);
uint8_t joystickXVal(void);
uint8_t  joystickButtonVal(void);

uint8_t joystickXValPrev(void);
uint8_t joystickYValPrev(void);

bool sliderAChanged(void);
bool sliderBChanged(void);

uint8_t sliderAVal(void);
uint8_t sliderBVal(void);

bool sliderAResetTrigger(void);
bool sliderBResetTrigger(void);