// VERSION SLPM_80165 x00 8012B554 128
#if defined(VERSION_SLPM_80165)
// Demo-only helper: while D_8009AD6D < 6, spawns a type-21 marker object and links it at +0x1c; else frees itself.
extern unsigned char D_8009AD6D;
extern unsigned char *func_80017040(void);
void func_8012B554_slpm80165(unsigned char *o)
{
    unsigned char *n;
    if (D_8009AD6D < 6) {
        n = func_80017040();
        if (n != 0) {
            n[0] = 1;
            n[2] = 21;
            *(short *)(n + 0x12) = 312;
            *(short *)(n + 0x2e) = 0;
            *(short *)(n + 0x16) = -284;
            *(short *)(n + 0x1a) = 0;
            *(unsigned char **)(o + 0x1c) = n;
        }
        o[4] = 1;
    } else {
        o[4] = 3;
    }
}
#endif
