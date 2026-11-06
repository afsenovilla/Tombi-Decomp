// VERSION SLPS_01144 slps01144 8001E2D4 0xB4
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
extern int queueSoundCommand(int, int);
extern unsigned short VOICE_SFX_ID[];
extern unsigned char VOICE_KEY_STATUS[];
extern short D_800A583E[];

static inline int findVoice(void)
{
    int i = 0x17;
    int m = -1;
    short *p = D_800A583E;
    do {
        if (!VOICE_KEY_STATUS[i] && *p == m) return i;
        i--;
        p--;
    } while (i >= 0x10);
    return -1;
}

int playSFX(unsigned a)
{
    int v = findVoice();
    if (v == -1) return -1;
    {
    unsigned b;
    asm("" : "=r"(b) : "0"(a));
    if (queueSoundCommand(a & 0x3ff, v) != -1) VOICE_SFX_ID[v] = b;
    }
    return v;
}
#endif
