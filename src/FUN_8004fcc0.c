// FUNC 8004fcc0 172 MAIN0
// MATCHING 8004fcc0 172
typedef struct {
    char pad[8];
    unsigned short w8, wa, pad1[2], w10, w12, pad2[2], w18, w1a, pad3[2], w20, w22;
} Q;

int FUN_8004fcc0(Q *o)
{
    if (o->wa < 0x100 || o->w12 < 0x100 || o->w1a < 0x100 || o->w22 < 0x100) {
        if (o->w8 < 0x140 || o->w10 < 0x140 || o->w18 < 0x140 || o->w20 < 0x140)
            return 1;
    }
    return 0;
}
