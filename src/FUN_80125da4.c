// FUNC 80125da4 396 X000
// MATCHING 80125da4 396
extern short FUN_8004461c(void *, void *);
extern void FUN_8001f96c(int, int, int, int);
extern short DAT_1f80019e;

void FUN_80125da4(unsigned char *o, unsigned char *p)
{
    unsigned t;
    if (*p & 2)
        return;
    if (FUN_8004461c(o, p) == -1)
        return;
    t = o[2];
    switch (t) {
    case 0:
    case 9:
        *(short *)(o + 0xa8) = *(short *)(o + 0xa8) - 0x200;
        if (*(short *)(o + 0xa8) < 0x500) {
            *(short *)(o + 0xa8) = 0x4ff;
            o[0xa5] = 0;
            o[0] = 2;
            DAT_1f80019e = 0;
        }
        break;
    case 1:
        o[0xa5] = 0;
        o[0] = 2;
        o[4] = 2;
        o[5] = 0;
        o[6] = 0;
        break;
    case 2:
    case 3:
    case 4:
        break;
    case 5:
    case 6:
    case 7:
        o[0x6a] = 1;
        *(short *)(o + 0xa8) = *(short *)(o + 0xa8) - 0x200;
        o[0] = 2;
        if (*(short *)(o + 0xa8) < 0x500) {
            *(short *)(o + 0xa8) = 0x4ff;
            o[0xa5] = 0;
            o[0] = 2;
            DAT_1f80019e = 0;
        }
        break;
    case 8:
        o[0] = 2;
        o[0x69] = 0;
        o[0xa5] = 0;
        *(short *)(o + 0xa8) = 0x4ff;
        FUN_8001f96c(1, *(short *)(o + 0x12), *(short *)(o + 0x16), *(short *)(o + 0x1a));
        break;
    default:
        goto dflt;
    }
    t = o[2];
dflt:
    if (t != 8) {
        *p = 3;
        p[0x68] = 1;
        FUN_8001f96c(0, *(short *)(o + 0x12), *(short *)(o + 0x16), *(short *)(o + 0x1a));
    }
}
