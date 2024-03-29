#pragma once
//#include "Inputs.h"
#include <Adafruit_NeoPixel.h>
#include <vector>
#include <queue>
#include "Common.h"
#include "Audiostuff.h"
#include "settings.h"
#include "Step.h"

extern Adafruit_NeoPixel strip;

// Class for our sequencer. Track bpm, num steps, etc.
// DJT - assume 16th notes for now
class Sequencer {

    private:
        elapsedMicros seqElapsedMicros;
        unsigned long microsecondsPerStep;
        bool          playingState;              // Is the sequencer currently playing
        bool          stepPlayed;                // Track if we already played a note for this step
        uint8_t       numSteps;
        uint8_t       currentStepIDX;
        int8_t        lastPlayedStepIDX;
        uint8_t       bpm;
        uint8_t       startStepIDX;
        int32_t       microsecondsNextStep;      // djt - do we need this? prob not...
        uint32_t      microsecondsNextNote;      // Because of swing or other future FX, we may want hit early or late (out of sync with actual step)
        uint32_t      maxSwingMicros;            // Max swing allowed based on microsecs per step
        uint32_t      defaultStepColor;

    public:
        Sequencer(uint8_t, uint8_t);
        uint16_t loopCount;                    // How many times have we loopd the sequence
        std::vector<Step> steps;
        std::queue<uint32_t> bonusHits;         // Store relative timing for any "bonus" hits, such as ratchet hits.
        static std::vector<Sequencer*> allSequencers;
        int8_t colorSetIDX;                // corresponsds to a set of 5 colors
        uint32_t color;                    // Color of the scrolling seq light
        static int8_t curSequencerIDX;
        static uint8_t numSequencers;
        void initializeSteps(void);
        bool newStep(void);
        bool newNote(void);                                         // Check if it's time for a new note to play. Not necessarily when the step hits (swing, etc.).
        void calcNextNoteTime(bool midStepCalc = false);            // Calculate the next hit time, and store it.
        void reset(void);                                           // Go back to initial step and reset counters
        void resetSwing(void);                                      // Reset swing of all steps to 0
        void resetPlayingStates(void);                              // Reset all play states, accounting for swing, etc.
        void clearPrevPlayingState(void);

        // -------- Getters --------//
        static  Sequencer* getCurrentSequencer(void);               // Return the current sequencer 
        uint8_t getstartStepIDX(void);
        bool    getPlayingState(void);
        uint8_t getlastPlayedStepIDX(void);
        bool    getStepStateAtIndex(uint8_t idx);
        uint8_t getCurrentStepIDX(void);
        uint8_t getNumSteps(void);
        bool    getcurrentStepIDXState(void);
        bool    getPreviousStepState(void);
        bool    getNextStepState(void);
        uint8_t getNextStepNumber(void);
        uint8_t getPrevStepIDX(void);
        int32_t getNextStepSwing(void);        // Get the millis offset for the next step (pos or neg)
        int32_t getcurrentStepIDXSwing(void);     // Get swing value of current step (pos or neg)
        int32_t getStpSwingAtIndex(uint8_t idx);
        uint8_t getcurrentStepIDXVoice(void){return steps[getCurrentStepIDX()].getVoiceNumber()-1;}
        
        int32_t getDefaultStepColor(void){return defaultStepColor;}  // All steps are the same color... just return the color of the 1st
        int32_t getcurrentStepIDXColor(void){return steps[currentStepIDX].getColor();}
        int32_t getPreviousStepColor(void){return steps[getPrevStepIDX()].getColor();}

        bool    getPlayStateAtIndex(uint8_t idx);
        bool    getcurrentStepIDXPlayedState(void){return steps[currentStepIDX].getPlayingState();}
        bool    getNextStepPlayedState(void){return steps[getNextStepNumber()].getPlayingState();}

        uint16_t  getCurrentVoiceAttack(void){return steps[getCurrentStepIDX()].getVoiceAttack();}
        uint32_t  getStepColorAtIndex(uint8_t idx);


        // -------- Setters --------//
        void setBpm(uint8_t newbpm);                  // Change the BPM. Handles max / min bpm, updating other sequencer properities such as microsecs / step
        void setStepNumber(uint8_t step);
        void flipPlayState(void);
        void resetCounter(void);
        void setcurrentStepIDXPlayedState(bool state){steps[currentStepIDX].setPlayingState(state);}
        void setNextStepPlayedState(bool state){steps[getNextStepNumber()].setPlayingState(state);}
        
        // -------> Index Funcions
        void stepOnAtIDX                 (uint8_t idx);
        void stepOffAtIDX                (uint8_t idx);
        void stepFlipStateAtIndex        (uint8_t idx);
        void setPlayStateAtIndex         (uint8_t idx, bool playstate);
        void setSwingAtIndex             (uint8_t idx, uint8_t swing);          // Pass a positive or negative value, from - 50 to 50
        void setStepAttackAtIndex        (uint8_t idx, uint8_t attack);
        void setStepReleaseAtIndex       (uint8_t idx, uint8_t release);
        void setStepDecayAtIndex         (uint8_t idx, uint8_t decay);
        void setStepSustainAtIndex       (uint8_t idx, uint8_t sustain);
        void setStepVolumeAtIndex        (uint8_t idx, uint8_t vol);
        void setStepSwingMillisAtIndex   (uint8_t idx, uint8_t swingMillis);           // Number of millis to add or subtract for step... for swing
        void setStepRatchetCountAtIndex  (uint8_t idx, uint8_t count);
        void setStepColorAtIndex         (uint8_t idx, uint16_t color);
        
        // djt - flesh me out

        // Bitcrush
        void setStepBitcrushDepthAtIndex (uint8_t idx, uint8_t depth);
        void setStepBitcurshRateAtIndex  (uint8_t idx, uint8_t freq);
        // Filter
        void setStepFiltFreqAtIndex      (uint8_t idx, uint8_t freq);
        void setStepFilterQAtIndex       (uint8_t idx, uint8_t q);
        // Drum stuffs
        void setDrumPModAtIndex          (uint8_t idx, uint8_t pmod);
        void setDrumLengthAtIndex        (uint8_t idx, uint8_t len);
        void setDrumMix2AtIndex          (uint8_t idx, uint8_t mix2);
        void setDrumFreqAtIndex          (uint8_t idx, uint8_t freq);

        // --------- Other --------- //
        void debugPrintSeqData(String message, bool extraSpace = false, bool detailed = true);
 
};

// ---- Callback functions for input handling ----- //
void cb_seq_set_bpm                        (uint8_t val);

void cb_set_attack                         (Sequencer& seq, uint8_t step_idx, uint8_t val);
void cb_set_release                        (Sequencer& seq, uint8_t step_idx, uint8_t val);
void cb_set_decay                          (Sequencer& seq, uint8_t step_idx, uint8_t val);
void cb_set_sustain                        (Sequencer& seq, uint8_t step_idx, uint8_t val);
void cb_set_volume                         (Sequencer& seq, uint8_t step_idx, uint8_t val);
void cb_set_ratchet                        (Sequencer& seq, uint8_t step_idx, uint8_t val);
void cb_set_playstate                      (Sequencer& seq, uint8_t step_idx, uint8_t val);
void cb_set_color                          (Sequencer& seq, uint8_t step_idx, uint8_t val);
void cb_set_bitcrushDepth                  (Sequencer& seq, uint8_t step_idx, uint8_t val);
void cb_set_bitcrushRate                   (Sequencer& seq, uint8_t step_idx, uint8_t val);
void cb_set_filterFreq                     (Sequencer& seq, uint8_t step_idx, uint8_t val);
void cb_set_filterQ                        (Sequencer& seq, uint8_t step_idx, uint8_t val);
void cb_set_drumPMod                       (Sequencer& seq, uint8_t step_idx, uint8_t val);
void cb_set_drumLen                        (Sequencer& seq, uint8_t step_idx, uint8_t val);
void cb_set_drumMix2                       (Sequencer& seq, uint8_t step_idx, uint8_t val);
void cb_set_drumFreq                       (Sequencer& seq, uint8_t step_idx, uint8_t val);
void cb_set_swing                          (Sequencer& seq, uint8_t step_idx, uint8_t val);

// Setters
void setCurrSeqSwingAtIndex(uint8_t step_idx, uint8_t pot_idx);         // Pass a positive or negative value, from - 50 to 50