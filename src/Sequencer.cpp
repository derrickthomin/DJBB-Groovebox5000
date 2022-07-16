#include <string>
#include <Arduino.h>
#include "Inputs.h"
#include "Sequencer.h" 
#include "Display.h"

Step::Step(uint8_t colorSetIDX)
{
    state = false;
    played = false;
    volume = 0.3;
    swingMcros = 0;            // How much swing to apply (positive or negative to hit early or late.)
    attack = 0;
    release = 500;
    decay = 500;
    sustain = 0; 
    reverbSendLevel = 0;  
    delaySendLevel = 0;
    ratchetCount = 0;
    assignedVoice = 1;         // Track which voice to use when this hits. z
    colorSetIDXstp = colorSetIDX;
    color = get_neopix_color_by_idx(colorSetIDX, 1);  // Same color as sequencer, but darker
    // FX n Filters
    bitcrushDepth =    DEFAULT_CRUSH_BITDEPTH;        // Bitcrush dept 1-16
    bitcrushSampRate = DEFAULT_CRUSH_FREQ;            // Up to 44.1k
    filterFreq =       DEFAULT_FILT_FREQ;             // 0 - 20k
    filterQ =          DEFAULT_FILT_Q;                // 0.7 - 5. Above 0.707 will add gain.
    // Drum
    drumFreq = DEFAULT_DRUM_FREQ;                     // Drum voice frequency 
    drumLength = DEFAULT_DRUM_LENGTH;                 // How long is the hit
    drumPMod = DEFAULT_DRUM_PMOD;                     // Pitch mod
    drum2ndHitMix = DEFAULT_DRUM_MIX2;                // Level of 2nd hit

}
Sequencer::Sequencer(uint8_t a, uint8_t b)
{
    colorSetIDX = 6;
    color = get_neopix_color_by_idx(colorSetIDX,3);
    defaultStepColor = color;
    playingState = false;
    stepPlayed = false;
    numSteps = a;
    bpm = b;
    startingStep = numSteps - 1;                          // Start at "end" so we don't miss the 1st step
    currentStep = startingStep;
    microsecondsPerStep = 60 * 1000 * 1000 / (bpm * 4);    // 4 = 16th notes. 1 = 1/4 notes. Etc.
    maxSwingMicros = (microsecondsPerStep / 2) - 400;     // Set max swing to a little less than half of a step 
    microsecondsNextStep = microsecondsPerStep;            // Track this separately... may need to apply swing and play a note early or late.    
    microsecondsNextNote = microsecondsPerStep;              
}

/*
********************************     Getters     *********************************
                
       Public functions used to get sequencer settings. They do not change state.

       Note: step functions are defined inline in the header. They mostly are
             accessed thru the sequncer functions

********************************************************************************** 
*/

uint8_t   Sequencer::getStartingStep(void) {return startingStep;}
bool      Sequencer::getPlayingState(void) {return playingState;}
uint8_t   Sequencer::getLastPlayedStep(void){return lastPlayedStep;}
bool      Sequencer::getStepStateAtIndex(uint8_t idx){return steps[idx].getStepState();}
uint8_t   Sequencer::getCurrentStepNumber(void) {return currentStep;}
uint8_t   Sequencer::getNumSteps(void) {return numSteps;}
bool      Sequencer::getCurrentStepState(void){return steps[currentStep].getStepState();}
int32_t   Sequencer::getNextStepSwing(void){return steps[getNextStepNumber()].getSwingMicros();}
int32_t   Sequencer::getCurrentStepSwing(void){return steps[getCurrentStepNumber()].getSwingMicros();};
bool      Sequencer::getPlayStateAtIndex(uint8_t idx){return steps[idx].getPlayingState();}
int32_t   Sequencer::getStpSwingAtIndex(uint8_t idx){return steps[idx].getSwingMicros();}
uint32_t  Sequencer::getStepColorAtIndex(uint8_t idx){return steps[idx].getColor();}

bool Sequencer::getPreviousStepState(void)
{
    return steps[getPrevStepNumber()].getStepState();
}

bool Sequencer::getNextStepState(void)
{
    return steps[getNextStepNumber()].getStepState();
}

uint8_t Sequencer::getPrevStepNumber(void)
{
    if (currentStep == 0) return numSteps - 1;
    return currentStep - 1;
}

uint8_t Sequencer::getNextStepNumber(void)
{
    if (currentStep == numSteps-1) return 0;
    return currentStep + 1;
}

/*
********************************     Setters     ********************************
                
        Public functions used to change sequencer settings. Changes state.

********************************************************************************** 
*/
void Step::setAttack(uint16_t envAttack) {attack = envAttack;}
void Step::setRelease(uint16_t envRelease){release = envRelease;}
void Step::setDecay(uint16_t envDecay){decay = envDecay;}
void Step::setSustain(uint16_t envSustain){sustain = envSustain;}

void Step::playNote(void)
{
    Serial.println("in Step::playnote");
    Serial.println(getSwingMicros());

    // Update voice settings for this step
    // djt - prob reset to default after playing??
    AudioNoInterrupts();
    voice->setAttack(attack);
    voice->setDecay(decay);
    voice->setSustain(sustain);
    voice->setRelease(release);
    voice->setDrumFreq(drumFreq);
    voice->setDrumPMod(drumPMod);
    AudioInterrupts();

    voice->play();
    voice -> printVoiceDebug();
}

void Sequencer::flipPlayState(void) 
{
    playingState = !playingState;
    if (playingState) seqElapsedMicros = 0;    // If we are turning the sequencer back on, reset elapsed microseconds
}
void Sequencer::resetCounter(void){seqElapsedMicros = 0;}
void Sequencer::setStepNumber(uint8_t step) {currentStep = step;}

// ----- ------- ------ INDEX FUNCTIONS ------ -------- --------
void Sequencer::stepOnAtIDX(uint8_t idx){steps[idx].stepOn();}
void Sequencer::stepOffAtIDX(uint8_t idx){steps[idx].stepOff();}
void Sequencer::stepFlipStateAtIndex(uint8_t idx){steps[idx].flipState();}

void Sequencer::setStepAttackAtIndex(uint8_t idx, uint16_t attack){steps[idx].setAttack(attack);}
void Sequencer::setStepReleaseAtIndex(uint8_t idx, uint16_t release){steps[idx].setRelease(release);}
void Sequencer::setStepDecayAtIndex(uint8_t idx, uint16_t decay){steps[idx].setDecay(decay);}
void Sequencer::setStepSustainAtIndex(uint8_t idx, uint16_t sustain){steps[idx].setSustain(sustain);}

void Sequencer::setStepVolumeAtIndex(uint8_t idx, uint16_t vol){steps[idx].setVolume(vol);}
void Sequencer::setStepSwingMillisAtIndex(uint8_t idx, int16_t swingMillis){steps[idx].setSwingMillis(swingMillis);}          
void Sequencer::setStepRatchetCountAtIndex(uint8_t idx, uint8_t count){steps[idx].setRatchetCount(count);}
void Sequencer::setPlayStateAtIndex(uint8_t idx, bool playstate){steps[idx].setPlayingState(playstate);}
void Sequencer::setStepColorAtIndex(uint8_t idx, uint16_t color){steps[idx].setColor(color);}
void Sequencer::clearPrevPlayingState(void) {steps[getPrevStepNumber()].setPlayingState(false);}

void Sequencer::setStepBitcrushDepthAtIndex(uint8_t idx, uint8_t depth){steps[idx].setBitCrushDepth(depth);} 
void Sequencer::setStepBitcurshRateAtIndex(uint8_t idx, uint16_t freq){steps[idx].setBitCrushRate(freq);} 
void Sequencer::setStepFiltFreqAtIndex(uint8_t idx, uint16_t freq){steps[idx].setFiltFreq(freq);} 
void Sequencer::setStepFilterQAtIndex(uint8_t idx, float q){steps[idx].setFilterQ(q);} 
void Sequencer::setDrumPModAtIndex(uint8_t idx, float pmod){steps[idx].setDrumPMod(pmod);} 
void Sequencer::setDrumLengthAtIndex(uint8_t idx, uint16_t len){steps[idx].setDrumLength(len);} 
void Sequencer::setDrumMix2AtIndex(uint8_t idx, float mix2){steps[idx].setDrumMix2(mix2);} 
void Sequencer::setDrumFreqAtIndex(uint8_t idx, uint16_t freq){steps[idx].setDrumFreq(freq);} 


void Sequencer::reset(void)
{
    for (uint8_t i = 0; i < numSteps; i++)
    {
        (getStepStateAtIndex(i)) ? strip.setPixelColor(i, getStepColorAtIndex(i)) : strip.setPixelColor(i, 0);
    }
    currentStep = startingStep;
}

void Sequencer::resetSwing(void)
{
    for (uint8_t i = 0; i < numSteps; i++)
    {
        setSwingAtIndex(i, 0);
    }
}

void Sequencer::setSwingAtIndex(uint8_t idx, int8_t swing) 
{
    if (swing < -50) swing = -50;
    if (swing > 50) swing = 50;
    steps[idx].setSwingMicros(map(swing, -50, 50, -maxSwingMicros, maxSwingMicros));
}

void Sequencer::setBpm(uint16_t newbpm) 
{
    // todo - check against max and min bpm
    // todo - Set new microsends per step values
    // todo - Set new max swing value
    // todo - recalc next note time? Or maybe not?
}

/*
********************************     Core / Other     *********************************
                
              Public functions used to change sequencer settings. Changes state.

***************************************************************************************
*/
// Check if we should update the next step
//     @returns: True if we go to the next step, false otherwise.
bool Sequencer::newStep(void)
{
    if (seqElapsedMicros >= microsecondsPerStep )
    {
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
    if ((seqElapsedMicros > microsecondsNextNote))
    {
        bool current_step_state = getCurrentStepState();
        bool next_step_state = getNextStepState();

        if (current_step_state && !getCurrentStepPlayedState() && getCurrentStepSwing() >= 0)   // Current step active, and hasn't played yet
        {
            setCurrentStepPlayedState(true);
            lastPlayedStep = currentStep;
            return true;
        }

        if (next_step_state && !getNextStepPlayedState() && getNextStepSwing() < 0)            // Next step active, hits early, and hasn't been played yet.
        {
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

/*
    This one was tricky to figure out so documenting... the basic logic is:
        1. 

*/

void Sequencer::calcNextNoteTime(bool midStepCalc = false)
{
    if (!midStepCalc)   // Calculating RIGHT after incrementing the step. 
    {
        if (getCurrentStepSwing() >= 0)
        {
            microsecondsNextNote = getCurrentStepSwing(); 
            return;  
        }
    }
    if (midStepCalc)  // Calculating right AFTER playng a new note.
    {
        if (!getNextStepState())   // Next step isn't even on.. punt
        { 
            microsecondsNextNote = microsecondsPerStep * 2; 
            return;          
        }
        if (getNextStepSwing() >= 0) // Just played current step note, next step note should not be played until after the next step.
        {
            microsecondsNextNote = microsecondsPerStep * 2;                  
            return;
        }

        bool already_played_next = (lastPlayedStep == getNextStepNumber());
        bool already_played_current = (lastPlayedStep == getCurrentStepNumber());

        if (already_played_next)
        {
            microsecondsNextNote = microsecondsPerStep * 2;
            return;
        }

        if (already_played_current && getNextStepSwing() >=0)   // Already played the note & next step hits late or on time., Punt
        {
            microsecondsNextNote = microsecondsPerStep * 2;
            return;
        }
    }
    microsecondsNextNote = microsecondsPerStep + (getNextStepSwing());    // Default state.. simply add the swing offset 

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