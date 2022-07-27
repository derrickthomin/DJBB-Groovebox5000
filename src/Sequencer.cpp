#include <string>
#include <Arduino.h>
#include "Inputs.h"
#include "Sequencer.h" 
#include "Display.h"

Step::Step(uint8_t colorSetIDX)
{
    state            = false;
    played           = false;
    volume           = 0.3;
    swingMcros       = 0;            // How much swing to apply (positive or negative to hit early or late.)
    attack           = 0;
    release          = 500;
    decay            = 500;
    sustain          = 0; 
    reverbSendLevel  = 0;  
    delaySendLevel   = 0;
    ratchetCount     = 0;
    assignedVoice    = 1;         // Track which voice to use when this hits. z
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

}
std::vector<Sequencer*> Sequencer::allSequencers = {};
int8_t Sequencer::curSequencerIDX = 0;
uint8_t Sequencer::numSequencers = 0;  

Sequencer::Sequencer(uint8_t a, uint8_t b)
{
    allSequencers.push_back(this);
    numSequencers++; 
    colorSetIDX          = 6;
    color                = get_neopix_color_by_idx(colorSetIDX,3);
    defaultStepColor     = color;
    playingState         = false;
    stepPlayed           = false;
    numSteps             = a;
    bpm                  = b;
    setBpm(b);
    startingStep         = numSteps - 1;                          // Start at "end" so we don't miss the 1st step
    currentStep          = startingStep;
    microsecondsNextStep = microsecondsPerStep;             // Track this separately... may need to apply swing and play a note early or late.    
    microsecondsNextNote = microsecondsPerStep;        
    curSequencerIDX      = 0;   
}

/*
********************************     Getters     *********************************
                
       Public functions used to get sequencer settings. They do not change state.

       Note: step functions are defined inline in the header. They mostly are
             accessed thru the sequncer functions

********************************************************************************** 
*/

uint8_t  Sequencer::  getStartingStep(void) {return startingStep;}
bool     Sequencer::  getPlayingState(void) {return playingState;}
uint8_t  Sequencer::  getLastPlayedStep(void){return lastPlayedStep;}
bool     Sequencer::  getStepStateAtIndex(uint8_t idx){return steps[idx].getStepState();}
uint8_t  Sequencer::  getCurrentStepNumber(void) {return currentStep;}
uint8_t  Sequencer::  getNumSteps(void) {return numSteps;}
bool     Sequencer::  getCurrentStepState(void){return steps[currentStep].getStepState();}
int32_t  Sequencer::  getNextStepSwing(void){return steps[getNextStepNumber()].getSwingMicros();}
int32_t  Sequencer::  getCurrentStepSwing(void){return steps[getCurrentStepNumber()].getSwingMicros();};
bool     Sequencer::  getPlayStateAtIndex(uint8_t idx){return steps[idx].getPlayingState();}
int32_t  Sequencer::  getStpSwingAtIndex(uint8_t idx){return steps[idx].getSwingMicros();}
uint32_t Sequencer::  getStepColorAtIndex(uint8_t idx){return steps[idx].getColor();}
bool     Sequencer::  getPreviousStepState(void) {return steps[getPrevStepNumber()].getStepState();}
bool     Sequencer::  getNextStepState(void){return steps[getNextStepNumber()].getStepState();}

uint8_t  Sequencer::  getPrevStepNumber(void)
{
    if (currentStep == 0) return numSteps - 1;
    return currentStep - 1;
}

uint8_t Sequencer::   getNextStepNumber(void)
{
    if (currentStep == numSteps-1) return 0;
    return currentStep + 1;
}

Sequencer* Sequencer::getCurrentSequencer(void)
{
    return Sequencer::allSequencers[Sequencer::curSequencerIDX];
}

/*
********************************     Setters     ********************************
                
        Public functions used to change sequencer settings. Changes state.

********************************************************************************** 
*/

// ----- ------- ------ STEP Functions ------ -------- --------
// These take in a value of 0 - 100, and convert it to whatever.

void Step::setAttack        (uint8_t val) {attack =  map(val, 0, 100, MIN_ATTACK, MAX_ATTACK);}
void Step::setRelease       (uint8_t val) {release = map(val, 0, 100, MIN_RELEASE, MAX_RELEASE);}
void Step::setDecay         (uint8_t val) {decay =   map(val, 0, 100, MIN_DECAY, MAX_DECAY);}
void Step::setSustain       (uint8_t val) {sustain = map(val, 0, 100, MIN_SUSTAIN, MAX_SUSTAIN);}
void Step::setRatchetCount  (uint8_t val) {ratchetCount = map(val, 0, 100, MIN_RATCHET, MAX_RATCHET);}
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


void Sequencer::setStepNumber(uint8_t step) {currentStep = step;}
void Sequencer::clearPrevPlayingState(void) {steps[getPrevStepNumber()].setPlayingState(false);}
void Sequencer::setBpm       (uint8_t newbpm)
{
    if (newbpm > MAX_BPM) newbpm = MAX_BPM;
    if (newbpm < MIN_BPM) newbpm = MIN_BPM;
    bpm = newbpm;

    // Recalculate BPM dependent FX, etc.
    microsecondsPerStep = 60 * 1000 * 1000 / (bpm * 4);  
    maxSwingMicros = (microsecondsPerStep / 2) - 400;
}

// ----- ------- ------ INDEX FUNCTIONS ------ -------- --------
void cb_seq_set_bpm                        (uint8_t val) {Sequencer::getCurrentSequencer() -> setBpm(map(val, 0, 100, MIN_BPM, MAX_BPM));}
void cb_set_attack                         (Sequencer& seq, uint8_t step_idx, uint8_t val) {seq.setStepAttackAtIndex(step_idx, val);}
void cb_set_release                        (Sequencer& seq, uint8_t step_idx, uint8_t val) {seq.setStepReleaseAtIndex(step_idx, val);}
void cb_set_decay                          (Sequencer& seq, uint8_t step_idx, uint8_t val) {seq.setStepDecayAtIndex(step_idx, val);}
void cb_set_sustain                        (Sequencer& seq, uint8_t step_idx, uint8_t val) {seq.setStepSustainAtIndex(step_idx, val);}
void cb_set_volume                         (Sequencer& seq, uint8_t step_idx, uint8_t val) {seq.setStepVolumeAtIndex(step_idx, val);}
void cb_set_ratchet                        (Sequencer& seq, uint8_t step_idx, uint8_t val) {seq.setStepRatchetCountAtIndex(step_idx, val);}
void cb_set_color                          (Sequencer& seq, uint8_t step_idx, uint8_t val) {seq.setStepColorAtIndex(step_idx, val);}
void cb_set_bitcrushDepth                  (Sequencer& seq, uint8_t step_idx, uint8_t val) {seq.setStepBitcrushDepthAtIndex(step_idx, val);}
void cb_set_bitcrushRate                   (Sequencer& seq, uint8_t step_idx, uint8_t val) {seq.setStepBitcurshRateAtIndex(step_idx, val);}
void cb_set_filterFreq                     (Sequencer& seq, uint8_t step_idx, uint8_t val) {seq.setStepFiltFreqAtIndex(step_idx, val);}
void cb_set_filterQ                        (Sequencer& seq, uint8_t step_idx, uint8_t val) {seq.setStepFilterQAtIndex(step_idx, val);}
void cb_set_drumPMod                       (Sequencer& seq, uint8_t step_idx, uint8_t val) {seq.setDrumPModAtIndex(step_idx, val);}
void cb_set_drumLen                        (Sequencer& seq, uint8_t step_idx, uint8_t val) {seq.setDrumLengthAtIndex(step_idx, val);}
void cb_set_drumMix2                       (Sequencer& seq, uint8_t step_idx, uint8_t val) {seq.setDrumMix2AtIndex(step_idx, val);}
void cb_set_drumFreq                       (Sequencer& seq, uint8_t step_idx, uint8_t val) {seq.setDrumFreqAtIndex(step_idx, val);}
void cb_set_swing                          (Sequencer& seq, uint8_t step_idx, uint8_t val) {seq.setSwingAtIndex(step_idx, val);}
void cb_set_playstate                      (Sequencer& seq, uint8_t step_idx, uint8_t val) // 1 = false, 2 = true;
{
    bool boolval;
    (val < 2) ? boolval = false : boolval = true;
    seq.setPlayStateAtIndex(step_idx, boolval);
}

void Sequencer::stepOnAtIDX                 (uint8_t idx) {steps[idx].stepOn();}
void Sequencer::stepOffAtIDX                (uint8_t idx) {steps[idx].stepOff();}
void Sequencer::stepFlipStateAtIndex        (uint8_t idx) {steps[idx].flipState();}

// Update step settings
void Sequencer::setStepAttackAtIndex        (uint8_t idx, uint8_t attack){steps[idx].setAttack(attack);}
void Sequencer::setStepReleaseAtIndex       (uint8_t idx, uint8_t release){steps[idx].setRelease(release);}
void Sequencer::setStepDecayAtIndex         (uint8_t idx, uint8_t decay){steps[idx].setDecay(decay);}
void Sequencer::setStepSustainAtIndex       (uint8_t idx, uint8_t sustain){steps[idx].setSustain(sustain);}
void Sequencer::setStepVolumeAtIndex        (uint8_t idx, uint8_t vol){steps[idx].setVolume(vol);} 
void Sequencer::setStepRatchetCountAtIndex  (uint8_t idx, uint8_t count){steps[idx].setRatchetCount(count);}
void Sequencer::setPlayStateAtIndex         (uint8_t idx, bool playstate){steps[idx].setPlayingState(playstate);}
void Sequencer::setStepColorAtIndex         (uint8_t idx, uint16_t color){steps[idx].setColor(color);}
void Sequencer::setStepBitcrushDepthAtIndex (uint8_t idx, uint8_t depth){steps[idx].setBitCrushDepth(depth);} 
void Sequencer::setStepBitcurshRateAtIndex  (uint8_t idx, uint8_t freq){steps[idx].setBitCrushRate(freq);} 
void Sequencer::setStepFiltFreqAtIndex      (uint8_t idx, uint8_t freq){steps[idx].setFiltFreq(freq);} 
void Sequencer::setStepFilterQAtIndex       (uint8_t idx, uint8_t q){steps[idx].setFilterQ(q);} 
void Sequencer::setDrumPModAtIndex          (uint8_t idx, uint8_t pmod){steps[idx].setDrumPMod(pmod);} 
void Sequencer::setDrumLengthAtIndex        (uint8_t idx, uint8_t len){steps[idx].setDrumLength(len);} 
void Sequencer::setDrumMix2AtIndex          (uint8_t idx, uint8_t mix2){steps[idx].setDrumMix2(mix2);} 
void Sequencer::setDrumFreqAtIndex          (uint8_t idx, uint8_t freq){steps[idx].setDrumFreq(freq);} 
void Sequencer::setSwingAtIndex             (uint8_t idx, uint8_t swing) 
{
    steps[idx].setSwingMicros(map(swing, 0, 100, -maxSwingMicros, maxSwingMicros));
}

/*
********************************     Core / Other     *********************************
                
              Public functions used to change sequencer settings. Changes state.

***************************************************************************************
*/

void Step::playNote(void)
{
    Serial.println("in Step::playnote");
    Serial.println(getSwingMicros());

    // Update voice settings for this step
    // djt - prob reset to default after playing??
    // djt - check to make sure the val is different before changing??

    AudioNoInterrupts(); // All Voices
    voice->setAttack(attack);
    voice->setDecay(decay);
    voice->setSustain(sustain);
    voice->setRelease(release);
    voice->setVolume(volume);

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

void Sequencer::flipPlayState(void) 
{
    playingState = !playingState;
    if (playingState) seqElapsedMicros = 0;    // If we are turning the sequencer back on, reset elapsed microseconds
}

void Sequencer::resetCounter(void){seqElapsedMicros = 0;}

// Check if we should update the next step
//     @returns: True if we go to the next step, false otherwise.
bool Sequencer::newStep(void)
{
    if (seqElapsedMicros >= microsecondsPerStep ){
        seqElapsedMicros = seqElapsedMicros - microsecondsPerStep;
        currentStep = getNextStepNumber();
        return true;
    } 
    return false;
}

// Check if it's time to play a new note. Not necessarily when a new step hits
//     Returns: True if it's time to play a note
bool Sequencer::newNote(void)
{
    if ((seqElapsedMicros > microsecondsNextNote)){
        bool current_step_state = getCurrentStepState();
        bool next_step_state = getNextStepState();
        if (current_step_state && !getCurrentStepPlayedState() && getCurrentStepSwing() >= 0){   // Current step active, and hasn't played yet
            setCurrentStepPlayedState(true);
            lastPlayedStep = currentStep;
            return true;
        }
        if (next_step_state && !getNextStepPlayedState() && getNextStepSwing() < 0){           // Next step active, hits early, and hasn't been played yet.
            setNextStepPlayedState(true);
            lastPlayedStep = getNextStepNumber();
            return true;
        }
    }
    return false;
}

void Sequencer::initializeSteps(void)
{
    for (uint8_t i = 0; i < numSteps; i++) 
    {
       steps.push_back(Step(Sequencer::colorSetIDX));
    }
}

void Sequencer::calcNextNoteTime(bool midStepCalc = false)
{
    if (!midStepCalc){                                         // Calculating RIGHT after incrementing the step. 
        if (getCurrentStepSwing() >= 0){
            microsecondsNextNote = getCurrentStepSwing();
            return;
        }}

    if (midStepCalc){                                          // Calculating right AFTER playng a new note.
        if (!getNextStepState()){                              // Next step isn't even on.. punt
            microsecondsNextNote = microsecondsPerStep * 2; 
            return;
        }         
        if (getNextStepSwing() >= 0){                           // Just played current step note, next step note should not be played until after the next step.
            microsecondsNextNote = microsecondsPerStep * 2;                  
            return;
        }
        bool already_played_next = (lastPlayedStep == getNextStepNumber());
        bool already_played_current = (lastPlayedStep == getCurrentStepNumber());
        if (already_played_next){
            microsecondsNextNote = microsecondsPerStep * 2;
            return;
        }

        if (already_played_current && getNextStepSwing() >=0){   // Already played the note & next step hits late or on time., Punt
            microsecondsNextNote = microsecondsPerStep * 2;
            return;
        }
    }
    microsecondsNextNote = microsecondsPerStep + (getNextStepSwing());    // Default state.. simply add the swing offset 
}

void Sequencer::reset(void){
    for (uint8_t i = 0; i < numSteps; i++){
        (getStepStateAtIndex(i)) ? strip.setPixelColor(i, getStepColorAtIndex(i)) : strip.setPixelColor(i, 0);
    }
    currentStep = startingStep;
}

void Sequencer::resetSwing(void)
{
    for (uint8_t i = 0; i < numSteps; i++){
        setSwingAtIndex(i, 0);
    }
}
 //**************     Globaslish
// DJT experimnetal...
 void setCurrSeqSwingAtIndex(uint8_t step_idx, uint8_t pot_idx)
 {
    int8_t swingval;
    switch(pot_idx){
        case 0:
            swingval = knob1Val();
            break;
        case 1:
            swingval = knob2Val();
            break;
        case 2:
            swingval = knob3Val();
            break;
        case 3:
            swingval = knob4Val();
            break;
        case 4:
            swingval = sliderAVal();
            break;
        case 5:
            swingval = sliderBVal();
            break;
    }
    swingval = map(swingval, 0, 100, -50, 50);
    Sequencer::allSequencers[Sequencer::curSequencerIDX]->setSwingAtIndex(step_idx, swingval);
 }
/*
**********************************     DEBUG     *********************************
                
                               Just for testing n stuff

********************************************************************************** 
*/
void Sequencer::debugPrintSeqData(String message, bool extraSpace = false, bool detailed = true)
{
    if (extraSpace)
    {
       Serial.println("");
       Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
       Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
       Serial.println(""); 
    }

    Serial.println("");
    Serial.println("----------  \t" + message + "\t  ----------");
    Serial.println("");
    Serial.println("microseconds per step: \t" + String(microsecondsPerStep));
    Serial.println("microseconds Next Note: \t" + String(microsecondsNextNote));
    Serial.println("elapsed micros \t" + String(seqElapsedMicros));
    Serial.println("");
    Serial.println("      [ Step Level Info ]     ");
    Serial.println("");
    Serial.println("current Step: \t" + String(getCurrentStepNumber()));
    Serial.println("Last played step \t" + String(lastPlayedStep));
    Serial.println("current Step State: \t" + String(getCurrentStepState()));
    Serial.println("Previous Step State: \t" + String(getPreviousStepState()));
    Serial.println("Next Step State: \t" + String(getNextStepState()));
    Serial.println("Current Step Swing: \t" + String(getCurrentStepSwing()));
    Serial.println("Next Step Swing: \t" + String(getNextStepSwing()));
}