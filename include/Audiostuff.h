#pragma once
#include <Audio.h>
#include <vector>
#include "Common.h"

#define STARTUP_NUM_VOICES 4    // How many voices to created
// #define DEFAULT_FILT_FREQ 17000
// #define DEFAULT_FILT_Q    0.7
// #define DEFAULT_CRUSH_BITDEPTH  16
// #define DEFAULT_CRUSH_FREQ     44100
// #define DEFAULT_NOISE_LVL      0.5
// #define DEFAULT_DRUM_FREQ      300
// #define DEFAULT_DRUM_PMOD      0.6
// #define DEFAULT_DRUM_LENGTH    100
// #define DEFAULT_DRUM_MIX2      0 


class Voice
{
    private:
        uint8_t type;                      // 0 - Sampler | 1 - noise | ...
        uint8_t id;
        float volume;
        bool playingState;                 // Is a sound currently playing? 

        // Audio Lib Objects
        AudioEffectEnvelope *      envelope_l;  // Envelope this voice is attached to - Left
        AudioEffectEnvelope *      envelope_r;  // Envelope this voice is attached to - right
        AudioFilterStateVariable*  filter_l;  
        AudioFilterStateVariable*  filter_r; 
        AudioEffectBitcrusher*     bitcrush_l;
        AudioEffectBitcrusher*     bitcrush_r;
        AudioSynthNoisePink*       noise;
        AudioSynthSimpleDrum*      drum;
        AudioPlaySdWav *           samplePlayer;     // Only populated if this voice is a sample player

        // Envelope / core
        uint16_t attack;                   // Current voice attack. If steps have param locks, they will be constantly updating this for their voice before playing.
        uint16_t release;                  // ^^ ditto
        uint16_t decay;                    // ^^ ditto
        uint16_t sustain;                  // ^^ ditto
        const char * sampleFilename;       // Filename for the sample to play
        uint16_t drumFreq;                 // Drum voice frequency 
        uint16_t drumLength;               // How long is the hit
        float    drumPMod;                 // Pitch mod
        float    drum2ndHitMix;            // Level of 2nd hit

        // fx n filters.. not sure if I even need these.. maybe just have on the step obj
        uint8_t  bitcrushBitdepth;
        uint16_t bitcrushSampRate;
        uint16_t filterFreq;
        float    filterQ;

        // menu related
        std::vector<char*> inputLabels_bank1;
        std::vector<char*> inputLabels_bank2;
        std::vector<char*> inputLabels_bank3;

    public:
        Voice(AudioPlaySdWav* sampPlayer, AudioEffectEnvelope* env_l, AudioEffectEnvelope* env_r);
        Voice(AudioSynthNoisePink* noise, AudioEffectEnvelope* env_l, AudioEffectEnvelope* env_r, 
              AudioFilterStateVariable* filt_l, AudioFilterStateVariable* filt_r, 
              AudioEffectBitcrusher* bitcr_l, AudioEffectBitcrusher* bitcr_r);
        Voice(AudioSynthSimpleDrum* drm, AudioEffectEnvelope* env);

        static uint8_t numVoices;                                                  // Track how many voices we have created altogether.
        static std::vector<Voice*> allVoices;                                      // Pointers to all voices. Voices get added automatically when created. DJT - may need to deal with deleting in the future.. but for now whatev
        static void assignDefaultVoicesForSequencer(Sequencer* seq);               // Logic to assign voices to each step based on number of voices. Kind of an "init" type deal.
        void play(void);     
        void setInputLabels(uint8_t bankNum, std::vector<char*> * labels);                                                      // Play the file or note. DJT - need logic to check what type of voice this is (could be a string, etc.)

        // -------- Getters --------//
        bool     isPlaying(void){return playingState;}                             // DJT - need to code out more logic to check play state... 
        uint16_t getAttack(void){return attack;}          
        uint16_t getRelease(void){return release;}
        uint8_t  getID(void){return id;}

        uint16_t getDrumFreq(void){return drumFreq;}
        float    getDrumPMod(void){return drumPMod;}
        float    getDrum2ndMix(void){return drum2ndHitMix;}
        uint16_t getDrumLength(void){return drumLength;}

        // -------- Setters --------//
        void setAttack(uint16_t atk);
        void setRelease(uint16_t rel);
        void setDecay(uint16_t dec);
        void setSustain(uint16_t sus);
        void setVolume(float vol);

        // Drum
        void setDrumFreq(uint16_t frq);
        void setDrumPMod(float pmod);
        void setDrum2ndMix(float mix2);
        void setDrumLength(uint16_t len);

        // Noise
        //void setBitcrush

        // --------- Other ---------//
        void printVoiceDebug(void);                     // Debug function that just prints out some voice values to the terminal
};

void initAudio(void);
/*
******************        Global-ish functions        *******************
*************************************************************************
*/

void setHeadphoneVolume(uint8_t val);                 // Set the headphone out volume. Takes in a value of 0-100.
void setGlobalReverbLevel(uint8_t val = -1);
void setGlobalDelayLevel(uint8_t val = -1);

// -50 to 50
// Idea is that this along with the next setting can set
// swing every X steps. May want to do per sequencer.
void setGlobalSwingAmt(int8_t val = 0);

// Use with above setting. 2 = apply swing every 2 steps, 3 = every 3 steps, etc.
void setGlobalSwingStepSpacing(int8_t val = 3);
