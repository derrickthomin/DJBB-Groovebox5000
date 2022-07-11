#include <Audio.h>
#include <Arduino.h>
#include <SerialFlash.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <vector>
#include "Audiostuff.h"
#include "Sequencer.h"

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav2; //xy=141,663
AudioPlaySdWav           playSdWav4; //xy=142,1048
AudioPlaySdWav           playSdWav1;     //xy=146,435
AudioPlaySdWav           playSdWav3; //xy=146,869
AudioEffectEnvelope      envelope2_R; //xy=378,709
AudioEffectEnvelope      envelope4_R; //xy=379,1094
AudioEffectEnvelope      envelope2_L; //xy=381,634
AudioEffectEnvelope      envelope1_R;    //xy=383,481
AudioEffectEnvelope      envelope4_L; //xy=382,1018
AudioEffectEnvelope      envelope3_R; //xy=383,915
AudioEffectEnvelope      envelope1_L;    //xy=386,405
AudioEffectEnvelope      envelope3_L; //xy=386,839
AudioMixer4              Mixer_Master_L; //xy=688.8888854980469,696.3333511352539
AudioMixer4              Mixer_Master_R; //xy=692.8888854980469,833.3333511352539
AudioOutputI2S           i2s1;           //xy=953.8888854980469,764.3333511352539
AudioConnection          patchCord1(playSdWav2, 0, envelope2_L, 0);
AudioConnection          patchCord2(playSdWav2, 1, envelope2_R, 0);
AudioConnection          patchCord3(playSdWav4, 0, envelope4_L, 0);
AudioConnection          patchCord4(playSdWav4, 1, envelope4_R, 0);
AudioConnection          patchCord5(playSdWav1, 0, envelope1_L, 0);
AudioConnection          patchCord6(playSdWav1, 1, envelope1_R, 0);
AudioConnection          patchCord7(playSdWav3, 0, envelope3_L, 0);
AudioConnection          patchCord8(playSdWav3, 1, envelope3_R, 0);
AudioConnection          patchCord9(envelope2_R, 0, Mixer_Master_R, 1);
AudioConnection          patchCord10(envelope4_R, 0, Mixer_Master_R, 3);
AudioConnection          patchCord11(envelope2_L, 0, Mixer_Master_L, 1);
AudioConnection          patchCord12(envelope1_R, 0, Mixer_Master_R, 0);
AudioConnection          patchCord13(envelope4_L, 0, Mixer_Master_L, 3);
AudioConnection          patchCord14(envelope3_R, 0, Mixer_Master_R, 2);
AudioConnection          patchCord15(envelope1_L, 0, Mixer_Master_L, 0);
AudioConnection          patchCord16(envelope3_L, 0, Mixer_Master_L, 2);
AudioConnection          patchCord17(Mixer_Master_L, 0, i2s1, 0);
AudioConnection          patchCord18(Mixer_Master_R, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=291,93
// GUItool: end automatically generated code



void initAudio(void)
{
  AudioMemory(16);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.4);
  sgtl5000_1.unmuteLineout(); 
  sgtl5000_1.lineOutLevel(13);

  new Voice(&playSdWav1, &envelope1_L, &envelope1_R);  // djt - jjust for testing... make a few and put into array
  // new Voice(&playSdWav2, &envelope2_L, &envelope2_R);  // djt - jjust for testing... make a few and put into array
  // new Voice(&playSdWav3, &envelope3_L, &envelope3_R);  // djt - jjust for testing... make a few and put into array
  // new Voice(&playSdWav4, &envelope4_L, &envelope4_R);  // djt - jjust for testing... make a few and put into array

  AudioInterrupts();
  // // TESTING
  // envelope_master_L.attack(0);
  // envelope_master_L.decay(2000);
  // envelope_master_L.release(2000);
  // envelope_master_L.sustain(1);
  // envelope_master_L.hold(0);
  // envelope_master_R.attack(0);
  // envelope_master_R.decay(500);
  // envelope_master_R.release(500);
  // envelope_master_R.sustain(1);
  // envelope_master_R.hold(0);
}

// Accepts a value from 0 - 100, maps to 0 - 0.8
void setHeadphoneVolume(uint8_t potval)
{
  if (potval < 0) potval = 0;
  if (potval > 100) potval = 100;

  float mappedVol = map(float(potval), 0, 100, 0, 0.7);
  Serial.println("mapped val");
  Serial.println(mappedVol);
  AudioNoInterrupts();
  sgtl5000_1.volume(mappedVol);
  AudioInterrupts();
}

// ------- Voice class stufffff --------///
uint8_t Voice::numVoices = 0;
std::vector<Voice*> Voice::allVoices = {};
Voice::Voice(AudioPlaySdWav* sampPlayer, AudioEffectEnvelope* env_l, AudioEffectEnvelope* env_r)
{
  numVoices++;
  allVoices.push_back(this);
  id = numVoices;
  volume = 0.4;
  playingState = false;
  attack = 0;
  release = 0; 
  samplePlayer = sampPlayer;

  envelope_l = env_l;
  envelope_r = env_r;
  envelope_l -> release(release);
  envelope_r -> release(release);
  envelope_l -> attack(attack);
  envelope_r -> attack(attack);
  envelope_l -> decay(500);
  envelope_r -> decay(500);
  envelope_l -> hold(0);
  envelope_r -> hold(0);
  envelope_l -> sustain(0);
  envelope_r -> sustain(0);
  envelope_l -> releaseNoteOn(8);  // djt - idk if I want or need these... but whatev.
  envelope_r -> releaseNoteOn(8);
 
 // DJT TESTING 
  sampleFilename = "hat.WAV";
  // if (numVoices % 2 == 0)
  // {
  //   Serial.println("Assign a clap!!!!!!!!!!!");
  //   sampleFilename = "clp.WAV";
  // }
}

void Voice::assignDefaultVoicesForSequencer(Sequencer* seq)
{
  uint8_t maxVoices = numVoices;
  uint8_t nextVoiceIdx = 0;              
  for (uint8_t i = 0; i < seq -> getNumSteps(); i++)
  {
    Voice * currVoice = allVoices[nextVoiceIdx];
    Serial.println(currVoice->getAttack());
    seq -> steps[i].assignVoice(allVoices[nextVoiceIdx]);
    Serial.println("In assigndefaultvoices");
    Serial.println(seq->getCurrentStepNumber());
    Serial.println(seq->getCurrentVoiceAttack());
    nextVoiceIdx++;
    if (nextVoiceIdx > maxVoices - 1) nextVoiceIdx = 0;
  }
}

void Voice::play(void)
{
  AudioNoInterrupts();
  envelope_l -> noteOff();
  envelope_r -> noteOff();
  // envelope_master_L.noteOff();
  // envelope_master_R.noteOff();

  samplePlayer -> play(sampleFilename);

  // envelope_master_L.noteOn();
  // envelope_master_R.noteOn();
  envelope_l -> noteOn();
  envelope_r -> noteOn();

  AudioInterrupts();

} 

void Voice::printVoiceDebug(void)
{
  Serial.println(" ---- VOICE DEBUG -----");
  Serial.println("Voice Attack: ");
  Serial.println(attack);
  Serial.println("Voice Release ");
  Serial.println(release);
  Serial.println("Voice ID ");
  Serial.println(id);
}

void Voice::setRelease(uint16_t rel)
{
  release = float(rel);
  AudioNoInterrupts();
  envelope_l -> release(release);
  envelope_r -> release(release);
  AudioInterrupts();
}

void Voice::setAttack(uint16_t atk)
{
  attack = atk;
  AudioNoInterrupts();
  envelope_l -> attack(attack);
  envelope_r -> attack(attack);
  AudioInterrupts();
}

void Voice::setDecay(uint16_t dec)
{
  decay = dec;
  AudioNoInterrupts();
  envelope_l -> decay(decay);
  envelope_r -> decay(decay);
  AudioInterrupts();
}

void Voice::setSustain(uint16_t sus)
{
  sustain = sus;
  AudioNoInterrupts();
  envelope_l -> sustain(sustain);
  envelope_r -> sustain(sustain);
  AudioInterrupts();
}