// FUNC 8001e76c 464 MAIN0
// MATCHING 8001e76c 464
#include "globals.h"
extern unsigned short PITCH_SLIDE_VOICE;
extern unsigned char DAT_800780c4[];
extern unsigned char *SFX_BANKS[];
extern unsigned short VAB_IDS[];
extern short VOICE_SFX_ID[];
extern void SsUtKeyOffV(int);
extern int SoundFindFreeVoice(unsigned);
extern int FUN_8001f3fc(unsigned, int);

int playSFXWithPitchSlide(unsigned p1, unsigned p2, unsigned p3, unsigned p4)
{
    unsigned short voice;
    int r, i;
    unsigned char *q;
    unsigned idx;
    unsigned sv;
    int t;

    if (PITCH_SLIDE_ACTIVE != 0) {
        voice = PITCH_SLIDE_VOICE;
        if (VOICE_KEY_STATUS[voice] != 0) {
            VOICE_PRIORITY[voice] = 0xf;
            SsUtKeyOffV((short)voice);
        }
        PITCH_SLIDE_ACTIVE = 0;
    }
    sv = p1; asm("" : "=r"(sv) : "0"(sv));
    idx = (sv & 0xffff) * 2;
    q = SFX_BANKS[DAT_800780c4[idx]] + DAT_800780c4[idx + 1] * 8;
    PITCH_SLIDE_VAB = VAB_IDS[q[0]];
    PITCH_SLIDE_PROG = q[1];
    t = q[3];
    PITCH_SLIDE_NEW_FINE = 0;
    PITCH_SLIDE_OLD_FINE = 0;
    PITCH_SLIDE_NEW_NOTE = t;
    PITCH_SLIDE_OLD_NOTE = t;
    r = SoundFindFreeVoice(p1);
    if (r == -1) return -1;
    if (FUN_8001f3fc(p1 & 0x3ff, r) == -1) return -1;
    if (FUN_8001f3fc(p2 & 0xff | 0xa000, r) == -1) return -1;
    if (FUN_8001f3fc(p3 & 0xff | 0xa100, r) == -1) return -1;
    if (FUN_8001f3fc(p4 & 0xff | 0xa200, r) == -1) return -1;
    if (FUN_8001f3fc(0xa300, r) != -1)
        VOICE_SFX_ID[r] = sv;
    return r;
}
