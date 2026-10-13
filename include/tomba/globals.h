#ifndef _TOMBA_GLOBALS_H
#define _TOMBA_GLOBALS_H

/* MAIN0 globals named in notes/data_names_main0.csv (names after psx_tomba / US, addresses checked on the PAL
   binary by tools/tomba_data.py): one declaration each, spelled with plain C types only, so any source can
   include it (tobj.h, raw7.h and game.h do). Only the globals every source declares with one and the same type are
   here; the ones with per-subsystem struct views are in globals_game.h (game.h). Sorted by address. */

extern unsigned char LOADED_LIST_FILE_SYM;     /* 0x1F8003D2 */
extern short AREA_BGM_VOLUME[];                /* 0x8007833C */
extern unsigned char *AREA_REVERB_DEPTH[];     /* 0x80078428 */
extern int AREA_REVERB_MODE[];                 /* 0x80078478 */
extern char SOUND_INITIALIZED;                 /* 0x80078788 */
extern int LDSYS_LOAD_LIST;                    /* 0x8007C5F8 */
extern int LDAR_LOAD_LISTS[];                  /* 0x8007C5FC */
extern int SOUND_SET_LOAD_LISTS[];             /* 0x8007D1B4 */
extern short MOVIE_FILE_IDS[];                 /* 0x8007D284 */
extern int WSS_LOAD_LISTS[];                   /* 0x8007D430 */
extern int *AREA_LOAD_LISTS[];                 /* 0x8007DEA0 */
extern int AP_TABLE[];                         /* 0x80080504 */
extern unsigned char EVENT_STARTED_AP_TABLE[]; /* 0x80080524 */
extern unsigned char EVENT_COMPLETE_AP_TABLE[]; /* 0x800805EC */
extern int SYSTEM_LOAD_LISTS[];                /* 0x8009A930 */
extern int LAST_KEYON_VOICE;                   /* 0x8009BD08 */
extern short BGM_MUTED;                        /* 0x8009BD0C */
extern short BGM_VOLUME;                       /* 0x8009BD10 */
extern short BGM_FADE_ACTIVE;                  /* 0x8009BD14 */
extern short BGM_FADE_TIMER;                   /* 0x8009BD18 */
extern short BGM_FADE_INTERVAL;                /* 0x8009BD1C */
extern short BGM_FADE_STEP;                    /* 0x8009BD20 */
extern short BGM_FADE_VOLUME;                  /* 0x8009BD24 */
extern short BGM_FADE_TARGET;                  /* 0x8009BD28 */
extern short PITCH_SLIDE_ACTIVE;               /* 0x8009BD2C */
extern short PITCH_SLIDE_TIMER;                /* 0x8009BD30 */
extern short PITCH_SLIDE_INTERVAL;             /* 0x8009BD34 */
extern short PITCH_SLIDE_STEP;                 /* 0x8009BD38 */
extern short PITCH_SLIDE_DURATION;             /* 0x8009BD3C */
extern short PITCH_SLIDE_VAB;                  /* 0x8009BD44 */
extern short PITCH_SLIDE_PROG;                 /* 0x8009BD48 */
extern short PITCH_SLIDE_OLD_NOTE;             /* 0x8009BD4C */
extern short PITCH_SLIDE_OLD_FINE;             /* 0x8009BD50 */
extern short PITCH_SLIDE_NEW_NOTE;             /* 0x8009BD54 */
extern short PITCH_SLIDE_NEW_FINE;             /* 0x8009BD58 */
extern unsigned char LZ_CURRENT_BIT;           /* 0x8009BD60 */
extern unsigned short LZ_BITMASK;              /* 0x8009BD64 */
extern short VOICE_PRIORITY[];                 /* 0x8009C8C0 */
extern unsigned char SCRIPT_GLOBALS[];         /* 0x8009CEA4 */
extern char *WVD_BODIES[];                     /* 0x8009D2F0 */
extern char *WVD_HEADERS[];                    /* 0x8009D3F0 */
extern short SOUND_QUEUE_COUNT;                /* 0x8009D688 */
extern char OT_FRAMEBUFFER[];                  /* 0x8009D6A8 */
extern short JINGLE_SEQ_ID;                    /* 0x8009F0C8 */
extern unsigned char VOICE_KEY_STATUS[];       /* 0x8009F0D0 */
extern long MEMCARD_HW_END_IO;                 /* 0x8009F0F4 */
extern long MEMCARD_HW_END_ERROR;              /* 0x8009F0F8 */
extern long MEMCARD_HW_TIMEOUT;                /* 0x8009F0FC */
extern long MEMCARD_HW_NEW_DEVICE;             /* 0x8009F100 */
extern long MEMCARD_SW_END_IO;                 /* 0x8009F10C */
extern long MEMCARD_SW_END_ERROR;              /* 0x8009F110 */
extern long MEMCARD_SW_TIMEOUT;                /* 0x8009F114 */
extern long MEMCARD_SW_NEW_DEVICE;             /* 0x8009F118 */
extern short SOUND_QUEUE_TAIL;                 /* 0x8009F2D0 */
extern short BGM_SEQ_ID;                       /* 0x800A3428 */
extern short JINGLE_VAB_ID;                    /* 0x800A3CB8 */
extern short SOUND_QUEUE_HEAD;                 /* 0x800A3F90 */
extern char LOAD_BUFFER_ALT[];                 /* 0x800B3E28 */

#endif
