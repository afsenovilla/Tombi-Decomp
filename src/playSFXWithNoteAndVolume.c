// FUNC 8001e688 228 MAIN0
// MATCHING 8001e688 228
// VERSION BETA_980203 beta980203 8001F8CC 240
// FLAGS.BETA_980203 -O2 -G0 -fno-strength-reduce
#if defined(VERSION_BETA_980203)
// Beta version of PAL main0 8001E688 (playSFXWithNoteAndVolume.c): JP C of src/func_8001E528_slps01144.c (playSFXWithNoteAndVolume) (1.000 masked) [-O2 -G0 -fno-strength-reduce] (tools/demo_sweep.py)
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
#else
extern unsigned short GAME;
extern short VOICE_SFX_ID[];
extern int SoundFindFreeVoice(int a);
extern int queueSoundCommand(int a, int b);

int playSFXWithNoteAndVolume(unsigned int a, unsigned int b, unsigned int c)
{
    int v;
    if ((GAME == 2 || GAME == 0x13) && a == 0x32) a = 0x61;
    v = SoundFindFreeVoice(a);
    if (v == -1 || queueSoundCommand((b & 0xff) | 0x8000, v) == -1 || queueSoundCommand((c & 0xff) | 0x8100, v) == -1)
        return -1;
    if (queueSoundCommand((a & 0x3ff) | 0xc00, v) != -1)
        VOICE_SFX_ID[v] = a;
    return v;
}
#endif
