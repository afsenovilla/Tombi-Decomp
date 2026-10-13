// FUNC 8001e430 192 MAIN0
// MATCHING 8001e430 192
#include "globals.h"
extern short DAT_800a3cf6[];
extern short DAT_8009c8ee[];
extern unsigned char DAT_800780c4[];
extern unsigned char DAT_800780c5[];
extern unsigned char *SFX_BANKS[];

int SoundFindFreeVoice(unsigned a)
{
    int i = 0x17;
    int m = -1;
    short *p = DAT_800a3cf6;
    int k;
    short *q;
    do {
        if (!VOICE_KEY_STATUS[i] && *p == m)
            return i;
        i--;
        p--;
    } while (i >= 0x10);
    i = 0x17;
    k = (a & 0xffff) * 2;
    q = DAT_8009c8ee;
    do {
        if (*q >= (short)(SFX_BANKS[DAT_800780c4[k]] + (DAT_800780c5[k] << 3))[5])
            return i;
        i--;
        q--;
    } while (i >= 0x10);
    return -1;
}
