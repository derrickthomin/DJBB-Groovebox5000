/*
    Putting most of the constants, etc. 
    in one place?
*/

// ------------- Inputs  ------------ //
#define INPUT_LONGPRESS_MILLIS  400      // hold this long to edit a step, etc.

// ------------- Screen ------------ //
#define SCREEN_WIDTH            128
#define SCREEN_HEIGHT           128
#define INFOBAR_DISP_TIME       200      // How many millis until the banner goes awasy
#define INPUT_CHG_DISP_TIME     1000     // Wait this long to reset labels after inputs quit chanigng

// ----------- Sequenecer ---------- //
#define MIN_BPM                 40
#define MAX_BPM                 240
#define DEFAULT_BPM             125

// ------------- Audio ------------- //
// Defaults //
#define STARTUP_NUM_VOICES      4    // How many voices to created
#define DEFAULT_FILT_FREQ       17000
#define DEFAULT_FILT_Q          1.1
#define DEFAULT_CRUSH_BITDEPTH  16
#define DEFAULT_CRUSH_FREQ      44100
#define DEFAULT_NOISE_LVL       0.5
#define DEFAULT_DRUM_FREQ       300
#define DEFAULT_DRUM_PMOD       0.6
#define DEFAULT_DRUM_LENGTH     100
#define DEFAULT_DRUM_MIX2       0 

// Drum //
#define MIN_DRUM_FREQUENCY      25
#define MAX_DRUM_FREQUENCY      800
#define MIN_DRUM_LEN            1      //ms
#define MAX_DRUM_LEN            1000    //ms
#define MIN_DRUM_PMOD           0.1
#define MAX_DRUM_PMOD           0.9   
#define MIN_DRUM_MIX2           0
#define MAX_DRUM_MIX2           0.5   // IDK what the units even are. needt to test   

// Filter & FX //
#define MIN_FILTER_FREQ         20
#define MAX_FILTER_FREQ         20000
#define MIN_FILTER_Q            0.7
#define MAX_FILTER_Q            4
#define MIN_CRUSH_BITDEPTH      3
#define MAX_CRUSH_BITDEPTH      16
#define MIN_CRUSH_FREQ          1000
#define MAX_CRUSH_FREQ          44100
#define MIN_RATCHET             0
#define MAX_RATCHET             4

// ASDR & Basic //
#define MIN_VOICE_VOL          0.1   // Don't want to go all the way to 0... should just turn the step off at that point
#define MAX_VOICE_VOL          0.6
#define MIN_ATTACK             0
#define MAX_ATTACK             500   
#define MIN_RELEASE            10
#define MAX_RELEASE            2000
#define MIN_SUSTAIN            0.2
#define MAX_SUSTAIN            1.0
#define MIN_DECAY              0
#define MAX_DECAY              750
