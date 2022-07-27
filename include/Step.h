#pragma once
#include <Adafruit_NeoPixel.h>
#include <vector>
#include "Common.h"
#include "Audiostuff.h"
#include "settings.h"

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
