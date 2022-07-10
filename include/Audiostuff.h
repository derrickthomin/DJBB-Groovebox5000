#pragma once
#include <Audio.h>
#include <vector>
#include "Common.h"

#define STARTUP_NUM_VOICES 4    // How many voices to created

class Voice
{
    private:
        uint8_t id;
        float volume;
        bool playingState;                 // Is a sound currently playing? 
        AudioEffectEnvelope * envelope_l;  // Envelope this voice is attached to - Left
        AudioEffectEnvelope * envelope_r;  // Envelope this voice is attached to - right
        AudioPlaySdWav * samplePlayer;     // Only populated if this voice is a sample player
        uint16_t attack;                   // Current voice attack. If steps have param locks, they will be constantly updating this for their voice before playing.
        uint16_t release;                  // ^^ ditto
        uint16_t decay;                    // ^^ ditto
        uint16_t sustain;                  // ^^ ditto
        const char * sampleFilename;       // Filename for the sample to play

    public:
        Voice(AudioPlaySdWav* sampPlayer, AudioEffectEnvelope* env_l, AudioEffectEnvelope* env_r);
        static uint8_t numVoices;                                                  // Track how many voices we have created altogether.
        static std::vector<Voice*> allVoices;                                      // Pointers to all voices. Voices get added automatically when created. DJT - may need to deal with deleting in the future.. but for now whatev
        static void assignDefaultVoicesForSequencer(Sequencer* seq);               // Logic to assign voices to each step based on number of voices. Kind of an "init" type deal.
        void play(void);                                                           // Play the file or note. DJT - need logic to check what type of voice this is (could be a string, etc.)

        // -------- Getters --------//
        bool isPlaying(void){return playingState;}                                 // DJT - need to code out more logic to check play state... 
        uint16_t getAttack(void){return attack;}          
        uint16_t getRelease(void){return release;}
        uint8_t  getID(void){return id;}

        // -------- Setters --------//
        void setAttack(uint16_t atk);
        void setRelease(uint16_t rel);
        void setDecay(uint16_t dec);
        void setSustain(uint16_t sus);
        void setVolume(float vol){volume = vol;}

        // --------- Other ---------//
        void printVoiceDebug(void);                     // Debug function that just prints out some voice values to the terminal
};

void initAudio(void);
void setHeadphoneVolume(uint8_t potval);                 // Set the headphone out volume. Takes in a value of 0-100.


// TESDTING

extern AudioEffectEnvelope envelope_master_L;