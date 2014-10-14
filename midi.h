#ifndef __MIDI_H__
#define __MIDI_H__
#include <stdint.h>

struct header_chunk_type {
    uint32_t signature = 0x4D546864;  	// "MThd"
    uint32_t chunk_len = 0x00000006;
    uint16_t format;                  	// track_type
    uint16_t tracks;      				// track_nums
    uint16_t division;    				// Time delta units per quarter-note.
};

struct track_chunk_type {
    uint32_t signature = 0x4d54726b;    // "MTrk" = track
    uint32_t chunk_len;
    track_event_t *events;
};

typedef struct track_event {
    uint8_t delta;
    uint8_t midi_event;
    uint8_t meta_event;
    uint8_t sysex_event;
} track_event_t;

enum format {
    SINGLE_TRACK,
    MULTI_TRACK_SYNC,
    MULTI_TRACK_ASYNC
};

enum midi_event {
    NOTE_OFF            = 0x80,
    NOTE_ON             = 0x90,
    POLY_KEY_PRESS      = 0xa0,
    CONTROLLER_CHANGE   = 0xb0,
    PROGRAM_CHANGE      = 0xc0,
    CHANNEL_PRESSURE	= 0xd0,
    PITCH_BEND          = 0xe0,
    SYS_EX_MESSAGE      = 0xf0
};

enum meta_event {
    SEQ_NUMBER  	 	  = 0X00,
    TEXT_EVENT            = 0X01,
    COPYRIGHT_NOTICE 	  = 0X02,
    TRACK_NAME 			  = 0X03,
    INSTRUMENT_NAME 	  = 0X04,
    LYRIC_TEXT 			  = 0X05,
    MARKER_TEXT           = 0X06,
    CUE_POINT			  = 0X07,
    MIDI_CPA		  	  = 0X20,
    END_OF_TRACK 		  = 0X2f,
    TEMPO_SETTING 		  = 0X51,
    SMPTE_OFFSET		  = 0X54,
    TIME_SIG		   	  = 0X58,
    KEY_SIG               = 0X59,
    SEQ_SPECIFIC_EVENT    = 0X7f
};

enum nodes {
    C0, CH0, D0, DH0, E0, F0, FH0, G0, GH0, A0, AH0, B0,
    C1, CH1, D1, DH1, E1, F1, FH1, G1, GH1, A1, AH1, B1,
    C2, CH2, D2, DH2, E2, F2, FH2, G2, GH2, A2, AH2, B2,
    C3, CH3, D3, DH3, E3, F3, FH3, G3, GH3, A3, AH3, B3,
    C4, CH4, D4, DH4, E4, F4, FH4, G4, GH4, A4, AH4, B4,
    C5, CH5, D5, DH5, E5, F5, FH5, G5, GH5, A5, AH5, B5,
    C6, CH6, D6, DH6, E6, F6, FH6, G6, GH6, A6, AH6, B6,
    C7, CH7, D7, DH7, E7, F7, FH7, G7, GH7, A7, AH7, B7,
    C8, CH8, D8, DH8, E8, F8, FH8, G8, GH8, A8, AH8, B8,
    C9, CH9, D9, DH9, E9, F9, FH9, G9, GH9, A9, AH9, B9,
    C10, CH10, D10, DH10, E10, F10, FH10, G10
};

#endif
