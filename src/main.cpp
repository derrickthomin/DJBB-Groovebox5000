#include <Arduino.h>
#include <Wire.h>
#include <Audio.h>
#include <SerialFlash.h>
#include <vector>
#include <string>

#include <Adafruit_NeoPixel.h>   // Neopixel strips
#include <Adafruit_SSD1351.h>    // OLED Library
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <SD.h>

#include "Inputs.h"
#include "Audiostuff.h"
#include "Sequencer.h"
#include "Display.h"

#define NUM_LEDS          16
#define NEOPIX_PIN        9
#define SDCARD_CS_PIN     10
#define SDCARD_MOSI_PIN   7
#define SDCARD_SCK_PIN    14
#define NEOPIX_BRIGHTNESS 2    // Out of like 200 or som. 

// ********** SET TO TRUE TO PRINT TEST STUFF **************

const bool stepTesting = false;               // Set to false to get rid of serial print stuff
uint32_t   last_time_print = millis();

// ********** SET TO TRUE TO PRINT TEST STUFF **************

bool sdStatus;            // sd card status
Sequencer sequencer_1 (16, 120);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, NEOPIX_PIN, NEO_GRB + NEO_KHZ800);
unsigned long inputCheckTimer = millis();
unsigned long inputCheckPeriod = 50;       // Only check every 50 ms
 
void setup()
{
  Serial.begin(20000);       
  sequencer_1.initializeSteps();  
  initAudio();
  Voice::assignDefaultVoicesForSequencer(&sequencer_1);
  inputSetup();
  initScreens();
  initOled();
  strip.begin();
  strip.setBrightness(NEOPIX_BRIGHTNESS);
  draw_cur_seq_oled();

  // //------ SD Card
  Sd2Card card;
  SdVolume volume;
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  sdStatus = card.init(SPI_FULL_SPEED, SDCARD_CS_PIN);
  volume.init(card);
  File root = SD.open("/");
}

void loop()
{
  bool inputsChanged;
  if (millis() - inputCheckTimer > inputCheckPeriod)
  {
    inputsChanged = checkAllInputs();   // 1. Check all of our inputs
    processInputs(sequencer_1);         // 2. If any changed, do something in response.
    inputCheckTimer = millis();
    checkDisplayContext();                     // 3. See if we need to erase an info bar. 
  }

  if (!sequencer_1.getPlayingState()){  // Not currently playing - bail early.
    strip.show();
    return;
  }

  if (sequencer_1.newNote())
  {
    sequencer_1.steps[sequencer_1.getlastPlayedStepIDX()].playNote();                             // Last played step is kinda weird. It represents the step we want to play NOW since we already incremented it in newnote.
    sequencer_1.calcNextNoteTime(true);  
  }

  if (sequencer_1.newStep()) 
  { 
    sequencer_1.calcNextNoteTime(false);
    strip.setPixelColor(sequencer_1.getCurrentStepIDX(), sequencer_1.color);                 // Light Current neopixel
    strip.setPixelColor(sequencer_1.getPrevStepIDX() , 0);                                   // Clear previous neopixel
    advance_cur_seq_step_oled(); // update olde DJT TESTIN GZZZZZ

    if (sequencer_1.getPreviousStepState())
    {
      strip.setPixelColor(sequencer_1.getPrevStepIDX() , sequencer_1.getPreviousStepColor());      // Put previous one back to the on color if needed
    }

    sequencer_1.clearPrevPlayingState();
    strip.show();
  }
    

  //Enable for memory and processor  diagnostics
  // if (millis()-last_time_print > 100){
  //   Serial.print(F("all="));
  //   Serial.print(AudioProcessorUsage());
  //   Serial.print(F(","));
  //   Serial.print(AudioProcessorUsageMax());
  //   Serial.print(F("    "));
  //   Serial.print(F("Memory: "));
  //   Serial.print(AudioMemoryUsage());
  //   Serial.print(F(","));
  //   Serial.println(AudioMemoryUsageMax());
  //   last_time_print = millis();
  // }
  
}

/*
    ***************************** Guide / Reference **************************

            Adding new functions based on inputs.
            1. For global - Create a function that takes a uint8t
            2. For step specific - ...

    ************************************************************************
*/