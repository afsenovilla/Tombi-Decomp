// VERSION SLPS_01144 slps01144 8001E528 0xF0
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
extern unsigned int queueSoundCommand(int, int);
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

int playSFXWithNoteAndVolume(unsigned a, unsigned b, unsigned d)
{
    int v = findVoice();
    unsigned c;
    if (v == -1) return -1;
    if (queueSoundCommand((b & 0xff) | 0x8000, v) == -1) return -1;
    if (queueSoundCommand((d & 0xff) | 0x8100, v) == -1) return -1;
    {
        unsigned int r;
        do { asm("" : "=r"(c) : "0"(a)); r = queueSoundCommand((a & 0x3ff) | 0xc00, v); } while (0);
        if (r != -1) VOICE_SFX_ID[v] = c;
    }
    return v;
}
#endif
