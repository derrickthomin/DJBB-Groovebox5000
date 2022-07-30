#include <string>
#include <Arduino.h>
#include "Inputs.h"
#include "Display.h"

Step::Step(uint8_t colorSetIDX, Sequencer* seq)
{     
    parentSeq        = seq;
    state            = false;
    played           = false;
    volume           = 0.3;
    swingMcros       = 0;            // How much swing to apply (positive or negative to hit early or late.)
    attack           = 0;
    release          = 100;
    decay            = 100;
    sustain          = 0; 
    reverbSendLevel  = 0;  
    delaySendLevel   = 0;
    ratchetCount     = 0;
    assignedVoice    = 1;             // Track which voice to use when this hits. z
    colorSetIDXstp   = colorSetIDX;
    color            = get_neopix_color_by_idx(colorSetIDX, 1);  // Same color as sequencer, but darker

    // FX n Filters
    bitcrushDepth    = DEFAULT_CRUSH_BITDEPTH;        // Bitcrush dept 1-16
    bitcrushSampRate = DEFAULT_CRUSH_FREQ;            // Up to 44.1k
    filterFreq       = DEFAULT_FILT_FREQ;             // 0 - 20k
    filterQ          = DEFAULT_FILT_Q;                // 0.7 - 5. Above 0.707 will add gain.
    filter_LP_amt    = 1.0;                           // 1.0 = passthru, less = attenuate
    filter_HP_amt    = 1.0;                           // 

    // Drum
    drumFreq         = DEFAULT_DRUM_FREQ;             // Drum voice frequency 
    drumLength       = DEFAULT_DRUM_LENGTH;           // How long is the hit
    drumPMod         = DEFAULT_DRUM_PMOD;             // Pitch mod
    drum2ndHitMix    = DEFAULT_DRUM_MIX2;             // Level of 2nd hit

    // DJT TESSSTINGG
    release_mod_vals.push_back(10);
    release_mod_interval.push_back(2);     // every 2 steps
    volume_mod_vals.push_back(0.2);
    volume_mod_interval.push_back(3);     // every 2 steps

}

/*
**********************    Core     ************************
                

***********************************************************
*/
void Step::playNote(void)
{
    Serial.println("in Step::playnote");
    Serial.println(getSwingMicros());

//  ----- Deal with overrides ------  //
    uint16_t loopCount  = parentSeq -> loopCount;
    uint16_t decay_tmp  = decay;                // Initialize some vars 
    float    volume_tmp = volume;               // Initialize some vars 
    
    // Release Overrides
    if (release_mod_interval.size() && loopCount > 1)
    {
        for (uint8_t i = 0; i < release_mod_interval.size(); i++)
        {
            if (!(loopCount % release_mod_interval[i]))
            {
                decay_tmp = release_mod_vals[i];
            }
        }
    }

    // Volume Overrides
    if (volume_mod_interval.size() && loopCount > 1)
    {
        for (uint8_t i = 0; i < volume_mod_interval.size(); i++)
        {
            if (!(loopCount % volume_mod_interval[i]))
            {
                volume_tmp = volume_mod_vals[i];
            }
        }
    }

    AudioNoInterrupts(); // All Voices
    voice->setAttack(attack);
    voice->setDecay(decay_tmp);
    voice->setSustain(sustain);
    voice->setRelease(decay_tmp);
    voice->setVolume(volume_tmp);

    switch (voice->getType())
    {
    case 0:  // Sampler
        break;

    case 1:  // Noise
        break;

    case 2:  // Drum Synth
        voice->setDrumFreq(drumFreq);
        voice->setDrumPMod(drumPMod);
        voice->setDrumLength(drumLength);
        voice->setDrum2ndMix(drum2ndHitMix);
        break;

    }
    AudioInterrupts();
    voice -> play();
    voice -> printVoiceDebug();
}

/*
********************************     Setters     ********************************
                
        Public functions used to change sequencer settings. Changes state.
        These take in a value of 0 - 100, and convert it to whatever.

********************************************************************************** 
*/

void Step::setAttack        (uint8_t val) {attack =  map(val, 0, 100, MIN_ATTACK, MAX_ATTACK);}
void Step::setRelease       (uint8_t val) {release = map(val, 0, 100, MIN_RELEASE, MAX_RELEASE);}
void Step::setDecay         (uint8_t val) {decay =   map(val, 0, 100, MIN_DECAY, MAX_DECAY);}
void Step::setSustain       (uint8_t val) {sustain = map(val, 0, 100, MIN_SUSTAIN, MAX_SUSTAIN);}
void Step::setRatchetCount  (uint8_t val) 
{
    ratchetCount = map(val, 0, 100, MIN_RATCHET, MAX_RATCHET);
}

void Step::setVolume        (uint8_t val) 
{
    volume = float(map(float(val),0.0,100.0,MIN_VOICE_VOL,MAX_VOICE_VOL));
    Serial.println("in step::setvolume");
    Serial.println(volume);

}
// drum voice
void Step::setDrumPMod      (uint8_t val) 
{
    drumPMod = float(map(val, 0, 100, MIN_DRUM_PMOD*10, MAX_DRUM_PMOD*10))/10.0; 
    Serial.println("here");
        Serial.println(drumPMod);
}
void Step::setDrumLength    (uint8_t val) {drumLength = map(val, 0,100, MIN_DRUM_LEN, MAX_DRUM_LEN);}
void Step::setDrumMix2      (uint8_t val) {drum2ndHitMix = map(float(val), 0.0, 100.0, MIN_DRUM_MIX2, MAX_DRUM_MIX2);}
void Step::setDrumFreq      (uint8_t val) {drumFreq = map(val, 0, 100, MIN_DRUM_FREQUENCY, MAX_DRUM_FREQUENCY);}
// fx n filter 
void Step::setBitCrushDepth (uint8_t val) {bitcrushDepth = map(val, 0, 100, MIN_CRUSH_BITDEPTH, MAX_CRUSH_BITDEPTH);}
void Step::setBitCrushRate  (uint8_t val) {bitcrushSampRate = map(val, 0, 100, MIN_CRUSH_FREQ, MAX_CRUSH_FREQ);}
void Step::setFiltFreq      (uint8_t val) {filterFreq = map(val, 0, 100, MIN_FILTER_FREQ, MAX_FILTER_FREQ);}
void Step::setFilterQ       (uint8_t val) {filterQ = map(val, 0, 100, MIN_FILTER_Q, MAX_FILTER_Q);}
// 
void Step::setSwingMicros   (int32_t swingMicr)     {swingMcros = swingMicr;}
void Step::setPlayingState  (bool playstate)        {played = playstate;} 