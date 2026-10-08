// FUNC 801274cc 280 X000
// MATCHING 801274cc 280
typedef struct O {
    char p0[0x16]; unsigned short y;
    char p1[0x40 - 0x18]; short *t;
    char p2[0x69 - 0x44]; unsigned char b69;
    char p3[0x8c - 0x6a]; int d8c;
    char p4[0x98 - 0x90]; short w98;
    char p5[0x9c - 0x9a]; unsigned char b9c;
    char p6[0xae - 0x9d]; short wae;
    char p7[0xb2 - 0xb0]; short wb2;
    char p8[0xb6 - 0xb4]; short wb6;
} O;
extern short DAT_1f80027e;
extern unsigned short DAT_1f800282, DAT_1f800284;
extern short FUN_80040278(O *, int, int);

int FUN_801274cc(O *o)
{
    short v;
    int r;
    short s;
    unsigned short u;
    if (o->b69 == 1) {
        o->d8c = 0;
        o->wb2 = 0;
        o->b69 = 0;
        o->wae = -1;
        o->b9c = 0;
        return 1;
    } else {
        s = FUN_80040278(o, o->t[1], (short)(o->y + 0x10));
        if (s != 0) {
            o->b69 = 0;
            v = DAT_1f80027e;
            if (v < 0) v = -v;
            if (v > 8) v = 8;
            if (DAT_1f80027e < 0) v = -v;
            o->d8c = (-v) & 0xff;
            s = DAT_1f800284;
            o->wb2 = v;
            o->b9c = 0;
            u = DAT_1f800282;
            o->wb6 = ((-v) << 2) & 0xff;
            o->wae = s;
            if ((u >> 5) & 8)
                o->w98 = 0;
            return 1;
        }
    }
    return 0;
}
