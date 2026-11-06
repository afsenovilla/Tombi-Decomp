// VERSION SLPS_01144 slps01144 8001E81C 244
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "globals.h"
extern short D_800A583E[];
extern unsigned char D_80071F30[];
extern unsigned char D_80071F31[];
extern unsigned char *D_80072138[];
extern short VAB_IDS[];
extern short SsUtKeyOnV(short, int, int, int, int, int, int, int);

static inline int find(unsigned x)
{
    int j = 0x17;
    int m = -1;
    int i = 0x17;
    short *q = D_800A583E;
    do {
        if (!VOICE_KEY_STATUS[i] && *q == m)
            return i;
        i--; j--; q--;
    } while (j >= 0x10);
    return -1;
}

int func_8001E81C_slps01144(unsigned a)
{
    int i;
    unsigned char *p;
    short v = find(a);
    i = (a & 0x3ff) * 2;
    p = D_80072138[D_80071F30[i]] + D_80071F31[i] * 8;
    return SsUtKeyOnV(v, VAB_IDS[p[0]], p[1], p[2], p[3], 0, p[4], p[4]);
}
#endif
