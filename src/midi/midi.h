#ifndef __MIDI_H__
#define __MIDI_H__

#include <stdint.h>
#include <stdio.h>

#if defined(_WIN32)
#include <stddef.h>
#endif

#define HEADER_SIGNATURE 0x4D546864
#define HEADER_LENGTH    0x00000006
#define TRACK_SIGNATURE  0x4D54726b

#define FIRST_TRACK_POS  14

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
    NOTE_OFF_1          = 0x80,
    NOTE_OFF_2          = 0x81,
    NOTE_OFF_3          = 0x82,
    NOTE_OFF_4          = 0x83,
    NOTE_OFF_5          = 0x84,
    NOTE_OFF_6          = 0x85,
    NOTE_OFF_7          = 0x86,
    NOTE_OFF_8          = 0x87,
    NOTE_OFF_9          = 0x88,
    NOTE_OFF_10         = 0x89,
    NOTE_OFF_11         = 0x8a,
    NOTE_OFF_12         = 0x8b,
    NOTE_OFF_13         = 0x8c,
    NOTE_OFF_14         = 0x8d,
    NOTE_OFF_15         = 0x8e,
    NOTE_OFF_16         = 0x8f,
    NOTE_ON_1           = 0x90,
    NOTE_ON_2           = 0x91,
    NOTE_ON_3           = 0x92,
    NOTE_ON_4           = 0x93,
    NOTE_ON_5           = 0x94,
    NOTE_ON_6           = 0x95,
    NOTE_ON_7           = 0x96,
    NOTE_ON_8           = 0x97,
    NOTE_ON_9           = 0x98,
    NOTE_ON_10          = 0x99,
    NOTE_ON_11          = 0x9a,
    NOTE_ON_12          = 0x9b,
    NOTE_ON_13          = 0x9c,
    NOTE_ON_14          = 0x9d,
    NOTE_ON_15          = 0x9e,
    NOTE_ON_16          = 0x9f,
    POLY_AFT_1          = 0xa0,
    POLY_AFT_2          = 0xa1,
    POLY_AFT_3          = 0xa2,
    POLY_AFT_4          = 0xa3,
    POLY_AFT_5          = 0xa4,
    POLY_AFT_6          = 0xa5,
    POLY_AFT_7          = 0xa6,
    POLY_AFT_8          = 0xa7,
    POLY_AFT_9          = 0xa8,
    POLY_AFT_10         = 0xa9,
    POLY_AFT_11         = 0xaa,
    POLY_AFT_12         = 0xab,
    POLY_AFT_13         = 0xac,
    POLY_AFT_14         = 0xad,
    POLY_AFT_15         = 0xae,
    POLY_AFT_16         = 0xaf,
    CTRL_MODE_1         = 0xb0,
    CTRL_MODE_2         = 0xb1,
    CTRL_MODE_3         = 0xb2,
    CTRL_MODE_4         = 0xb3,
    CTRL_MODE_5         = 0xb4,
    CTRL_MODE_6         = 0xb5,
    CTRL_MODE_7         = 0xb6,
    CTRL_MODE_8         = 0xb7,
    CTRL_MODE_9         = 0xb8,
    CTRL_MODE_10        = 0xb9,
    CTRL_MODE_11        = 0xba,
    CTRL_MODE_12        = 0xbb,
    CTRL_MODE_13        = 0xbc,
    CTRL_MODE_14        = 0xbd,
    CTRL_MODE_15        = 0xbe,
    CTRL_MODE_16        = 0xbf,
    PRG_CHANGE_1        = 0xc0,
    PRG_CHANGE_2        = 0xc1,
    PRG_CHANGE_3        = 0xc2,
    PRG_CHANGE_4        = 0xc3,
    PRG_CHANGE_5        = 0xc4,
    PRG_CHANGE_6        = 0xc5,
    PRG_CHANGE_7        = 0xc6,
    PRG_CHANGE_8        = 0xc7,
    PRG_CHANGE_9        = 0xc8,
    PRG_CHANGE_10       = 0xc9,
    PRG_CHANGE_11       = 0xca,
    PRG_CHANGE_12       = 0xcb,
    PRG_CHANGE_13       = 0xcc,
    PRG_CHANGE_14       = 0xcd,
    PRG_CHANGE_15       = 0xce,
    PRG_CHANGE_16       = 0xcf,
    CHAN_AFT_1          = 0xd0,
    CHAN_AFT_2          = 0xd1,
    CHAN_AFT_3          = 0xd2,
    CHAN_AFT_4          = 0xd3,
    CHAN_AFT_5          = 0xd4,
    CHAN_AFT_6          = 0xd5,
    CHAN_AFT_7          = 0xd6,
    CHAN_AFT_8          = 0xd7,
    CHAN_AFT_9          = 0xd8,
    CHAN_AFT_10         = 0xd9,
    CHAN_AFT_11         = 0xda,
    CHAN_AFT_12         = 0xdb,
    CHAN_AFT_13         = 0xdc,
    CHAN_AFT_14         = 0xdd,
    CHAN_AFT_15         = 0xde,
    CHAN_AFT_16         = 0xdf,
    PITCH_BEND_1        = 0xe0,
    PITCH_BEND_2        = 0xe1,
    PITCH_BEND_3        = 0xe2,
    PITCH_BEND_4        = 0xe3,
    PITCH_BEND_5        = 0xe4,
    PITCH_BEND_6        = 0xe5,
    PITCH_BEND_7        = 0xe6,
    PITCH_BEND_8        = 0xe7,
    PITCH_BEND_9        = 0xe8,
    PITCH_BEND_10       = 0xe9,
    PITCH_BEND_11       = 0xea,
    PITCH_BEND_12       = 0xeb,
    PITCH_BEND_13       = 0xec,
    PITCH_BEND_14       = 0xed,
    PITCH_BEND_15       = 0xee,
    PITCH_BEND_16       = 0xef,
    SYS_EXCUSIVE        = 0xf0,
    TIME_CODE           = 0xf1,
    SONG_POS_PTR        = 0xf2,
    SONG_SELECT         = 0xf3,
    FUNC_UNDEF_1        = 0xf4,
    FUNC_UNDEF_2        = 0xf5,
    TUNE_REQ            = 0xf6,
    END_SYSEX           = 0xf7,
    TIMING_CLOCK        = 0xf8,
    FUNC_UNDEF_3        = 0xf9,
    FUNC_START          = 0xfa,
    FUNC_CONTINUE       = 0xfb,
    FUNC_STOP           = 0xfc,
    FUNC_UNDEF_4        = 0xfd,
    ACTIVE_SENSING      = 0xfe,
    SYS_RESET           = 0xff,
    META_MSG            = 0xff
} msg_t;

/**
 * Meta Message
 *
 * See:
 * http://somascape.org/midi/tech/mfile.html#meta
 */
enum
{
    SEQ_NUMBER          = 0x00,
    TEXT_EVENT          = 0x01,
    COPYRIGHT_NOTICE    = 0x02,
    TRACK_NAME          = 0x03,
    INSTRUMENT_NAME     = 0x04,
    LYRIC_TEXT          = 0x05,
    MARKER_TEXT         = 0x06,
    CUE_POINT           = 0x07,
    MIDI_CPA            = 0x20,
    END_OF_TRACK        = 0x2F,
    TEMPO_SETTING       = 0x51,
    SMPTE_OFFSET        = 0x54,
    TIME_SIG            = 0x58,
    KEY_SIG             = 0x59,
    SEQ_SPECIFIC_EVENT  = 0x7f
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
 */
enum
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

typedef struct event_s
{
    msg_t   msg;      /**< Event type                 */
    uint8_t para_1;   /**< First parameter            */
    uint8_t para_2;   /**< Second parameter           */
    uint8_t delta;    /**< Delta time                 */
    uint8_t *mdata;   /**< Pointer to meta event data */
} event_t;

typedef struct track_s
{
    uint32_t len;    /**< Track length in bytes     */
    uint32_t events; /**< Number of events in track */
    event_t *event;  /**< Pointer to events         */
} track_t;

typedef struct mid_s
{
    fmt_t format;       /**< Midi format       */
    uint16_t division;  /**< Time division     */
    uint16_t tracks;    /**< Number of tracks  */
    track_t *track;     /**< Pointer to tracks */
} mid_t;

uintptr_t *ffread(FILE *file, long int offset ,size_t buf_size);

int count_events(uint8_t *data, uint32_t len);

mid_t *read_mid(FILE *file);
track_t *read_tracks(FILE *file, uint16_t num);
event_t *read_events(uint8_t *data, uint16_t num);

void write_mid(FILE *file, mid_t mid);
void free_mid(mid_t *mid);

#endif
