// VERSION BETA_980203 beta980203 8001F7FC 208
// FLAGS.BETA_980203 -O2 -G0 -fno-strength-reduce
#if defined(VERSION_BETA_980203)
// Beta version: JP C of src/func_8001E458_slps01144.c (func_8001E458_slps01144) (1.000 masked) [-O2 -G0 -fno-strength-reduce] (tools/demo_sweep.py)
extern unsigned int queueSoundCommand(int, int);
extern unsigned short VOICE_SFX_ID[];
extern unsigned char VOICE_KEY_STATUS[];
extern short D_800A583E_beta980203[];

static inline int findVoice(void)
{
    int i = 0x17;
    int m = -1;
    short *p = D_800A583E_beta980203;
    do {
        if (!VOICE_KEY_STATUS[i] && *p == m) return i;
        i--;
        p--;
    } while (i >= 0x10);
    return -1;
}

int func_8001F7FC_beta980203(unsigned a, unsigned b)
{
    int v = findVoice();
    unsigned c;
    if (v == -1) return -1;
    if (queueSoundCommand((b & 0xff) | 0x8100, v) == -1) return -1;
    {
        unsigned int r;
        do { asm("" : "=r"(c) : "0"(a)); r = queueSoundCommand((a & 0x3ff) | 0x800, v); } while (0);
        if (r != -1) VOICE_SFX_ID[v] = c;
    }
    return v;
}
#endif
