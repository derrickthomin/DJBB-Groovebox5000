#pragma once
//#include "Inputs.h"
#include <Adafruit_NeoPixel.h>
#include <vector>
#include "Common.h"
#include "Audiostuff.h"
#include "settings.h"

extern Adafruit_NeoPixel strip;

// Need to track each step individually for param locks and suck
class Step {
    private:
        Voice*   voice;                    // Store pointer to a voice object
        bool     state;                     // Is it on or nah?
        bool     played;                    // True if played, false if not. Resets when loops around.
        float    volume;
        int32_t  swingMcros;              // How much swing to apply (positive or negative to hit early or late.)
        uint16_t attack;
        uint16_t decay;
        uint16_t sustain;
        uint16_t release;
        uint8_t  reverbSendLevel;  
        uint8_t  delaySendLevel;
        uint8_t  ratchetCount;
        uint8_t  assignedVoice;            // Track which voice to use when this hits.
        uint32_t color;
        uint8_t  colorSetIDXstp;

//      ---- Drum voice settings ----
        uint16_t drumFreq;                 // Drum voice frequency 
        uint16_t drumLength;               // How long is the hit
        float    drumPMod;                 // Pitch mod
        float    drum2ndHitMix;            // Level of 2nd hit

//      -----   FX n Filters  ------
        uint8_t  bitcrushDepth;           // Bitcrush dept 1-16
        uint16_t bitcrushSampRate;        // Up to 44.1k
        uint16_t filterFreq;              // 0 - 20k
        float    filterQ;                 // 0.7 - 5. Above 0.707 will add gain.
        float    filter_LP_amt;           // 1.0 = passthru, less = attenuate
        float    filter_HP_amt;           // DJT IMPLEMENT ME... need to figure out how ot handl ethe mixer inputs..  dont want too many params to voice constructor   
    
    public:
        Step(uint8_t colorSetIDX);

        void     playNote(void);          // Play note from the current voice
        void     assignVoice(Voice* vOice){voice = vOice;}

        // -------- Getters --------//
        uint16_t getStepAttack  (void);
        bool     getStepState   (void) {return state;}
        int32_t  getSwingMicros (void) {return swingMcros;}
        bool     getPlayingState(void){return played;}
        uint32_t getColor       (void){return color;}
        uint8_t  getVoiceNumber (void){return voice -> getID();}
        uint16_t getVoiceAttack (void){return voice -> getAttack();}

        // -------- Setters --------//  ---- ALL expect an input of 0 - 100.
        void setColor          (uint32_t colour){color = colour;}
        void flipState         (void){state = !state;}
        void stepOn            (void) {state = true;}
        void stepOff           (void) {state = false;}
        void setAttack         (uint8_t val);
        void setRelease        (uint8_t val);
        void setDecay          (uint8_t val);
        void setSustain        (uint8_t val);

        void setRatchetCount   (uint8_t val);
        void setSwingMicros    (int32_t swingMicr);
        void setVolume         (uint8_t val);
        void setPlayingState   (bool playstate);
        
        // drum voice
        void setDrumPMod       (uint8_t val);
        void setDrumLength     (uint8_t val);
        void setDrumMix2       (uint8_t val);
        void setDrumFreq       (uint8_t val);

        // fx n filter
        void setBitCrushDepth  (uint8_t val);
        void setBitCrushRate   (uint8_t val);
        void setFiltFreq       (uint8_t val);
        void setFilterQ        (uint8_t val);
};

// Class for our sequencer. Track bpm, num steps, etc.
// DJT - assume 16th notes for now
class Sequencer {

    private:
        elapsedMicros seqElapsedMicros;
        unsigned long microsecondsPerStep;
        bool          playingState;              // Is the sequencer currently playing
        bool          stepPlayed;                // Track if we already played a note for this step
        uint8_t       numSteps;
        uint8_t       currentStep;
        int8_t        lastPlayedStep;
        uint8_t       bpm;
        uint8_t       startingStep;
        int32_t       microsecondsNextStep;      // djt - do we need this? prob not...
        uint32_t      microsecondsNextNote;      // Because of swing or other future FX, we may want hit early or late (out of sync with actual step)
        uint32_t      maxSwingMicros;            // Max swing allowed based on microsecs per step
        uint32_t      defaultStepColor;

    public:
        Sequencer(uint8_t, uint8_t);
        std::vector<Step> steps;
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
        uint8_t getStartingStep(void);
        bool    getPlayingState(void);
        uint8_t getLastPlayedStep(void);
        bool    getStepStateAtIndex(uint8_t idx);
        uint8_t getCurrentStepNumber(void);
        uint8_t getNumSteps(void);
        bool    getCurrentStepState(void);
        bool    getPreviousStepState(void);
        bool    getNextStepState(void);
        uint8_t getNextStepNumber(void);
        uint8_t getPrevStepNumber(void);
        int32_t getNextStepSwing(void);        // Get the millis offset for the next step (pos or neg)
        int32_t getCurrentStepSwing(void);     // Get swing value of current step (pos or neg)
        int32_t getStpSwingAtIndex(uint8_t idx);
        uint8_t getCurrentStepVoice(void){return steps[getCurrentStepNumber()].getVoiceNumber()-1;}
        
        int32_t getDefaultStepColor(void){return defaultStepColor;}  // All steps are the same color... just return the color of the 1st
        int32_t getCurrentStepColor(void){return steps[currentStep].getColor();}
        int32_t getPreviousStepColor(void){return steps[getPrevStepNumber()].getColor();}

        bool    getPlayStateAtIndex(uint8_t idx);
        bool    getCurrentStepPlayedState(void){return steps[currentStep].getPlayingState();}
        bool    getNextStepPlayedState(void){return steps[getNextStepNumber()].getPlayingState();}

        uint16_t  getCurrentVoiceAttack(void){return steps[getCurrentStepNumber()].getVoiceAttack();}
        uint32_t  getStepColorAtIndex(uint8_t idx);


        // -------- Setters --------//
        void setBpm(uint8_t newbpm);                  // Change the BPM. Handles max / min bpm, updating other sequencer properities such as microsecs / step
        void setStepNumber(uint8_t step);
        void flipPlayState(void);
        void resetCounter(void);
        void setCurrentStepPlayedState(bool state){steps[currentStep].setPlayingState(state);}
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