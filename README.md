# DJBB-Groovebox5000

## High level design

* Sequencer object -> vector of step objects -> Each step ponits towards single step object

### Sequencer
* House higher level settings, such as BPM, number of steps, color used to represent, etc.
* This is the main object to interact with. It has a bunch of index functions
* * For example, setStepAttackAtIndex(idx, atk)

### Step
* Houses step specific settings (think: p-locks)
* * Ex: attack time, decay time, swing time, filters, etc..

Function of note here: playNote()
This takes care of looking at step settings, and then applying them to the attached voice.
Then, the voice play() function is called. 

### Voice
* Houses voice specific settings (is this a synth drum voice, sample voice, noise, etc.)
* These settings are updated by the steps as they play

## Creating new voices
todo

## Creating new screens
todo

## Roadmap
todo
