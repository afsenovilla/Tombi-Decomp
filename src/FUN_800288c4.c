// FUNC 800288c4 444 MAIN0
// MATCHING 800288c4 444
extern unsigned char DAT_800a60d4;
extern short DAT_1f8000e6;
extern unsigned short DAT_8009d670;
extern void FUN_800285ec(char *, int);
extern void FUN_80028754(char *, int);

static __inline__ void decay(void)
{
    short v = DAT_1f8000e6;
    if (v == 0) return;
    if (v > 0) {
        v -= 2;
        DAT_1f8000e6 = v;
        if (v < 0) DAT_1f8000e6 = 0;
    } else {
        v += 2;
        DAT_1f8000e6 = v;
        if (v > 0) DAT_1f8000e6 = 0;
    }
}

void FUN_800288c4(char *o, int a)
{
    volatile unsigned short *k;
    
    if (DAT_800a60d4 != 0) {
        decay();
        return;
    }
    k = &DAT_8009d670;
    if (*k & 0x10) {
        if (a != 0 || *(int *)(o + 0x20) < -0x9ff) {
            FUN_800285ec(o, a);
            return;
        }
        decay();
    } else if (*k & 0x40) {
        if (a != 0 || *(int *)(o + 0x20) >= 0xa00) {
            FUN_80028754(o, a);
            return;
        }
        decay();
    } else {
        decay();
    }
}
