// FUNC 8001f3fc 192 MAIN0
// MATCHING 8001f3fc 192
// Ported from psx_tomba (sound.c, queueSoundCommand); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
#define SOUND_QUEUE_SIZE 0x80
#define BGM_VOICE_MASK (SPU_00CH | SPU_01CH | SPU_02CH | SPU_03CH | SPU_04CH | SPU_05CH | SPU_06CH | SPU_07CH | SPU_08CH | SPU_09CH | SPU_10CH | SPU_11CH | SPU_12CH | SPU_13CH | SPU_14CH | SPU_15CH)
#define SFX_VOICE_MASK (SPU_16CH | SPU_17CH | SPU_18CH | SPU_19CH | SPU_20CH | SPU_21CH | SPU_22CH | SPU_23CH)

enum {
    SND_CMD_KEY_ON            = 0x0000,
    SND_CMD_KEY_OFF           = 0x1000,
    SND_CMD_OVERRIDE          = 0x8000,
    SND_CMD_BGM_FADE          = 0x9000,
    SND_CMD_PITCH_SLIDE       = 0xA000,
    SND_CMD_TYPE_MASK         = 0xF000,
    SND_CMD_PARAM_MASK        = 0x0F00,
    SND_CMD_VALUE_MASK        = 0x00FF,
    SND_SFX_ID_MASK           = 0x03FF,
    SND_KEYON_NOTE_OVERRIDE   = 0x0400,
    SND_KEYON_VOLUME_OVERRIDE = 0x0800,
    SND_OVERRIDE_NOTE         = 0x0000,
    SND_OVERRIDE_VOLUME       = 0x0100,
    SND_FADE_INTERVAL         = 0x0000,
    SND_FADE_STEP             = 0x0100,
    SND_FADE_TARGET           = 0x0200,
    SND_SLIDE_INTERVAL        = 0x0000,
    SND_SLIDE_STEP            = 0x0100,
    SND_SLIDE_DURATION        = 0x0200,
    SND_SLIDE_START           = 0x0300
};

typedef struct {
    u8 vab;
    u8 prog;
    u8 tone;
    u8 note;
    u8 volume;
    u8 priority;
    u8 pad[2];
} SfxDef;

SfxDef D_80077774[41] = {
    { 0, 2, 8, 40, 100, 0 },
    { 0, 0, 1, 1, 100, 0 },
    { 0, 0, 2, 2, 100, 0 },
    { 0, 0, 3, 3, 80, 0 },
    { 0, 0, 4, 4, 80, 0 },
    { 0, 0, 5, 5, 80, 0 },
    { 0, 0, 6, 6, 100, 0 },
    { 0, 0, 7, 7, 120, 0 },
    { 0, 0, 8, 8, 100, 0 },
    { 0, 0, 9, 9, 110, 0 },
    { 0, 0, 10, 10, 110, 0 },
    { 0, 0, 11, 11, 70, 0 },
    { 0, 0, 12, 12, 100, 0 },
    { 0, 0, 13, 13, 100, 0 },
    { 0, 0, 14, 14, 100, 0 },
    { 0, 0, 15, 15, 100, 0 },
    { 0, 1, 0, 16, 100, 0 },
    { 0, 1, 1, 17, 70, 0 },
    { 0, 1, 2, 18, 100, 0 },
    { 0, 1, 3, 19, 100, 0 },
    { 0, 1, 4, 20, 100, 0 },
    { 0, 1, 5, 21, 100, 0 },
    { 0, 1, 6, 22, 100, 0 },
    { 0, 1, 7, 23, 100, 0 },
    { 0, 1, 8, 24, 100, 0 },
    { 0, 1, 9, 25, 100, 0 },
    { 0, 1, 10, 26, 100, 0 },
    { 0, 1, 11, 27, 100, 0 },
    { 0, 1, 12, 28, 100, 0 },
    { 0, 1, 13, 29, 70, 0 },
    { 0, 1, 14, 30, 100, 0 },
    { 0, 1, 15, 31, 100, 0 },
    { 0, 2, 0, 32, 120, 0 },
    { 0, 2, 1, 33, 115, 0 },
    { 0, 2, 2, 34, 115, 0 },
    { 0, 2, 3, 35, 115, 0 },
    { 0, 2, 4, 36, 115, 0 },
    { 0, 2, 5, 37, 120, 0 },
    { 0, 2, 6, 38, 100, 0 },
    { 0, 1, 12, 27, 110, 0 },
    { 0, 2, 7, 39, 100, 0 }
};

SfxDef D_800778BC[1] = {
    { 4, 0, 0, 7, 120, 0 }
};

SfxDef D_800778C4[4] = {
    { 1, 0, 0, 0, 100, 0 },
    { 1, 0, 1, 1, 100, 0 },
    { 1, 0, 2, 2, 100, 0 },
    { 1, 0, 3, 3, 100, 0 }
};

extern s16 BGM_FADE_TIMER;
extern s16 BGM_FADE_INTERVAL;
extern s16 BGM_FADE_STEP;
extern s16 BGM_FADE_TARGET;
extern s16 PITCH_SLIDE_TIMER;
extern s16 PITCH_SLIDE_INTERVAL;
extern s16 PITCH_SLIDE_STEP;
extern s16 PITCH_SLIDE_DURATION;
extern s16 PITCH_SLIDE_VOICE;
extern int LAST_KEYON_VOICE;

typedef struct {
    u16 id;
    s16 arg;
} SfxQueueEntry;

typedef struct {
    u8 seq;
    u8 vab;
    u8 fadeIn;
    u8 unk3;
} BgmDef;

extern u16 AREA_BGM_VOLUME[];
extern s16 BGM_FADE_VOLUME;
extern SfxQueueEntry SOUND_QUEUE[];
extern BgmDef BGM_TRACK_DEFS[];
extern s16 BGM_VAB_ID;
extern u_long* SEQ_DATA[];
extern s16 BGM_TRACK_VOLUME[];
extern s16 JINGLE_VAB_ID;


extern s16 PITCH_SLIDE_VAB;
extern s16 PITCH_SLIDE_PROG;
extern s16 PITCH_SLIDE_OLD_NOTE;
extern s16 PITCH_SLIDE_OLD_FINE;
extern s16 PITCH_SLIDE_NEW_NOTE;
extern s16 PITCH_SLIDE_NEW_FINE;


s32 getSfxVabOffset(u16 id);





















extern BgmDef AREA_BGM_DEFS[];
extern u8* AREA_REVERB_DEPTH[];
extern s32 AREA_REVERB_MODE[];

s32 queueSoundCommand(cmd, param)
    u16 cmd;
    s32 param;
{
    s32 i;
    u16 idx = SOUND_QUEUE_HEAD;

    for (i = 0; i < SOUND_QUEUE_COUNT; i++) {
        if (SOUND_QUEUE[(s16)idx].id == cmd) {
            return -1;
        }
        idx = (idx + 1) & (SOUND_QUEUE_SIZE - 1);
    }
    SOUND_QUEUE[SOUND_QUEUE_TAIL].id = cmd;
    SOUND_QUEUE[SOUND_QUEUE_TAIL].arg = param;
    SOUND_QUEUE_TAIL = (SOUND_QUEUE_TAIL + 1) & (SOUND_QUEUE_SIZE - 1);
    SOUND_QUEUE_COUNT++;
    return 0;
}
