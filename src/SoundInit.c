// FUNC 8001f6b4 364 MAIN0
// MATCHING 8001f6b4 364
#include "globals.h"
typedef struct { int mask; short pad0; short pad1; short mvl, mvr; short pad2[4]; short cdvl, cdvr; short pad3[2]; int cdmix; } SC;
extern void SoundShutdown(void);
extern void FUN_8006c544(void);
extern void FUN_800702c0(void *, int, int);
extern void FUN_800704a0(int);
extern void FUN_800745a8(int);
extern void FUN_80077064(int, int);
extern void FUN_800745dc(int);
extern void FUN_80077404(void *);
extern void FUN_8006eed8(void);
extern char DAT_800a2270[];
extern int DAT_8009bce0;
extern short DAT_8009bce4, DAT_8009bce6, DAT_8009bcf0, DAT_8009bcf2;
extern int DAT_8009bcf8;
extern short VOICE_SFX_ID[];
extern short VAB_IDS[];

void SoundInit(void)
{
    int i;
    short m;
    int *s;
    short *q, *p;
    if (SOUND_INITIALIZED != 0)
        SoundShutdown();
    FUN_8006c544();
    FUN_800702c0(DAT_800a2270, 4, 1);
    FUN_800704a0(1);
    FUN_800745a8(0);
    FUN_80077064(0, 0xffffff);
    FUN_800745dc(0x10);
    s = &DAT_8009bce0;
    *s = 0x2c3;
    DAT_8009bce4 = 0x3fff;
    DAT_8009bce6 = 0x3fff;
    DAT_8009bcf0 = 0x7fff;
    DAT_8009bcf2 = 0x7fff;
    DAT_8009bcf8 = 1;
    FUN_80077404(s);
    FUN_8006eed8();
    i = 0;
    SOUND_QUEUE_TAIL = 0;
    SOUND_QUEUE_HEAD = 0;
    SOUND_QUEUE_COUNT = 0;
    BGM_VOLUME = 0;
    BGM_FADE_ACTIVE = 0;
    PITCH_SLIDE_ACTIVE = 0;
    do {
        VOICE_PRIORITY[i] = 0xf;
        VOICE_SFX_ID[i] = -1;
        i++;
    } while (i < 0x18);
    m = -1;
    i = 7;
    do {
        VAB_IDS[i] = m;
        i--;
    } while (i >= 0);
    BGM_SEQ_ID = -1;
    JINGLE_SEQ_ID = -1;
    SOUND_INITIALIZED = 1;
}
