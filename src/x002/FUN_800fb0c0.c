// FUNC 800fb0c0 448 X002
// MATCHING 800fb0c0 448
typedef struct P { char pad0[0xa]; unsigned char b0a; char pad1[0x20 - 0xb]; unsigned short w20, w22; } P;
extern volatile unsigned short DAT_8009d670[];
extern P *DAT_8009c330;
extern int FUN_8003facc(char *);
#define I32(p, o) (*(int *)((p) + (o)))
#define S16(p, o) (*(short *)((p) + (o)))

void FUN_800fb0c0(char *o)
{
    P *q;
    int t;
#define pad DAT_8009d670
    if (!(*pad & 0x10))
        DAT_8009c330->w20 = 0;
    if (!(*pad & 0x40))
        DAT_8009c330->w22 = 0;
    if (DAT_8009c330->b0a == 0) {
        if ((*pad & 0x10) && FUN_8003facc(o) == 0) {
            q = DAT_8009c330;
            I32(o, 0x38) -= q->w20 << 8;
            if ((q->w20 += 8) > 0x200)
                q->w20 = 0x200;
            if (I32(o, 0x38) < 0x280000)
                I32(o, 0x38) = 0x280000;
        }
        if (*pad & 0x40) {
            q = DAT_8009c330;
            I32(o, 0x38) += q->w22 << 8;
            if ((q->w22 += 8) > 0x200)
                q->w22 = 0x200;
            if (I32(o, 0x38) > 0x780000)
                I32(o, 0x38) = 0x780000;
        }
    }
    t = S16(o, 0x3a);
    S16(o, 0xb4) = t;
    if (t < 0x28)
        S16(o, 0xb4) = 0x28;
    if (S16(o, 0xb4) > 0x78)
        S16(o, 0xb4) = 0x78;
}
