# DJBB-Groovebox5000

## High level 

* Sequencer object -> vector of step objects -> Each step points towards single step object

## Current Demo / Pics
todo

## Objects and how they interact
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

## How timing works
## Roadmap
### Up Next

### Completed
none

### Brain dump
Everything I can think of, realistic or not. Can pull from this into up next.

- Global FX
- Per step fx sends
- Jumps (specify a jump from step X to Y ever Z sequence plays)
- Copy + paste full steps
- Copy + paste step params
- Propagate step params
- - Ex: propagate swing value to every 3rd step
- Global swing
- Voice swaps (ability to change voice for a step)
- Voice swap intervals (swap voice every X loops)
- Random swaps (randomly swap params between steps)
- Sequence shift (shift the whole thing forward / back)
- Sequence shift interval (shift only every X loops)
- Multiple sequences
- More than 16 steps (low priority.. trying to get variation via intervals to be "uNiQuE"
- Voice layering
- Undo (super low priority because it sounds very hard)
- Record to SD card
- Multiple "parts" - chaining sequencers
- Preset saving (low priority... 2 hard)
- Add voice types (string, sine wave, etc.)
- Ability to swap files in sample voice (list of SD card files, select)
- Add RAW audio files of some small primatives (little clicks, transients, other small samples)
- - Raw as in just an H file with the data ary
- LFOs
- Variable speed sample playback
- Reverse mode
- Additional sequencer types (euclidian, etc.)
- Mic record in to step (record to SD, assign to sample voice for current step)
- Param reset support (some way to clear a param / go back to default)
-  - Also a way to clear ALL params from a step
