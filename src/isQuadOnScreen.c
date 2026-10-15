// FUNC 8004fcc0 172 MAIN0
// MATCHING 8004fcc0 172
// VERSION SLPM_80165 slpm80165 8003F734 172
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 8004FCC0 (FUN_8004fcc0.c): screen bounds 0xf0/0x140 inclusive (PAL < 0x100 / < 0x140).
typedef struct {
    char pad[8];
    unsigned short w8, wa, pad1[2], w10, w12, pad2[2], w18, w1a, pad3[2], w20, w22;
} Q;

int FUN_8004fcc0(Q *o)
{
    if (o->wa < 0xf1 || o->w12 < 0xf1 || o->w1a < 0xf1 || o->w22 < 0xf1) {
        if (o->w8 < 0x141 || o->w10 < 0x141 || o->w18 < 0x141 || o->w20 < 0x141)
            return 1;
    }
    return 0;
}
#else
typedef struct {
    char pad[8];
    unsigned short w8, wa, pad1[2], w10, w12, pad2[2], w18, w1a, pad3[2], w20, w22;
} Q;

int isQuadOnScreen(Q *o)
{
    if (o->wa < 0x100 || o->w12 < 0x100 || o->w1a < 0x100 || o->w22 < 0x100) {
        if (o->w8 < 0x140 || o->w10 < 0x140 || o->w18 < 0x140 || o->w20 < 0x140)
            return 1;
    }
    return 0;
}
#endif
