/** @file mid.h */

#ifndef __MID_H__
#define __MID_H__

#include <list/list.h>

#include <stdint.h>
#include <stdio.h>

#if defined(_WIN32)
#include <stddef.h>
#endif

#define HEADER_SIGNATURE 0x4D546864
#define HEADER_LENGTH    0x00000006
#define HEADER_BYTES     10
#define TRACK_SIGNATURE  0x4D54726b
#define TRACK_BYTES      8


#define FIRST_TRACK_POS  14
#define CHANNELS         16
#define INSTR_CLASSES    16
#define INSTR_PER_CLASS  8

/** Default tempo: 500,000 */
#define SET_TEMPO_DEFAULT 0x07a120

/**
 * MIDI format
 */
typedef enum
{
    SINGLE_TRACK,
    MULTI_TRACK_SYNC,
    MULTI_TRACK_ASYNC
} fmt_t;

/**
 * MIDI Messages
 *
 * See:
 * http://www.midi.org/techspecs/midimessages.php
 */
typedef enum
{
    BANK_SELECT         = 0x00, 
    MOD_WHEEL_LEVER     = 0x01,
    BREATH_CTRL         = 0x02,
    
    FOOT_CTRL           = 0x04,
    PORTAMENTO_TIME     = 0x05,
    DATA_ENTRY_MSB      = 0x06,
    CHANNEL_VOLUME      = 0x07,
    BALANCE             = 0x08,
    
    PAN                 = 0x0a,
    EXP_CTRL            = 0x0b,
    EFFECT_CONTROL_1    = 0x0c,
    EFFECT_CONTROL_2    = 0x0d,
                         
    GEN_PURPOSE_CTRL_1  = 0x10,
    GEN_PURPOSE_CTRL_2  = 0x11,
    GEN_PURPOSE_CTRL_3  = 0x12,
    GEN_PURPOSE_CTRL_4  = 0x13,
                         
    LSB_FOR_CTRL_0      = 0x20,
    LSB_FOR_CTRL_1      = 0x21,
    LSB_FOR_CTRL_2      = 0x22,
    LSB_FOR_CTRL_3      = 0x23,
    LSB_FOR_CTRL_4      = 0x24,
    LSB_FOR_CTRL_5      = 0x25,
    LSB_FOR_CTRL_6      = 0x26,
    LSB_FOR_CTRL_7      = 0x27,
    LSB_FOR_CTRL_8      = 0x28,
    LSB_FOR_CTRL_9      = 0x29,
    LSB_FOR_CTRL_10     = 0x2a,
    LSB_FOR_CTRL_11     = 0x2b,
    LSB_FOR_CTRL_12     = 0x2c,
    LSB_FOR_CTRL_13     = 0x2d,
    LSB_FOR_CTRL_14     = 0x2e,
    LSB_FOR_CTRL_15     = 0x2f,
    LSB_FOR_CTRL_16     = 0x30,
    LSB_FOR_CTRL_17     = 0x31,
    LSB_FOR_CTRL_18     = 0x32,
    LSB_FOR_CTRL_19     = 0x33,
                         
    DAMPER_PEDAL        = 0x40,
    PORTAMENTO          = 0x41,
    SOSTENUTO           = 0x42,
    SOFT_PEDAL          = 0x43,
    LEGATO_FOOTSWITCH   = 0x44,
    HOLD_2              = 0x45,
    SOUND_CTRL_1        = 0x46,
    SOUND_CTRL_2        = 0x47,
    SOUND_CTRL_3        = 0x48,
    SOUND_CTRL_4        = 0x49,
    SOUND_CTRL_5        = 0x4a,
    SOUND_CTRL_6        = 0x4b,
    SOUND_CTRL_7        = 0x4c,
    SOUND_CTRL_8        = 0x4d,
    SOUND_CTRL_9        = 0x4e,
    SOUND_CTRL_10       = 0x4f,
    GEN_PURPOSE_CTRL_5  = 0x50,
    GEN_PURPOSE_CTRL_6  = 0x51,
    GEN_PURPOSE_CTRL_7  = 0x52,
    GEN_PURPOSE_CTRL_8  = 0x53,
    PORTAMENTO_CTRL     = 0x54,
                         
    HIGH_RESO_VELO_PRE  = 0x58,
                         
    EFFECTS_1_DEPTH     = 0x5b,
    EFFECTS_2_DEPTH     = 0x5c,
    EFFECTS_3_DEPTH     = 0x5d,
    EFFECTS_4_DEPTH     = 0x5e,
    EFFECTS_5_DEPTH     = 0x5f,
    DATA_INC            = 0x60,
    DATA_DEC            = 0x61,
    NREG_PARA_NUM_LSB   = 0x62,
    NREG_PARA_NUM_MSB   = 0x63,
    REG_PARA_NUM_LSB    = 0x64,
    REG_PARA_NUM_MSB    = 0x65,
                         
    ALL_SOUND_OFF       = 0x78,
    RESET_ALL_CTRLS     = 0x79,
    LOCAL_CONTROL       = 0x7a,
    ALL_NOTES_OFF       = 0x7b,
    OMNI_MODE_OFF       = 0x7c,
    OMNI_MODE_ON        = 0x7d,
    MONO_MODE_ON        = 0x7e,
    POLY_MODE_ON        = 0x7f,
    
    NOTE_OFF            = 0x80, /**< Channel message: Sent when note is released */
    NOTE_ON             = 0x90, /**< Channel message: Sent when note is pressed */
    POLY_AFT            = 0xa0, /**< Channel message: Key pressure value*/
    CTRL_MODE           = 0xb0, /**< Channel message: Sent when controller changes (pedals and levers etc.)*/
    PRG_CHANGE          = 0xc0, /**< Channel message: Sent when patch number changes*/
    CHAN_AFT            = 0xd0, /**< Channel message: Channel pressure. Different from Key pressure. Use to get single greatest pressure value of all keys*/
    PITCH_BEND          = 0xe0, /**< Channel message: Sent to indicate pitch bend change(wheel or lever)*/
    
    SYSEX_START         = 0xf0, /**< Messaging to midi hardware - start with 0xf0*/
    TIME_CODE           = 0xf1, /**< Time code*/
    SONG_POS_PTR        = 0xf2, /**< Song position pointer*/
    SONG_SELECT         = 0xf3, /**< Song Select*/
    FUNC_UNDEF_1        = 0xf4, /**< Function undefined 1*/
    FUNC_UNDEF_2        = 0xf5, /**< Function undefined 2*/
    TUNE_REQ            = 0xf6, /**< Tune requirement*/
    SYSEX_END           = 0xf7, /**< Messaging to midi hardware - End with 0xf7*/
    TIMING_CLOCK        = 0xf8, /**< Timing clock*/
    FUNC_UNDEF_3        = 0xf9, /**< Function undefined 3*/
    FUNC_START          = 0xfa, /**< Function start*/
    FUNC_CONTINUE       = 0xfb, /**< Function continue*/
    FUNC_STOP           = 0xfc, /**< Function stop*/
    FUNC_UNDEF_4        = 0xfd, /**< Function undefined 4*/
    ACTIVE_SENSING      = 0xfe, /**< Active sensing*/
    SYS_RESET           = 0xff, /**< system reset*/
    META_MSG            = 0xff  /**< Meta message*/
} msg_t;

/**
 * Meta Message
 *
 * See:
 * http://somascape.org/midi/tech/mfile.html#meta
 */
typedef enum
{
    SEQ_NUMBER          = 0x00, /**< 16-bit value specifying the sequence number, is optional, should occur at time = 0, and prior to any MIDI events */
    TEXT_EVENT          = 0x01, /**< This event is optional, and is used to include comments and other user information */
    COPYRIGHT_NOTICE    = 0x02, /**< Contains a copyright message comprising the characters '(C)' along with the year and owner of the copyright, optional */
    TRACK_NAME          = 0x03, /**< If it occurs in the first track of a format 0 or 1 MIDI file, then it gives the Sequence Name, otherwise gives Track Name */
    INSTRUMENT_NAME     = 0x04, /**< Optional event to provide textual clue regarding instrumentation. Recommended to be at the start of track */
    LYRIC_TEXT          = 0x05, /**< Lyric of song. Placed appropriately throughout track */
    MARKER_TEXT         = 0x06, /**< Used to mark points in a song such as verses */
    CUE_POINT           = 0x07, /**< Optional event is used to describe something that happens within a film, video or stage production at that point in the musical score. Eg 'Car crashes', 'Door opens', etc. */
    MIDI_CPA            = 0x20, /**< Optional event is used to associate any subsequent SysEx and Meta events with a particular MIDI channel, and will remain in effect until the next MIDI Channel Prefix Meta event or the next MIDI event. */
    END_OF_TRACK        = 0x2F, /**< Mandatory event must be the last event in each MTrk chunk, and that should be the only occurrence per track. */
    SET_TEMPO           = 0x51, /**< 24-bit value specifying the tempo as the number of microseconds per quarter note, Specifying tempos as time per beat. */
    SMPTE_OFFSET        = 0x54, /**< Optional event, if present, should occur at the start of a track, at time = 0, and prior to any MIDI events. It is used to specify the SMPTE time at which the track is to start */
    TIME_SIG            = 0x58, /**< There should generally be a Time Signature Meta event at the beginning of a track (at time = 0), otherwise a default 4/4 time signature will be assumed */
    KEY_SIG             = 0x59, /**< Specifies key signatures */
    SEQ_SPECIFIC_EVENT  = 0x7f  /**< The first 1 or 3 bytes of data is a manufacturer's ID code */
} meta_t;

/**
 * MIDI node
 */
typedef enum
{
    C0,  CH0,  D0,  DH0,  E0,  F0,  FH0,  G0,  GH0,  A0,  AH0,  B0,
    C1,  CH1,  D1,  DH1,  E1,  F1,  FH1,  G1,  GH1,  A1,  AH1,  B1,
    C2,  CH2,  D2,  DH2,  E2,  F2,  FH2,  G2,  GH2,  A2,  AH2,  B2,
    C3,  CH3,  D3,  DH3,  E3,  F3,  FH3,  G3,  GH3,  A3,  AH3,  B3,
    C4,  CH4,  D4,  DH4,  E4,  F4,  FH4,  G4,  GH4,  A4,  AH4,  B4,
    C5,  CH5,  D5,  DH5,  E5,  F5,  FH5,  G5,  GH5,  A5,  AH5,  B5,
    C6,  CH6,  D6,  DH6,  E6,  F6,  FH6,  G6,  GH6,  A6,  AH6,  B6,
    C7,  CH7,  D7,  DH7,  E7,  F7,  FH7,  G7,  GH7,  A7,  AH7,  B7,
    C8,  CH8,  D8,  DH8,  E8,  F8,  FH8,  G8,  GH8,  A8,  AH8,  B8,
    C9,  CH9,  D9,  DH9,  E9,  F9,  FH9,  G9,  GH9,  A9,  AH9,  B9,
    C10, CH10, D10, DH10, E10, F10, FH10, G10
} node_t;

/**
 * MIDI instrument
 *
 * See:
 * http://www.midi.org/techspecs/gm1sound.php
 */
typedef enum
{
    ACOUSTIC_GRAND_PIANO,
    BRIGHT_ACOUSTIC_PIANO,
    ELECTRIC_GRAND_PIANO,
    HONKY_TONK_PIANO,
    ELECTRIC_PIANO_1,
    ELECTRIC_PIANO_2,
    HARPSICHORD,
    CLAVI,
    CELESTA,
    GLOCKENSPIEL,
    MUSIC_BOX,
    VIBRAPHONE,
    MARIMBA,
    XYLOPHONE,
    TUBULAR_BELLS,
    DULCIMER,
    DRAWBAR_ORGAN,
    PERCUSSIVE_ORGAN,
    ROCK_ORGAN,
    CHURCH_ORGAN,
    REED_ORGAN,
    ACCORDION,
    HARMONICA,
    TANGO_ACCORDION,
    ACOUSTIC_GUITAR_NYLON,
    ACOUSTIC_GUITAR_STEEL,
    ELECTRIC_GUITAR_JAZZ,
    ELECTRIC_GUITAR_CLEAN,
    ELECTRIC_GUITAR_MUTED,
    OVERDRIVEN_GUITAR,
    DISTORTION_GUITAR,
    GUITAR_HARMONICS,
    ACOUSTIC_BASS,
    ELECTRIC_BASS_FINGER,
    ELECTRIC_BASS_PICK,
    FRETLESS_BASS,
    SLAP_BASS_1,
    SLAP_BASS_2,
    SYNTH_BASS_1,
    SYNTH_BASS_2,
    VIOLIN,
    VIOLA,
    CELLO,
    CONTRABASS,
    TREMOLO_STRINGS,
    PIZZICATO_STRINGS,
    ORCHESTRAL_HARP,
    TIMPANI,
    STRING_ENSEMBLE_1,
    STRING_ENSEMBLE_2,
    SYNTHSTRINGS_1,
    SYNTHSTRINGS_2,
    CHOIR_AAHS,
    VOICE_OOHS,
    SYNTH_VOICE,
    ORCHESTRA_HIT,
    TRUMPET,
    TROMBONE,
    TUBA,
    MUTED_TRUMPET,
    FRENCH_HORN,
    BRASS_SECTION,
    SYNTHBRASS_1,
    SYNTHBRASS_2,
    SOPRANO_SAX,
    ALTO_SAX,
    TENOR_SAX,
    BARITONE_SAX,
    OBOE,
    ENGLISH_HORN,
    BASSOON,
    CLARINET,
    PICCOLO,
    FLUTE,
    RECORDER,
    PAN_FLUTE,
    BLOWN_BOTTLE,
    SHAKUHACHI,
    WHISTLE,
    OCARINA,
    LEAD_1_SQUARE,
    LEAD_2_SAWTOOTH,
    LEAD_3_CALLIOPE,
    LEAD_4_CHIFF,
    LEAD_5_CHARANG,
    LEAD_6_VOICE,
    LEAD_7_FIFTHS,
    LEAD_8_BASS_LEAD,
    PAD_1_NEW_AGE,
    PAD_2_WARM,
    PAD_3_POLYSYNTH,
    PAD_4_CHOIR,
    PAD_5_BOWED,
    PAD_6_METALLIC,
    PAD_7_HALO,
    PAD_8_SWEEP,
    FX_1_RAIN,
    FX_2_SOUNDTRACK,
    FX_3_CRYSTAL,
    FX_4_ATMOSPHERE,
    FX_5_BRIGHTNESS,
    FX_6_GOBLINS,
    FX_7_ECHOES,
    FX_8_SCI_FI,
    SITAR,
    BANJO,
    SHAMISEN,
    KOTO,
    KALIMBA,
    BAG_PIPE,
    FIDDLE,
    SHANAI,
    TINKLE_BELL,
    AGOGO,
    STEEL_DRUMS,
    WOODBLOCK,
    TAIKO_DRUM,
    MELODIC_TOM,
    SYNTH_DRUM,
    REVERSE_CYMBAL,
    GUITAR_FRET_NOISE,
    BREATH_NOISE,
    SEASHORE,
    BIRD_TWEET,
    TELEPHONE_RING,
    HELICOPTER,
    APPLAUSE,
    GUNSHOT
} instr_t;

typedef enum
{
    PIANO                   = 0x00,
    CHROMATIC_PERCUSSION    = 0x01,
    ORGAN                   = 0x02,
    GUITAR                  = 0x03,
    BASS                    = 0x04,
    STRINGS                 = 0x05,
    ENSEMBLE                = 0x06,
    BRASS                   = 0x07,
    REED                    = 0x08,
    PIPE                    = 0x09,
    SYNTH_LEAD              = 0x0A,
    SYNTH_PAD               = 0x0B,
    SYNTH_EFFECT            = 0x0C,
    ETHNIC                  = 0x0D,
    PERCUSSION              = 0x0E,
    SOUND_EFFECTS           = 0x0F
} instr_class_t;


typedef struct event_s
{
    msg_t   msg;      /**< Event message              */
    uint8_t chan;     /**< Event channel              */
    uint8_t byte_1;   /**< First data byte            */
    uint8_t byte_2;   /**< Second data byte           */
    uint32_t delta;    /**< Delta ticks                */
    double time;    /**< Total ticks                */
    list_t *data;     /**< List for sysex/meta data   */
} event_t;

typedef struct track_s
{
    uint32_t bytes;  /**< Track length in bytes     */
    list_t *events;  /**< Events list               */
} track_t;

typedef struct mid_s
{
    fmt_t format;       /**< Midi format       */
    uint16_t division;  /**< Time division     */
    list_t *tracks;     /**< Tracks list       */
} mid_t;


mid_t *read_mid(FILE *file);
int read_tracks(list_t *data, uint16_t division, list_t *tracks);
int read_events(list_t *data, uint16_t division, uint32_t start_tempo, list_t *events);

uint32_t find_start_tempo(uint8_t *data, uint32_t bytes);
uint32_t count_events(uint8_t *data, uint32_t len);

mid_t *merge_tracks(mid_t *mid);

void write_mid(FILE *file, mid_t *mid);

void free_mid(mid_t *mid);

#endif
