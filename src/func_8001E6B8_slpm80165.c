// VERSION SLPM_80165 slpm80165 8001E6B8 500
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPM_80165)
// Demo version of func_8001E618_slps01144 (src/func_8001E618_slps01144.c), the pitch-slide sound: all 24
// voices searched, the sound id written when a free voice is found. 
#include "globals.h"
extern unsigned short PITCH_SLIDE_VOICE;
extern unsigned char D_80071F30[], D_80071F31[];
extern unsigned char *D_80072138[];
extern unsigned short VAB_IDS[];
extern short VOICE_SFX_ID[];
extern void SsUtKeyOffV(int);
extern short D_800A583E[];
extern int queueSoundCommand(unsigned, int);

int func_8001E618_slps01144(unsigned p1, unsigned p2, unsigned p3, unsigned p4)
{
    unsigned short voice;
    int r, i;
    unsigned char *q;
    unsigned idx;
    int t;

    if (PITCH_SLIDE_ACTIVE != 0) {
        voice = PITCH_SLIDE_VOICE;
        if (VOICE_KEY_STATUS[voice] != 0) {
            VOICE_PRIORITY[voice] = 0xf;
            SsUtKeyOffV((short)voice);
        }
        PITCH_SLIDE_ACTIVE = 0;
    }
    idx = (p1 & 0xffff) * 2;
    q = D_80072138[D_80071F30[idx]] + D_80071F31[idx] * 8;
    PITCH_SLIDE_VAB = VAB_IDS[q[0]];
    PITCH_SLIDE_PROG = q[1];
    t = q[3];
    PITCH_SLIDE_NEW_FINE = 0;
    PITCH_SLIDE_OLD_FINE = 0;
    PITCH_SLIDE_NEW_NOTE = t;
    PITCH_SLIDE_OLD_NOTE = t;
    {
        int m;
        short *pp;
        i = 0x17;
        m = -1;
        pp = D_800A583E;
        do {
            if (!VOICE_KEY_STATUS[i] && *pp == m) { *pp = p1; r = i; goto found; }
            i--;
            pp--;
        } while (i >= 0);
        r = -1;
    }
found:
    if (queueSoundCommand(p1 & 0x3ff, r) == -1) return -1;
    if (queueSoundCommand(p2 & 0xff | 0xa000, r) == -1) return -1;
    if (queueSoundCommand(p3 & 0xff | 0xa100, r) == -1) return -1;
    if (queueSoundCommand(p4 & 0xff | 0xa200, r) == -1) return -1;
    queueSoundCommand(0xa300, r);
    return r;
}
#endif
