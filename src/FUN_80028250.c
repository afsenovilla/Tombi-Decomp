// FUNC 80028250 464 MAIN0
// MATCHING 80028250 464
extern int DAT_1f8000f0;
extern short DAT_1f8000f2;
extern int DAT_1f800190;
extern void FUN_800284d8(unsigned char *);
#define W(o) (*(int *)((o) + 0x18))

void FUN_80028250(unsigned char *o)
{
    int *p, t;
    switch (*(short *)(o + 0x5c)) {
    case 0:
        o[0x6d] = 0;
        *(signed char *)(o + 0x6f) = -10;
        (*(short *)(o + 0x5c))++;
    case 1:
        W(o) += 0x2000;
        if (W(o) > 0x3ffff)
            W(o) = 0x40000;
        if (W(o) > 0x12000)
            FUN_800284d8(o);
        p = &DAT_1f8000f0;
        *p += W(o);
        if (DAT_1f8000f2 > *(short *)(o + 0x32)) {
            DAT_1f8000f0 = *(short *)(o + 0x32) << 16;
            W(o) = 0;
            *(unsigned short *)(o + 0x44) |= 8;
        }
        t = *p - DAT_1f800190;
        if (t > 0x580000)
            (*(short *)(o + 0x5c))++;
        break;
    case 2:
        W(o) += 0x2000;
        if (W(o) > 0x2ffff)
            W(o) = 0x30000;
        p = &DAT_1f8000f0;
        *p += W(o);
        if (DAT_1f8000f2 > *(short *)(o + 0x32)) {
            DAT_1f8000f0 = *(short *)(o + 0x32) << 16;
            W(o) = 0;
            *(unsigned short *)(o + 0x44) |= 8;
        }
        t = *p - DAT_1f800190;
        if (t <= 0)
            o[3] = 3;
        break;
    }
}
