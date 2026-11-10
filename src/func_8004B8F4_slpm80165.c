// VERSION SLPM_80165 slpm80165 8004B8F4 280
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of playerContactCat2Type15 (src/playerContactCat2Type15.c): skipped while +0x9E is set; a pushed
// object whose byte 0 lacks bit 2 sends the player sliding off it (+0xBE 8 / 9, +0xB0 +-2); the grab stores the
// object at +0xD0 (PAL +0xE4) and clears the scratchpad word 0x1F80019E.
extern short func_80049850_slpm80165(void);
extern void func_80020054_slpm80165(int, int, int, int);

void func_8004B8F4(char *o0, char *p)
{
    register char *o asm("$16") = o0;
    short r;
    if (o[0x9e] != 0)
        return;
    r = func_80049850_slpm80165();
    if (r != 0) {
        if (o[0xa6] & 2)
            o[0xa6] = o[0xa6] + 2;
        if (r == 1) {
            if (p[0] & 4) {
                if (o[0xac] == r) {
                    p[0] = 2;
                    p[4] = 2;
                    p[5] = 0;
                    p[6] = 0;
                    p[0x69] = 0;
                    *(char **)(o + 0xd0) = p;
                    o[0xac] = 2;
                    func_80020054_slpm80165(2, *(short *)(o + 0x12), *(short *)(o + 0x16), *(short *)(o + 0x1a));
                }
                *(short *)0x1F80019E = 0;
            } else {
                short v;
                if (*(short *)(*(char **)(o + 0x40) + 2) > *(short *)(*(char **)(p + 0x40) + 2)) {
                    o[0xbe] = 8;
                    v = 2;
                } else {
                    o[0xbe] = 9;
                    v = -2;
                }
                *(short *)(o + 0xb0) = v;
            }
        }
    }
}
#endif
