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
uint8_t Sequencer::getStartingStep(void) {return startingStep;}
bool    Sequencer::getPlayingState(void) {return playingState;}
uint8_t Sequencer::getLastPlayedStep(void){return lastPlayedStep;}
bool    Sequencer::getStepStateAtIndex(uint8_t idx){return steps[idx].getStepState();}
uint8_t Sequencer::getCurrentStepNumber(void) {return currentStep;}
uint8_t Sequencer::getNumSteps(void) {return numSteps;}
bool    Sequencer::getCurrentStepState(void){return steps[currentStep].getStepState();}
int32_t Sequencer::getNextStepSwing(void){return steps[getNextStepNumber()].getSwingMicros();}
int32_t Sequencer::getCurrentStepSwing(void){return steps[getCurrentStepNumber()].getSwingMicros();};
bool    Sequencer::getPlayStateAtIndex(uint8_t idx){return steps[idx].getPlayingState();}
int32_t Sequencer::getStpSwingAtIndex(uint8_t idx){return steps[idx].getSwingMicros();}
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
void Step::setAttack(uint16_t envAttack)
{
    attack = envAttack;
    //voice->setAttack(envAttack);
}

void Step::setRelease(uint16_t envRelease)
{
    release = envRelease;
    //voice->setRelease(release);
}

void Step::setDecay(uint16_t envDecay)
{
    decay = envDecay;
    //voice->setDecay(decay);
}

void Step::setSustain(uint16_t envSustain)
{
    sustain = envSustain;
    //voice->setSustain(sustain);
}

void Step::playNote(void)
{
    Serial.println("in Step::playnote");
    Serial.println(getSwingMicros());

    // Update voice settings for this step
    // djt - prob reset to default after playing??
    AudioNoInterrupts();
    voice->setAttack(attack);
    voice->setDecay(decay);
    voice->setRelease(sustain);
    voice->setRelease(release);
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

void Sequencer::stepOnAtIndex(uint8_t idx){steps[idx].stepOn();}
void Sequencer::stepOffAtIndex(uint8_t idx){steps[idx].stepOff();}
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

void Sequencer::reset(void)
{
    for (uint8_t i = 0; i < 16; i++)
    {
        (getStepStateAtIndex(i)) ? strip.setPixelColor(i, getDefaultStepColor()) : strip.setPixelColor(i, 0);
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

void Sequencer::calcNextNoteTime(bool midStepCalc = false)
{
    if (!midStepCalc)   // Calculating RIGHT after incrementing the step
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

void Sequencer::clearPrevPlayingState(void) {steps[getPrevStepNumber()].setPlayingState(false);}
 
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