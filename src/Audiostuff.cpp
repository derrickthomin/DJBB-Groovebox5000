#include <Audio.h>
#include <Arduino.h>
#include <SerialFlash.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <vector>
#include "Audiostuff.h"
#include "Sequencer.h"
#include "Inputs.h"

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav1;     //xy=133,580
AudioPlaySdWav           playSdWav3;     //xy=134,760
AudioPlaySdWav           playSdWav4;     //xy=134,851
AudioPlaySdWav           playSdWav2;     //xy=136,674
AudioSynthSimpleDrum     drum2;          //xy=137.77777099609375,1477.7779579162598
AudioSynthSimpleDrum     drum3;          //xy=137.77777481079102,1524.4443283081055
AudioSynthNoisePink      pink1;          //xy=139.66665649414062,1164.1111707687378
AudioSynthSimpleDrum     drum1;          //xy=138.88888549804688,1432.2221851348877
AudioSynthNoisePink      pink2; //xy=143,1278
AudioEffectEnvelope      envelope_Drum3; //xy=316.6666564941406,1524.4443359375
AudioEffectEnvelope      envelope_Drum2;  //xy=318.8888931274414,1475.5555601119995
AudioEffectEnvelope      envelope_Drum1; //xy=321.111083984375,1428.8887939453125
AudioEffectEnvelope      envelope6_L; //xy=324.6666793823242,1254.666648864746
AudioEffectEnvelope      envelope6_R; //xy=325.66666412353516,1296.0000381469727
AudioEffectEnvelope      envelope5_L; //xy=327.9999580383301,1144.9999504089355
AudioEffectEnvelope      envelope5_R; //xy=328.99999237060547,1184.9999923706055
AudioEffectEnvelope      envelope1_L;    //xy=331.88887786865234,559.1111335754395
AudioEffectEnvelope      envelope1_R;    //xy=336,594
AudioEffectEnvelope      envelope4_L;    //xy=337,829
AudioEffectEnvelope      envelope4_R;    //xy=337,872
AudioEffectEnvelope      envelope3_L;    //xy=339,740
AudioEffectEnvelope      envelope3_R;    //xy=339,775
AudioEffectEnvelope      envelope2_L;    //xy=340,655
AudioEffectEnvelope      envelope2_R;    //xy=340,691
AudioEffectBitcrusher    bitcrush_noise_R1; //xy=518.5,1185
AudioEffectBitcrusher    bitcrush_noise_L1;    //xy=519,1144
AudioEffectBitcrusher    bitcrush_noise_L2; //xy=523,1257
AudioEffectBitcrusher    bitcrush_noise_R2; //xy=524.5,1297
AudioFilterStateVariable filter_noise_L1;        //xy=717.0000228881836,1137.0000171661377
AudioFilterStateVariable filter_noise_R2; //xy=717,1301
AudioFilterStateVariable filter_noise_R1; //xy=718,1193
AudioFilterStateVariable filter_noise_L2; //xy=718,1250
AudioMixer4              Mixer_Samplr_R; //xy=941,721
AudioMixer4              Mixer_Samplr_L; //xy=943,654
AudioMixer4              Mixer_Noise_Drum; //xy=986.6666717529297,1471.1111278533936
AudioMixer4              Mixer_Noise_L; //xy=998.888988494873,1165.111240386963
AudioMixer4              Mixer_Noise_R; //xy=999.2778434753418,1267.2223815917969
AudioMixer4              Mixer_Master_L; //xy=1272.888916015625,950.2221984863281
AudioMixer4              Mixer_Master_R; //xy=1272.888916015625,1026.2221984863281
AudioOutputI2S           i2s1;           //xy=1466.888916015625,990.2221984863281
AudioConnection          patchCord1(playSdWav1, 0, envelope1_L, 0);
AudioConnection          patchCord2(playSdWav1, 1, envelope1_R, 0);
AudioConnection          patchCord3(playSdWav3, 0, envelope3_L, 0);
AudioConnection          patchCord4(playSdWav3, 1, envelope3_R, 0);
AudioConnection          patchCord5(playSdWav4, 0, envelope4_L, 0);
AudioConnection          patchCord6(playSdWav4, 1, envelope4_R, 0);
AudioConnection          patchCord7(playSdWav2, 0, envelope2_L, 0);
AudioConnection          patchCord8(playSdWav2, 1, envelope2_R, 0);
AudioConnection          patchCord9(drum2, envelope_Drum2);
AudioConnection          patchCord10(drum3, envelope_Drum3);
AudioConnection          patchCord11(pink1, envelope5_L);
AudioConnection          patchCord12(pink1, envelope5_R);
AudioConnection          patchCord13(drum1, envelope_Drum1);
AudioConnection          patchCord14(pink2, envelope6_L);
AudioConnection          patchCord15(pink2, envelope6_R);
AudioConnection          patchCord16(envelope_Drum3, 0, Mixer_Noise_Drum, 2);
AudioConnection          patchCord17(envelope_Drum2, 0, Mixer_Noise_Drum, 1);
AudioConnection          patchCord18(envelope_Drum1, 0, Mixer_Noise_Drum, 0);
AudioConnection          patchCord19(envelope6_L, bitcrush_noise_L2);
AudioConnection          patchCord20(envelope6_R, bitcrush_noise_R2);
AudioConnection          patchCord21(envelope5_L, bitcrush_noise_L1);
AudioConnection          patchCord22(envelope5_R, bitcrush_noise_R1);
AudioConnection          patchCord23(envelope1_L, 0, Mixer_Samplr_L, 0);
AudioConnection          patchCord24(envelope1_R, 0, Mixer_Samplr_R, 0);
AudioConnection          patchCord25(envelope4_L, 0, Mixer_Samplr_L, 3);
AudioConnection          patchCord26(envelope4_R, 0, Mixer_Samplr_R, 3);
AudioConnection          patchCord27(envelope3_L, 0, Mixer_Samplr_L, 2);
AudioConnection          patchCord28(envelope3_R, 0, Mixer_Samplr_R, 2);
AudioConnection          patchCord29(envelope2_L, 0, Mixer_Samplr_L, 1);
AudioConnection          patchCord30(envelope2_R, 0, Mixer_Samplr_R, 1);
AudioConnection          patchCord31(bitcrush_noise_R1, 0, filter_noise_R1, 0);
AudioConnection          patchCord32(bitcrush_noise_L1, 0, filter_noise_L1, 0);
AudioConnection          patchCord33(bitcrush_noise_L2, 0, filter_noise_L2, 0);
AudioConnection          patchCord34(bitcrush_noise_R2, 0, filter_noise_R2, 0);
AudioConnection          patchCord35(filter_noise_L1, 0, Mixer_Noise_L, 0);
AudioConnection          patchCord36(filter_noise_L1, 2, Mixer_Noise_L, 1);
AudioConnection          patchCord37(filter_noise_R2, 0, Mixer_Noise_R, 2);
AudioConnection          patchCord38(filter_noise_R2, 2, Mixer_Noise_R, 3);
AudioConnection          patchCord39(filter_noise_R1, 0, Mixer_Noise_R, 0);
AudioConnection          patchCord40(filter_noise_R1, 2, Mixer_Noise_R, 1);
AudioConnection          patchCord41(filter_noise_L2, 0, Mixer_Noise_L, 2);
AudioConnection          patchCord42(filter_noise_L2, 2, Mixer_Noise_L, 3);
AudioConnection          patchCord43(Mixer_Samplr_R, 0, Mixer_Master_R, 0);
AudioConnection          patchCord44(Mixer_Samplr_L, 0, Mixer_Master_L, 0);
AudioConnection          patchCord45(Mixer_Noise_Drum, 0, Mixer_Master_L, 2);
AudioConnection          patchCord46(Mixer_Noise_Drum, 0, Mixer_Master_R, 2);
AudioConnection          patchCord47(Mixer_Noise_L, 0, Mixer_Master_L, 1);
AudioConnection          patchCord48(Mixer_Noise_R, 0, Mixer_Master_R, 1);
AudioConnection          patchCord49(Mixer_Master_L, 0, i2s1, 0);
AudioConnection          patchCord50(Mixer_Master_R, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=266,451
// GUItool: end automatically generated code

void initAudio(void)
{
  AudioMemory(16);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.4);
  sgtl5000_1.unmuteLineout(); 
  sgtl5000_1.lineOutLevel(13);

  //new Voice(&playSdWav1, &envelope1_L, &envelope1_R);  // djt - jjust for testing... make a few and put into array
  //new Voice(&pink1, &envelope5_L, &envelope5_R, &filter_noise_L1, &filter_noise_R1, &bitcrush_noise_L1, &bitcrush_noise_R1);  // djt - jjust for testing... make a few and put into array
  new Voice(&drum1, &envelope_Drum1); 
  new Voice(&drum2, &envelope_Drum2); 
  new Voice(&drum3, &envelope_Drum3); 
  // new Voice(&playSdWav4, &envelope4_L, &envelope4_R);  // djt - jjust for testing... make a few and put into array

  AudioInterrupts();
}
/*
     *********************** Global / Sequencer Level Functions *************************
     ***************************************************************************************
*/

// Accepts a value from 0 - 100, maps to 0 - 0.8
void setHeadphoneVolume(uint8_t val)
{
  float mappedVol = map(float(val), 0, 100, 0, 0.7);
  AudioNoInterrupts();
  sgtl5000_1.volume(mappedVol);
  AudioInterrupts();
}

void setGlobalReverbLevel(uint8_t val = -1)  // Pot x callback function
{
  Serial.print("DJT CONFIGURE ME - set global reverb level");
}

void setGlobalDelayLevel(uint8_t val = -1) // Pot x callback function
{
  Serial.print("DJT CONFIGURE ME - set global delay level");
}

// -50 to 50
// Idea is that this along with the next setting can set
// swing every X steps. May want to do per sequencer.
void setGlobalSwingAmt(int8_t val = 0) // Pot x callback function
{
  Serial.print("DJT CONFIGURE ME - set global Swing Value");
}

// Use with above setting. 2 = apply swing every 2 steps, 3 = every 3 steps, etc.
void setGlobalSwingStepSpacing(int8_t val = 3) // Pot x callback function
{
  Serial.print("DJT CONFIGURE ME - set global Swing spacing");
}


// ------- Voice Class --------///
uint8_t Voice::numVoices = 0;
std::vector<Voice*> Voice::allVoices = {};
Voice::Voice(AudioPlaySdWav* sampPlayer, AudioEffectEnvelope* env_l, AudioEffectEnvelope* env_r)  // Sampler constructor
{
  type = 0;
  numVoices++;
  allVoices.push_back(this);
  id = numVoices;
  volume = 0.4;
  playingState = false;
  attack = 0;
  release = 500; 
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
  sampleFilename = "clp.WAV";
  // if (numVoices % 2 == 0)
  // {
  //   Serial.println("Assign a clap!!!!!!!!!!!");
  //   sampleFilename = "clp.WAV";
  // }
}

Voice::Voice(AudioSynthNoisePink* noisee, AudioEffectEnvelope* env_l, AudioEffectEnvelope* env_r,   // Noise voice
      AudioFilterStateVariable* filt_l, AudioFilterStateVariable* filt_r, 
      AudioEffectBitcrusher* bitcr_l, AudioEffectBitcrusher* bitcr_r)
{
  type = 1;   // Noise
  numVoices++;
  allVoices.push_back(this);
  id = numVoices;
  volume = 0.4;
  playingState = false;
  attack = 0;
  release = 500; 
  noise = noisee;

  envelope_l = env_l;
  envelope_r = env_r;

  filter_l = filt_l;
  filter_r = filt_r;

  bitcrush_l = bitcr_l;
  bitcrush_r = bitcr_r;

  noise    -> amplitude(DEFAULT_NOISE_LVL);
  filter_l -> frequency(DEFAULT_FILT_FREQ);
  filter_r -> frequency(DEFAULT_FILT_FREQ);
  filter_l -> resonance(DEFAULT_FILT_Q);
  filter_r -> resonance(DEFAULT_FILT_Q);

  bitcrush_l -> sampleRate(DEFAULT_CRUSH_FREQ);
  bitcrush_r -> sampleRate(DEFAULT_CRUSH_FREQ);
  bitcrush_l -> bits(DEFAULT_CRUSH_BITDEPTH);
  bitcrush_r -> bits(DEFAULT_CRUSH_BITDEPTH);

  envelope_l -> release(release);
  envelope_r -> release(release);

  envelope_l -> attack(attack);
  envelope_r -> attack(attack);

  envelope_l -> decay(500);
  envelope_r -> decay(500);

  envelope_l -> hold(0);
  envelope_r -> hold(0);

  envelope_l -> sustain(0.0);
  envelope_r -> sustain(0.0);

  envelope_l -> releaseNoteOn(8);  // djt - idk if I want or need these... but whatev.
  envelope_r -> releaseNoteOn(8);

}

Voice::Voice(AudioSynthSimpleDrum* drm, AudioEffectEnvelope* env) // Drum constructor
{
  type = 2;
  numVoices++;
  allVoices.push_back(this);
  id = numVoices;
  volume = 0.4;
  playingState = false;
  attack = 0;
  release = 500; 
  drum = drm;

  envelope_l = env;
  envelope_r = env;

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
}

void Voice::assignDefaultVoicesForSequencer(Sequencer* seq)
{
  uint8_t maxVoices = numVoices;
  uint8_t nextVoiceIdx = 0;              
  for (uint8_t i = 0; i < seq -> getNumSteps(); i++)
  {
    Voice * currVoice = allVoices[nextVoiceIdx];
    seq -> steps[i].assignVoice(allVoices[nextVoiceIdx]);
    nextVoiceIdx++;
    if (nextVoiceIdx > maxVoices - 1) nextVoiceIdx = 0;
  }
}

void Voice::play(void)
{
  Serial.println("in Voice::play");
  AudioNoInterrupts();
  envelope_l -> noteOff();
  envelope_r -> noteOff();
  switch (type)
  {
    case 0:  // Sample voice
      samplePlayer -> play(sampleFilename);
      break;

    case 1: // noise voice
      break;

    case 2: // Drum voice
      drum->noteOn();
      break;
  }
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
  Serial.println("Voice Decay ");
  Serial.println(decay);
  Serial.println("Voice ID ");
  Serial.println(id);
}

void Voice::setVolume(float vol){volume = vol;}

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

// Drum Voice Type
void Voice::setDrumFreq(uint16_t frq)
{
  drumFreq = frq;
  drum->frequency(drumFreq);
}

void Voice::setDrumPMod(float pmod)
{
  drumPMod = pmod;
  drum->pitchMod(drumPMod);
}

void Voice::setDrum2ndMix(float mix2)
{
  drum2ndHitMix = mix2;
  drum->secondMix(drum2ndHitMix);
}

void Voice::setDrumLength(uint16_t len)
{
  drumLength = len;
  drum->length(drumLength);
}

void Voice::setInputLabels(uint8_t bankNum, std::vector<char*> * labels)
{
  switch (bankNum)
  {
  case 1:
  
    break;
  
  case 2:
    break;
  
  default:
    break;
  }
}