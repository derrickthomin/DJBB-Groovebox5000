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

// GUItool: begin automatically generated code
AudioSynthNoisePink      pink1;          //xy=62,783
AudioSynthNoisePink      pink2;          //xy=66,897
AudioPlaySdWav           playSdWav1;     //xy=95,200
AudioPlaySdWav           playSdWav3;     //xy=96,380
AudioPlaySdWav           playSdWav4;     //xy=96,471
AudioPlaySdWav           playSdWav2;     //xy=98,294
AudioSynthSimpleDrum     drum2;          //xy=99,1095
AudioSynthSimpleDrum     drum3;          //xy=99,1144
AudioSynthSimpleDrum     drum1;          //xy=101,1048
AudioEffectEnvelope      envelope6_L;    //xy=247,873
AudioEffectEnvelope      envelope6_R;    //xy=248,915
AudioEffectEnvelope      envelope5_L;    //xy=250,763
AudioEffectEnvelope      envelope5_R;    //xy=251,803
AudioEffectEnvelope      envelope_Drum2; //xy=280,1095
AudioEffectEnvelope      envelope_Drum3; //xy=282,1143
AudioEffectEnvelope      envelope_Drum1; //xy=283,1048
AudioEffectEnvelope      envelope1_L;    //xy=293,179
AudioEffectEnvelope      envelope1_R;    //xy=298,214
AudioEffectEnvelope      envelope4_L;    //xy=299,449
AudioEffectEnvelope      envelope4_R;    //xy=299,492
AudioEffectEnvelope      envelope3_L;    //xy=301,360
AudioEffectEnvelope      envelope3_R;    //xy=301,395
AudioEffectEnvelope      envelope2_L;    //xy=302,275
AudioEffectEnvelope      envelope2_R;    //xy=302,311
AudioEffectBitcrusher    bitcrush_noise_R1; //xy=441,804
AudioEffectBitcrusher    bitcrush_noise_L1; //xy=442,763
AudioEffectBitcrusher    bitcrush_noise_L2; //xy=446,876
AudioEffectBitcrusher    bitcrush_noise_R2; //xy=447,916
AudioAmplifier           amp_samplr_L3; //xy=482,359
AudioAmplifier           amp_samplr_R4; //xy=485,491
AudioAmplifier           amp_samplr_R3; //xy=488,398
AudioAmplifier           amp_samplr_L2; //xy=489,270
AudioAmplifier           amp_samplr_L4; //xy=489,448
AudioAmplifier           amp_samplr_R2; //xy=491,313
AudioAmplifier           amp_samplr_R1; //xy=495,216
AudioAmplifier           amp_samplr_L1;           //xy=497,175
AudioAmplifier           amp_drum_3; //xy=530,1142
AudioAmplifier           amp_drum_2; //xy=533,1090
AudioAmplifier           amp_drum_1; //xy=538,1046
AudioAmplifier           amp_noise_L2; //xy=618,873
AudioAmplifier           amp_noise_R2; //xy=625,919
AudioAmplifier           amp_noise_L1; //xy=628,760
AudioAmplifier           amp_noise_R1; //xy=628,800
AudioFilterStateVariable filter_noise_R2; //xy=790,932
AudioFilterStateVariable filter_noise_L2; //xy=791,871
AudioFilterStateVariable filter_noise_L1; //xy=795,761
AudioFilterStateVariable filter_noise_R1; //xy=800,812
AudioMixer4              Mixer_Samplr_R; //xy=903,341
AudioMixer4              Mixer_Samplr_L; //xy=905,274
AudioMixer4              Mixer_Noise_Drum; //xy=954,1110
AudioMixer4              Mixer_Noise_R;  //xy=961,887
AudioMixer4              Mixer_Noise_L;  //xy=1030,776
AudioMixer4              Mixer_Master_L; //xy=1234,570
AudioMixer4              Mixer_Master_R; //xy=1234,646
AudioOutputI2S           i2s1;           //xy=1428,610
AudioConnection          patchCord1(pink1, envelope5_L);
AudioConnection          patchCord2(pink1, envelope5_R);
AudioConnection          patchCord3(pink2, envelope6_L);
AudioConnection          patchCord4(pink2, envelope6_R);
AudioConnection          patchCord5(playSdWav1, 0, envelope1_L, 0);
AudioConnection          patchCord6(playSdWav1, 1, envelope1_R, 0);
AudioConnection          patchCord7(playSdWav3, 0, envelope3_L, 0);
AudioConnection          patchCord8(playSdWav3, 1, envelope3_R, 0);
AudioConnection          patchCord9(playSdWav4, 0, envelope4_L, 0);
AudioConnection          patchCord10(playSdWav4, 1, envelope4_R, 0);
AudioConnection          patchCord11(playSdWav2, 0, envelope2_L, 0);
AudioConnection          patchCord12(playSdWav2, 1, envelope2_R, 0);
AudioConnection          patchCord13(drum2, envelope_Drum2);
AudioConnection          patchCord14(drum3, envelope_Drum3);
AudioConnection          patchCord15(drum1, envelope_Drum1);
AudioConnection          patchCord16(envelope6_L, bitcrush_noise_L2);
AudioConnection          patchCord17(envelope6_R, bitcrush_noise_R2);
AudioConnection          patchCord18(envelope5_L, bitcrush_noise_L1);
AudioConnection          patchCord19(envelope5_R, bitcrush_noise_R1);
AudioConnection          patchCord20(envelope_Drum2, amp_drum_2);
AudioConnection          patchCord21(envelope_Drum3, amp_drum_3);
AudioConnection          patchCord22(envelope_Drum1, amp_drum_1);
AudioConnection          patchCord23(envelope1_L, amp_samplr_L1);
AudioConnection          patchCord24(envelope1_R, amp_samplr_R1);
AudioConnection          patchCord25(envelope4_L, amp_samplr_L4);
AudioConnection          patchCord26(envelope4_R, amp_samplr_R4);
AudioConnection          patchCord27(envelope3_L, amp_samplr_L3);
AudioConnection          patchCord28(envelope3_R, amp_samplr_R3);
AudioConnection          patchCord29(envelope2_L, amp_samplr_L2);
AudioConnection          patchCord30(envelope2_R, amp_samplr_R2);
AudioConnection          patchCord31(bitcrush_noise_R1, amp_noise_R1);
AudioConnection          patchCord32(bitcrush_noise_L1, amp_noise_L1);
AudioConnection          patchCord33(bitcrush_noise_L2, amp_noise_L2);
AudioConnection          patchCord34(bitcrush_noise_R2, amp_noise_R2);
AudioConnection          patchCord35(amp_samplr_L3, 0, Mixer_Samplr_L, 2);
AudioConnection          patchCord36(amp_samplr_R4, 0, Mixer_Samplr_R, 3);
AudioConnection          patchCord37(amp_samplr_R3, 0, Mixer_Samplr_R, 2);
AudioConnection          patchCord38(amp_samplr_L2, 0, Mixer_Samplr_L, 1);
AudioConnection          patchCord39(amp_samplr_L4, 0, Mixer_Samplr_L, 3);
AudioConnection          patchCord40(amp_samplr_R2, 0, Mixer_Samplr_R, 1);
AudioConnection          patchCord41(amp_samplr_R1, 0, Mixer_Samplr_R, 0);
AudioConnection          patchCord42(amp_samplr_L1, 0, Mixer_Samplr_L, 0);
AudioConnection          patchCord43(amp_drum_3, 0, Mixer_Noise_Drum, 2);
AudioConnection          patchCord44(amp_drum_2, 0, Mixer_Noise_Drum, 1);
AudioConnection          patchCord45(amp_drum_1, 0, Mixer_Noise_Drum, 0);
AudioConnection          patchCord46(amp_noise_L2, 0, filter_noise_L2, 0);
AudioConnection          patchCord47(amp_noise_R2, 0, filter_noise_R2, 0);
AudioConnection          patchCord48(amp_noise_L1, 0, filter_noise_L1, 0);
AudioConnection          patchCord49(amp_noise_R1, 0, filter_noise_R1, 0);
AudioConnection          patchCord50(filter_noise_R2, 0, Mixer_Noise_R, 2);
AudioConnection          patchCord51(filter_noise_R2, 2, Mixer_Noise_R, 3);
AudioConnection          patchCord52(filter_noise_L2, 0, Mixer_Noise_L, 2);
AudioConnection          patchCord53(filter_noise_L2, 2, Mixer_Noise_L, 3);
AudioConnection          patchCord54(filter_noise_L1, 0, Mixer_Noise_L, 0);
AudioConnection          patchCord55(filter_noise_L1, 2, Mixer_Noise_L, 1);
AudioConnection          patchCord56(filter_noise_R1, 0, Mixer_Noise_R, 0);
AudioConnection          patchCord57(filter_noise_R1, 2, Mixer_Noise_R, 1);
AudioConnection          patchCord58(Mixer_Samplr_R, 0, Mixer_Master_R, 0);
AudioConnection          patchCord59(Mixer_Samplr_L, 0, Mixer_Master_L, 0);
AudioConnection          patchCord60(Mixer_Noise_Drum, 0, Mixer_Master_L, 2);
AudioConnection          patchCord61(Mixer_Noise_Drum, 0, Mixer_Master_R, 2);
AudioConnection          patchCord62(Mixer_Noise_R, 0, Mixer_Master_R, 1);
AudioConnection          patchCord63(Mixer_Noise_L, 0, Mixer_Master_L, 1);
AudioConnection          patchCord64(Mixer_Master_L, 0, i2s1, 0);
AudioConnection          patchCord65(Mixer_Master_R, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=228,71
// GUItool: end automatically generated code

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
  new Voice(&drum1, &envelope_Drum1, &amp_drum_1); 
  new Voice(&drum2, &envelope_Drum2, &amp_drum_2); 
  new Voice(&drum3, &envelope_Drum3, &amp_drum_3); 
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

Voice::Voice(AudioSynthSimpleDrum* drm, AudioEffectEnvelope* env, AudioAmplifier* amp) // Drum constructor
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
  amplifier = amp;

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

void Voice::setVolume(float vol)
{
  volume = vol;
  AudioNoInterrupts();
  amplifier -> gain(volume);
  AudioInterrupts();
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