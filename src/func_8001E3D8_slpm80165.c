// VERSION SLPM_80165 slpm80165 8001E3D8 136
#if defined(VERSION_SLPM_80165)
// Demo-only sound helper: takes the highest free channel slot (23..0) for sound id n, queues it; -1 when none is free.
extern unsigned char D_8009F718[];
extern short D_800A3328[];
extern void queueSoundCommand(int, int);
int func_8001E3D8_slpm80165(int n)
{
    int i, ch;
    for (i = 23; i >= 0; i--) {
        if (D_8009F718[i] == 0 && D_800A3328[i] == -1) {
            ch = i;
            D_800A3328[i] = n;
            goto found;
        }
    }
    ch = -1;
found:
    queueSoundCommand(n & 0x3ff, ch);
    return ch;
}
#endif
