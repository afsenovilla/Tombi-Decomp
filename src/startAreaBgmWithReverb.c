// FUNC 8001eb84 36 MAIN0
// HEADERS tomba24
// MATCHING 8001eb84 36
// Portado de psx_tomba (main/audio/sound.c, startAreaBgmWithReverb); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"
#include "psyq/libspu.h"
#include "psyq/libsnd.h"

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

extern u_long* SEQ_DATA[];

void updateSound(void);

s32 keyOnQueuedSfx(s32 cmd);

static inline s32 getSfxVabOffset(u16 arg0)
{
    u8 a = D_800778E4[arg0 * 2];
    u8 b = D_800778E5[arg0 * 2];

    return SFX_BANKS[a] + b * sizeof(SfxDef);
}

static inline s16 keyOffSfxVoice(s16 voice)
{
    u16 i = voice;

    if (VOICE_KEY_STATUS[i] != 0) {
        VOICE_PRIORITY[i] = 0xF;
        return SsUtKeyOffV(voice);
    }
    return -1;
}

s32 setSfxOverride(s32 cmd);

s32 setBgmFadeParam(s32 cmd);

s32 setPitchSlideParam(s32 cmd);

s32 allocSfxVoice();

s32 playSFX(s32 arg);

s32 playSFXWithNote(s32 id, s32 note);

s32 playSFXWithVolume(s32 id, s32 volume);

s32 playSFXWithNoteAndVolume(s32 id, s32 note, s32 volume);

s32 playSFXWithPitchSlide(s32 id, s32 interval, s32 step, s32 duration);

s16 keyOnSFX(s32 id);

s16 playSFXAndSetNote(s32 id, s16 note);

void queueSfxKeyOff(s32 voice);

void keyOffActiveSfxVoices(void);

void startAreaBgm(void);

void startAreaBgmWithReverb(s16 reverbDepth)
{
    openAreaBgm(reverbDepth);
}

static inline s32 adjustAreaBgmVolume(s32 id)
{
    switch (GAME.selectedArea) {
    case 0:
    case 2:
        id += 5;
        break;
    case 6:
        switch (D_8009BCCA) {
        case 0:
            id -= 5;
            break;
        case 1:
            id += 0xC;
            break;
        case 2:
            id += 0xC;
            break;
        }
        break;
    case 9:
        switch (D_8009BCCA) {
        case 0:
            id -= 0xB;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            id += 0xA;
            break;
        }
        break;
    }
    return id;
}

s32 openAreaBgm(s32 reverbDepth);

s32 startBgmTrack(s32 id);

void stopBgm(s32 fadeOut);

void restoreAreaBgmVolume(void);

s32 playJingle(s32 seqIndex);

s32 queueSoundCommand();

void stopAllSound(void);

void setReverbMode(s32 reverbMode, s16 reverbDepth);

void muteBgm(void);

void unmuteBgm(void);

void keyOffSfxAll(void);

void shutdownSound(void);

void sndQuit(void);
