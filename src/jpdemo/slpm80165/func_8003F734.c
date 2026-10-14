// JPDEMO slpm80165 8003F734 172
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
