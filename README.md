# DJBB-Groovebox5000
Little groovebox that I'm still working on. The plan is to focus on features that increse the rythmic fun (as opposed to sound design-y features.. so think timing features, probability, swing, etc.). I'm slowly realizing that it's not the sounds - quality OR timbre - that really make a track, it's the rythm, so we shall focus on this.

## Demos
https://www.youtube.com/shorts/VS3Zzbqb1rc

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
todo

## Roadmap
### Up Next
todo

### Completed
todo

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
