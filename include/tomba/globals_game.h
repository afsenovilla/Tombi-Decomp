#ifndef _TOMBA_GLOBALS_GAME_H
#define _TOMBA_GLOBALS_GAME_H

/* The other named MAIN0 globals, typed with the game.h structs (included at the end of game.h, after globals.h).
   Sources that read them through their own struct views declare them locally. Sorted by address. */

/* TODO: LZ_FILE_CTRL 0x1F800070 needs a struct type (structs.h) */
extern u8  LOAD_COMPLETE_SYM;                  /* 0x1F8001CE */
extern Task* TASK_C;                           /* 0x1F8001D4 */
extern s32  LOAD_NEXT_ADDR;                    /* 0x1F800298 */
extern s32 LOAD_SLOTS[];                       /* 0x1F8002A4 */
extern int SPR_DATA[];                         /* 0x1F8002C8 */
extern int WMD_DATA[];                         /* 0x1F800330 */
extern int WPP_DATA[];                         /* 0x1F800348 */
extern int APD_DATA[];                         /* 0x1F800358 */
extern int WFM3_DATA[];                        /* 0x1F800398 */
extern s16  VAB_IDS[];                         /* 0x1F8003A8 */
extern s32  SFX_BANKS[];                       /* 0x800782CC */
/* TODO: AREA_BGM_DEFS 0x800784C8 needs a struct type (structs.h) */
extern int  SPU_BANK_ADDRS[];                  /* 0x80078530 */
extern fileLink FILE_LINKS[];                  /* 0x8007DEF0 */
extern int  FILE_LINK_SIZES[];                 /* 0x8007DEF4 */
extern int MOVIE_DEC_IMGID;                    /* 0x8009BCBC */
extern int MOVIE_DEC_RECT_X;                   /* 0x8009BCC0 */
extern int MOVIE_DEC_RECT_W;                   /* 0x8009BCC4 */
extern s16 PITCH_SLIDE_VOICE;                  /* 0x8009BD40 */
extern s16 BGM_VAB_ID;                         /* 0x8009C8F0 */
extern u8   SCRIPT_FLAGS[];                    /* 0x8009CDA4 */
extern u8 TALK_POSE;                           /* 0x8009D00F */
extern u8*  SCRIPT_CODE;                       /* 0x8009D60C */
extern u_long* SEQ_DATA[];                     /* 0x8009D630 */
extern s16  SFX_VOLUME_OVERRIDE;               /* 0x8009D690 */
extern ScriptContext* SCRIPT_CTX;              /* 0x8009F0F0 */
extern u8*  SCRIPT_OBJECTS[];                  /* 0x8009F2D8 */
extern int  CD_QUEUE[];                        /* 0x8009F3E0 */
extern int  CD_QUEUE_DST[];                    /* 0x8009F3E4 */
extern char* SPRINTF_BUFFER_MSG[];             /* 0x800A2220 */
extern s16  SFX_NOTE_OVERRIDE;                 /* 0x800A34B0 */
extern short VOICE_SFX_ID[];                   /* 0x800A3CC8 */
/* TODO: SOUND_QUEUE 0x800A3D00 needs a struct type (structs.h) */
extern byte LOAD_BUFFER[0x3D4];                /* 0x800A3FE0 */
/* TODO: MESSAGE_SLOTS 0x800A4648 needs a struct type (structs.h) */
/* TODO: MESSAGE_GLYPHS 0x800A4DE0 needs a struct type (structs.h) */
/* TODO: GLYPH_CACHE 0x800A5DE0 needs a struct type (structs.h) */
extern u_char PLAYER[0x178];                   /* 0x800A6038 */
extern SpriteSlot SPRITE_SLOTS[];              /* 0x800B0BB8 */

#endif
