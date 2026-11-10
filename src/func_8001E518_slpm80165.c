// VERSION SLPM_80165 slpm80165 8001E518 184
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPM_80165)
// Demo version of playSFXWithVolume (twin of func_8001E460: volume override 0x8100 / 0x800): all 24 voices are searched, the sound id is
// written when a free voice is found, and the commands are queued even when none is free (voice -1).
extern unsigned char VOICE_KEY_STATUS[];
extern short VOICE_SFX_ID[];
extern int queueSoundCommand(int, int);

int func_8001E518(unsigned a, unsigned b)
{
    int i = 0x17;
    int m = -1;
    int v;
    short *p = &VOICE_SFX_ID[0x17];
    do {
        if (!VOICE_KEY_STATUS[i] && *p == m) {
            *p = a;
            v = i;
            goto found;
        }
        i--;
        p--;
    } while (i >= 0);
    v = -1;
found:
    if (queueSoundCommand((b & 0xff) | 0x8100, v) == -1) return -1;
    queueSoundCommand((a & 0x3ff) | 0x800, v);
    return v;
}
#endif
