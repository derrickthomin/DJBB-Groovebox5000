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
AudioSynthNoisePink      pink1;          //xy=59.11109924316406,716.999981880188
AudioSynthSimpleDrum     drum2;          //xy=61.666656494140625,1028.9999685287476
AudioSynthSimpleDrum     drum3;          //xy=61.666656494140625,1077.9999685287476
AudioSynthNoisePink      pink2;          //xy=63.11109924316406,830.999981880188
AudioSynthSimpleDrum     drum1;          //xy=63.666656494140625,981.9999685287476
AudioPlaySdWav           playSdWav1;     //xy=86.5555648803711,260.6666851043701
AudioPlaySdWav           playSdWav3;     //xy=87.5555648803711,440.6666851043701
AudioPlaySdWav           playSdWav4;     //xy=87.5555648803711,531.6666851043701
AudioPlaySdWav           playSdWav2;     //xy=89.5555648803711,354.6666851043701
AudioEffectEnvelope      envelope_Drum2; //xy=221.55555725097656,1029.0000104904175
AudioEffectEnvelope      envelope_Drum1; //xy=222.33331298828125,980.8888368606567
AudioEffectEnvelope      envelope_Drum3; //xy=225.77777099609375,1076.9999723434448
AudioEffectEnvelope      envelope6_L;    //xy=244.11109924316406,806.999981880188
AudioEffectEnvelope      envelope6_R;    //xy=245.11109924316406,848.999981880188
AudioEffectEnvelope      envelope5_L;    //xy=247.11109924316406,696.999981880188
AudioEffectEnvelope      envelope5_R;    //xy=248.11109924316406,736.999981880188
AudioEffectEnvelope      envelope1_L;    //xy=284.5555648803711,239.66668510437012
AudioEffectEnvelope      envelope1_R;    //xy=289.5555648803711,274.6666851043701
AudioEffectEnvelope      envelope4_L;    //xy=290.5555648803711,509.6666851043701
AudioEffectEnvelope      envelope4_R;    //xy=290.5555648803711,552.6666851043701
AudioEffectEnvelope      envelope3_L;    //xy=292.5555648803711,420.6666851043701
AudioEffectEnvelope      envelope3_R;    //xy=292.5555648803711,455.6666851043701
AudioEffectEnvelope      envelope2_L;    //xy=293.5555648803711,335.6666851043701
AudioEffectEnvelope      envelope2_R;    //xy=293.5555648803711,371.6666851043701
AudioAmplifier           amp_drum_3;     //xy=405.99999237060547,1077.1110858917236
AudioAmplifier           amp_drum_2;     //xy=407.8888931274414,1028.444356918335
AudioAmplifier           amp_drum_1;     //xy=408.4444770812988,979.9999237060547
AudioEffectBitcrusher    bitcrush_noise_L2; //xy=429.7777557373047,806.6666584014893
AudioEffectBitcrusher    bitcrush_noise_R2; //xy=429.66663360595703,848.88889503479
AudioEffectBitcrusher    bitcrush_noise_R1; //xy=438.11109924316406,737.999981880188
AudioEffectBitcrusher    bitcrush_noise_L1; //xy=439.11109924316406,696.999981880188
AudioAmplifier           amp_samplr_L3;  //xy=473.5555648803711,419.6666851043701
AudioAmplifier           amp_samplr_R4;  //xy=476.5555648803711,551.6666851043701
AudioAmplifier           amp_samplr_R3;  //xy=479.5555648803711,458.6666851043701
AudioAmplifier           amp_samplr_L2;  //xy=480.5555648803711,330.6666851043701
AudioAmplifier           amp_samplr_L4;  //xy=480.5555648803711,508.6666851043701
AudioAmplifier           amp_samplr_R2;  //xy=482.5555648803711,373.6666851043701
AudioAmplifier           amp_samplr_R1;  //xy=486.5555648803711,276.6666851043701
AudioAmplifier           amp_samplr_L1;  //xy=488.5555648803711,235.66668510437012
AudioEffectBitcrusher    bitcrusher_drum2;    //xy=586.6665878295898,1028.8888549804688
AudioEffectBitcrusher    bitcrusher_drum3;    //xy=587.7777709960938,1076.6668720245361
AudioEffectBitcrusher    bitcrusher_drum1;    //xy=588.8889083862305,978.8890352249146
AudioAmplifier           amp_noise_L2;   //xy=615.1110992431641,806.999981880188
AudioAmplifier           amp_noise_R2;   //xy=616.5555419921875,849.6666812896729
AudioAmplifier           amp_noise_L1;   //xy=625.1110992431641,693.999981880188
AudioAmplifier           amp_noise_R1;   //xy=625.1110992431641,733.999981880188
AudioFilterStateVariable filter_drum3;        //xy=773.3332824707031,1086.666805267334
AudioFilterStateVariable filter_drum2;        //xy=774.4444580078125,1030.0001058578491
AudioFilterStateVariable filter_drum1;        //xy=775.5555572509766,971.1111536026001
AudioFilterStateVariable filter_noise_R2; //xy=787.1110992431641,865.999981880188
AudioFilterStateVariable filter_noise_L2; //xy=788.1110992431641,804.999981880188
AudioFilterStateVariable filter_noise_L1; //xy=792.1110992431641,694.999981880188
AudioFilterStateVariable filter_noise_R1; //xy=797.1110992431641,745.999981880188
AudioMixer4              Mixer_Samplr_R; //xy=894.5555648803711,401.6666851043701
AudioMixer4              Mixer_Samplr_L; //xy=896.5555648803711,334.6666851043701
AudioMixer4              Mixer_Drum; //xy=1016.6666412353516,1019.5557518005371
AudioMixer4              Mixer_Noise_L;  //xy=1027.111099243164,709.999981880188
AudioMixer4              Mixer_Noise_R;  //xy=1029.2222137451172,821.0000085830688
AudioEffectDelay         delay2; //xy=1302.2223281860352,1072.2221717834473
AudioMixer4              Mixer_PreFX_R; //xy=1306.6666870117188,813.333324432373
AudioMixer4              Mixer_PreFX_L; //xy=1308.8888473510742,725.1110916137695
AudioEffectFreeverbStereo freeverbs1;     //xy=1311.111125946045,583.3332824707031
AudioEffectDelay         delay1;         //xy=1317.7776794433594,487.7777862548828
AudioEffectFreeverbStereo freeverbs2; //xy=1324.4444961547852,975.5554656982422
AudioMixer4              mixer1;         //xy=1480.000156402588,1046.6666278839111
AudioMixer4              mixer2;         //xy=1486.6668281555176,462.22220611572266
AudioMixer4              Mixer_Master_R; //xy=1558.8887329101562,843.3333396911621
AudioMixer4              Mixer_Master_L; //xy=1562.222095489502,708.8888320922852
AudioMixer4              Mixer_FX_R; //xy=1616.6667861938477,988.8888740539551
AudioMixer4              Mixer_FX_L; //xy=1630.0000038146973,462.22218322753906
AudioOutputI2S           i2s1;           //xy=1749.5556373596191,752.8887691497803
AudioConnection          patchCord1(pink1, envelope5_L);
AudioConnection          patchCord2(pink1, envelope5_R);
AudioConnection          patchCord3(drum2, envelope_Drum2);
AudioConnection          patchCord4(drum3, envelope_Drum3);
AudioConnection          patchCord5(pink2, envelope6_L);
AudioConnection          patchCord6(pink2, envelope6_R);
AudioConnection          patchCord7(drum1, envelope_Drum1);
AudioConnection          patchCord8(playSdWav1, 0, envelope1_L, 0);
AudioConnection          patchCord9(playSdWav1, 1, envelope1_R, 0);
AudioConnection          patchCord10(playSdWav3, 0, envelope3_L, 0);
AudioConnection          patchCord11(playSdWav3, 1, envelope3_R, 0);
AudioConnection          patchCord12(playSdWav4, 0, envelope4_L, 0);
AudioConnection          patchCord13(playSdWav4, 1, envelope4_R, 0);
AudioConnection          patchCord14(playSdWav2, 0, envelope2_L, 0);
AudioConnection          patchCord15(playSdWav2, 1, envelope2_R, 0);
AudioConnection          patchCord16(envelope_Drum2, amp_drum_2);
AudioConnection          patchCord17(envelope_Drum1, amp_drum_1);
AudioConnection          patchCord18(envelope_Drum3, amp_drum_3);
AudioConnection          patchCord19(envelope6_L, bitcrush_noise_L2);
AudioConnection          patchCord20(envelope6_R, bitcrush_noise_R2);
AudioConnection          patchCord21(envelope5_L, bitcrush_noise_L1);
AudioConnection          patchCord22(envelope5_R, bitcrush_noise_R1);
AudioConnection          patchCord23(envelope1_L, amp_samplr_L1);
AudioConnection          patchCord24(envelope1_R, amp_samplr_R1);
AudioConnection          patchCord25(envelope4_L, amp_samplr_L4);
AudioConnection          patchCord26(envelope4_R, amp_samplr_R4);
AudioConnection          patchCord27(envelope3_L, amp_samplr_L3);
AudioConnection          patchCord28(envelope3_R, amp_samplr_R3);
AudioConnection          patchCord29(envelope2_L, amp_samplr_L2);
AudioConnection          patchCord30(envelope2_R, amp_samplr_R2);
AudioConnection          patchCord31(amp_drum_3, bitcrusher_drum3);
AudioConnection          patchCord32(amp_drum_2, bitcrusher_drum2);
AudioConnection          patchCord33(amp_drum_1, bitcrusher_drum1);
AudioConnection          patchCord34(bitcrush_noise_L2, amp_noise_L2);
AudioConnection          patchCord35(bitcrush_noise_R2, amp_noise_R2);
AudioConnection          patchCord36(bitcrush_noise_R1, amp_noise_R1);
AudioConnection          patchCord37(bitcrush_noise_L1, amp_noise_L1);
AudioConnection          patchCord38(amp_samplr_L3, 0, Mixer_Samplr_L, 2);
AudioConnection          patchCord39(amp_samplr_R4, 0, Mixer_Samplr_R, 3);
AudioConnection          patchCord40(amp_samplr_R3, 0, Mixer_Samplr_R, 2);
AudioConnection          patchCord41(amp_samplr_L2, 0, Mixer_Samplr_L, 1);
AudioConnection          patchCord42(amp_samplr_L4, 0, Mixer_Samplr_L, 3);
AudioConnection          patchCord43(amp_samplr_R2, 0, Mixer_Samplr_R, 1);
AudioConnection          patchCord44(amp_samplr_R1, 0, Mixer_Samplr_R, 0);
AudioConnection          patchCord45(amp_samplr_L1, 0, Mixer_Samplr_L, 0);
AudioConnection          patchCord46(bitcrusher_drum2, 0, filter_drum2, 0);
AudioConnection          patchCord47(bitcrusher_drum3, 0, filter_drum3, 0);
AudioConnection          patchCord48(bitcrusher_drum1, 0, filter_drum1, 0);
AudioConnection          patchCord49(amp_noise_L2, 0, filter_noise_L2, 0);
AudioConnection          patchCord50(amp_noise_R2, 0, filter_noise_R2, 0);
AudioConnection          patchCord51(amp_noise_L1, 0, filter_noise_L1, 0);
AudioConnection          patchCord52(amp_noise_R1, 0, filter_noise_R1, 0);
AudioConnection          patchCord53(filter_drum3, 0, Mixer_Drum, 2);
AudioConnection          patchCord54(filter_drum2, 0, Mixer_Drum, 1);
AudioConnection          patchCord55(filter_drum1, 0, Mixer_Drum, 0);
AudioConnection          patchCord56(filter_noise_R2, 0, Mixer_Noise_R, 2);
AudioConnection          patchCord57(filter_noise_R2, 2, Mixer_Noise_R, 3);
AudioConnection          patchCord58(filter_noise_L2, 0, Mixer_Noise_L, 2);
AudioConnection          patchCord59(filter_noise_L2, 2, Mixer_Noise_L, 3);
AudioConnection          patchCord60(filter_noise_L1, 0, Mixer_Noise_L, 0);
AudioConnection          patchCord61(filter_noise_L1, 2, Mixer_Noise_L, 1);
AudioConnection          patchCord62(filter_noise_R1, 0, Mixer_Noise_R, 0);
AudioConnection          patchCord63(filter_noise_R1, 2, Mixer_Noise_R, 1);
AudioConnection          patchCord64(Mixer_Samplr_R, 0, Mixer_PreFX_R, 0);
AudioConnection          patchCord65(Mixer_Samplr_L, 0, Mixer_PreFX_L, 0);
AudioConnection          patchCord66(Mixer_Drum, 0, Mixer_PreFX_R, 2);
AudioConnection          patchCord67(Mixer_Drum, 0, Mixer_PreFX_L, 2);
AudioConnection          patchCord68(Mixer_Noise_L, 0, Mixer_PreFX_L, 1);
AudioConnection          patchCord69(Mixer_Noise_R, 0, Mixer_PreFX_R, 1);
AudioConnection          patchCord70(delay2, 0, mixer1, 0);
AudioConnection          patchCord71(delay2, 1, mixer1, 1);
AudioConnection          patchCord72(delay2, 2, mixer1, 2);
AudioConnection          patchCord73(delay2, 3, mixer1, 3);
AudioConnection          patchCord74(delay2, 4, Mixer_FX_L, 3);
AudioConnection          patchCord75(Mixer_PreFX_R, freeverbs2);
AudioConnection          patchCord76(Mixer_PreFX_R, delay2);
AudioConnection          patchCord77(Mixer_PreFX_R, 0, Mixer_Master_R, 0);
AudioConnection          patchCord78(Mixer_PreFX_L, delay1);
AudioConnection          patchCord79(Mixer_PreFX_L, freeverbs1);
AudioConnection          patchCord80(Mixer_PreFX_L, 0, Mixer_Master_L, 0);
AudioConnection          patchCord81(freeverbs1, 0, Mixer_FX_L, 0);
AudioConnection          patchCord82(freeverbs1, 1, Mixer_FX_L, 1);
AudioConnection          patchCord83(delay1, 0, mixer2, 0);
AudioConnection          patchCord84(delay1, 1, mixer2, 1);
AudioConnection          patchCord85(delay1, 2, mixer2, 2);
AudioConnection          patchCord86(delay1, 3, mixer2, 3);
AudioConnection          patchCord87(delay1, 4, Mixer_FX_R, 3);
AudioConnection          patchCord88(freeverbs2, 0, Mixer_FX_R, 0);
AudioConnection          patchCord89(freeverbs2, 1, Mixer_FX_R, 1);
AudioConnection          patchCord90(mixer1, 0, Mixer_FX_R, 2);
AudioConnection          patchCord91(mixer2, 0, Mixer_FX_L, 2);
AudioConnection          patchCord92(Mixer_Master_R, 0, i2s1, 1);
AudioConnection          patchCord93(Mixer_Master_L, 0, i2s1, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=219.5555648803711,131.66668510437012
// GUItool: end automatically generated code

void initAudio(void){  
  AudioMemory(16);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.4);
  sgtl5000_1.unmuteLineout(); 
  sgtl5000_1.lineOutLevel(13);

  //new Voice(&playSdWav1, &envelope1_L, &envelope1_R);  // djt - jjust for testing... make a few and put into array 
   //new Voice(&pink1, &envelope5_L, &envelope5_R, &amp_noise_L1, &amp_noise_R1, &filter_noise_L1, &filter_noise_R1, &bitcrush_noise_L1, &bitcrush_noise_R1);  // djt - jjust for testing... make a few and put into array
  new Voice(&drum1, &envelope_Drum1, &amp_drum_1, &bitcrusher_drum1, &filter_drum1); 
  //new Voice(&drum2, &envelope_Drum2, &amp_drum_2, &bitcrusher_drum2, &filter_drum2); 
  //new Voice(&drum3, &envelope_Drum3, &amp_drum_3, &bitcrusher_drum3, &filter_drum3); 
  //new Voice(&playSdWav1, &envelope1_L, &envelope1_R, &amp_samplr_L1, &amp_samplr_R1);  // djt - jjust for testing... make a few and put into array

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
Voice::Voice(AudioPlaySdWav* sampPlayer, AudioEffectEnvelope* env_l, AudioEffectEnvelope* env_r, AudioAmplifier* amp_l, AudioAmplifier* amp_r)  // Sampler constructor
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
  amplifier_l = amp_l;
  amplifier_r = amp_r;

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
      AudioAmplifier* amp_l, AudioAmplifier* amp_r, AudioFilterStateVariable* filt_l, AudioFilterStateVariable* filt_r, 
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

  amplifier_l = amp_l;
  amplifier_r = amp_r;

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

  amplifier_l -> gain(0);
  amplifier_r -> gain(0);

}

Voice::Voice(AudioSynthSimpleDrum* drm, AudioEffectEnvelope* env, AudioAmplifier* amp,   // Drum
      AudioEffectBitcrusher* bitcr, AudioFilterStateVariable* filt)
{
  type = 2; // drum
  numVoices++;
  allVoices.push_back(this);
  id = numVoices;
  volume = 0.4;
  playingState = false;
  attack = 0;
  release = 500; 
  drum = drm;
  amplifier_l = amp;
  amplifier_r = amp;
  bitcrush_l = bitcr;
  filter_l = filt;

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

  // Other defaults
  bitcrush_l -> sampleRate(DEFAULT_CRUSH_FREQ);
  bitcrush_l -> bits(DEFAULT_CRUSH_BITDEPTH);
  filter_l   -> frequency(DEFAULT_FILT_FREQ);
  filter_l   -> resonance(DEFAULT_FILT_Q);
}

void Voice::assignDefaultVoicesForSequencer(Sequencer* seq)
{
  uint8_t maxVoices = numVoices;
  uint8_t nextVoiceIdx = 0;              
  for (uint8_t i = 0; i < seq -> getNumSteps(); i++)
  {
    // Voice * currVoice = allVoices[nextVoiceIdx];
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
  Serial.println("Voice Type ");
  Serial.println(type);
}

void Voice::setVolume(float vol)
{
  volume = vol;
  AudioNoInterrupts();
  amplifier_l -> gain(volume);
  amplifier_r -> gain(volume);
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

////////

const char* Voice::getTypeName(void)
{
  switch (type)
  {
  case 0:
    return "Sample";
  case 1:
    return "Noise";
  case 2:
    return "Drum Synth";
  }
}