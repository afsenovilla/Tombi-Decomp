// VERSION BETA_980203 beta980203 8001FCB4 256
// FLAGS.BETA_980203 -O2 -G0 -fno-strength-reduce
#if defined(VERSION_BETA_980203)
// Beta version: JP C of src/func_8001E910_slps01144.c (func_8001E910_slps01144) (1.000 masked) [-O2 -G0 -fno-strength-reduce] (tools/demo_sweep.py)
#include "globals.h"
extern short D_800A583E_beta980203[];
extern unsigned char D_80071F30_beta980203[];
extern unsigned char D_80071F31_beta980203[];
extern unsigned char *D_80072138_beta980203[];
extern short VAB_IDS[];
extern short SsUtKeyOnV(short, int, int, int, int, int, int, int);

static inline int find(unsigned x)
{
    int j = 0x17;
    int m = -1;
    int i = 0x17;
    short *q = D_800A583E_beta980203;
    do {
        if (!VOICE_KEY_STATUS[i] && *q == m)
            return i;
        i--; j--; q--;
    } while (j >= 0x10);
    return -1;
}

int func_8001FCB4_beta980203(unsigned a, short b)
{
    int i;
    unsigned char *p;
    short v = find(a);
    i = (a & 0x3ff) * 2;
    p = D_80072138_beta980203[D_80071F30_beta980203[i]] + D_80071F31_beta980203[i] * 8;
    return SsUtKeyOnV(v, VAB_IDS[p[0]], p[1], p[2], b, 0, p[4], p[4]);
}
#endif
