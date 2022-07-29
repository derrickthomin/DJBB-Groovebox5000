#include <string>
#include <Arduino.h>
#include "Inputs.h"
#include "Sequencer.h" 
#include "Display.h"
#include "Step.h"

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
    startStepIDX         = numSteps - 1;                          // Start at "end" so we don't miss the 1st step
    currentStepIDX          = startStepIDX;
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

uint8_t  Sequencer::  getstartStepIDX(void) {return startStepIDX;}
bool     Sequencer::  getPlayingState(void) {return playingState;}
uint8_t  Sequencer::  getlastPlayedStepIDX(void){return lastPlayedStepIDX;}
bool     Sequencer::  getStepStateAtIndex(uint8_t idx){return steps[idx].getStepState();}
uint8_t  Sequencer::  getCurrentStepIDX(void) {return currentStepIDX;}
uint8_t  Sequencer::  getNumSteps(void) {return numSteps;}
bool     Sequencer::  getcurrentStepIDXState(void){return steps[currentStepIDX].getStepState();}
int32_t  Sequencer::  getNextStepSwing(void){return steps[getNextStepNumber()].getSwingMicros();}
int32_t  Sequencer::  getcurrentStepIDXSwing(void){return steps[getCurrentStepIDX()].getSwingMicros();};
bool     Sequencer::  getPlayStateAtIndex(uint8_t idx){return steps[idx].getPlayingState();}
int32_t  Sequencer::  getStpSwingAtIndex(uint8_t idx){return steps[idx].getSwingMicros();}
uint32_t Sequencer::  getStepColorAtIndex(uint8_t idx){return steps[idx].getColor();}
bool     Sequencer::  getPreviousStepState(void) {return steps[getPrevStepIDX()].getStepState();}
bool     Sequencer::  getNextStepState(void){return steps[getNextStepNumber()].getStepState();}

uint8_t  Sequencer::  getPrevStepIDX(void)
{
    if (currentStepIDX == 0) return numSteps - 1;
    return currentStepIDX - 1;
}

uint8_t Sequencer::   getNextStepNumber(void)
{
    if (currentStepIDX == numSteps-1) return 0;
    return currentStepIDX + 1;
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

void Sequencer::setStepNumber(uint8_t step) {currentStepIDX = step;}
void Sequencer::clearPrevPlayingState(void) {steps[getPrevStepIDX()].setPlayingState(false);}
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
        currentStepIDX = getNextStepNumber();
        return true;
    } 
    return false;
}

// Check if it's time to play a new note. Not necessarily when a new step hits
//     Returns: True if it's time to play a note
bool Sequencer::newNote(void)
{
    if ((seqElapsedMicros > microsecondsNextNote)){
        bool current_step_state = getcurrentStepIDXState();
        bool next_step_state = getNextStepState();
        if (current_step_state && !getcurrentStepIDXPlayedState() && getcurrentStepIDXSwing() >= 0){   // Current step active, and hasn't played yet. Either on time or late.
            setcurrentStepIDXPlayedState(true);
            lastPlayedStepIDX = currentStepIDX;
            return true;
        }
        if (next_step_state && !getNextStepPlayedState() && getNextStepSwing() < 0){                   // Next step active, hits early, and hasn't been played yet.
            setNextStepPlayedState(true);
            lastPlayedStepIDX = getNextStepNumber();
            return true;
        }
    }
    // Now deal with ratchet / bonus hits
    if (bonusHits.size() > 0 && (seqElapsedMicros > bonusHits.front()))
    {
        bonusHits.pop();
        return true;
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
    bool already_played_next = (lastPlayedStepIDX == getNextStepNumber());
    bool already_played_current = (lastPlayedStepIDX == getCurrentStepIDX());

    if (!midStepCalc){                                         // Calculating RIGHT after incrementing the step. 

        if (getcurrentStepIDXSwing() >= 0){
            microsecondsNextNote = getcurrentStepIDXSwing();
            return;
        }}

    if (midStepCalc){                                           // Calculating right AFTER playng a new note.                
        uint8_t ratchetCount = steps[lastPlayedStepIDX].getRatchetCount();                      
        if (ratchetCount > 0)    // First - deal with ratchets. Add to the ratchet micros vector if we need to.
        {   
            unsigned long ratchet_micros = microsecondsPerStep / ratchetCount;
            for (uint8_t i = 1; i < ratchetCount + 1; i++)
            {
                unsigned long next_ratchet_micros = seqElapsedMicros + ratchet_micros * i;
                if (next_ratchet_micros < microsecondsPerStep) bonusHits.push(next_ratchet_micros); // Don't go past next step.
            }
        }
        
        if (!getNextStepState()){                              // Next step isn't even on.. punt
            microsecondsNextNote = microsecondsPerStep * 2; 
            return;
        }         
        if (getNextStepSwing() >= 0){                           // Just played current step note, next step note should not be played until after the next step.
            microsecondsNextNote = microsecondsPerStep * 2;                  
            return;
        }

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
    currentStepIDX = startStepIDX;
    draw_cur_seq_oled();
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
    Serial.println("current Step: \t" + String(getCurrentStepIDX()));
    Serial.println("Last played step \t" + String(lastPlayedStepIDX));
    Serial.println("current Step State: \t" + String(getcurrentStepIDXState()));
    Serial.println("Previous Step State: \t" + String(getPreviousStepState()));
    Serial.println("Next Step State: \t" + String(getNextStepState()));
    Serial.println("Current Step Swing: \t" + String(getcurrentStepIDXSwing()));
    Serial.println("Next Step Swing: \t" + String(getNextStepSwing()));
}