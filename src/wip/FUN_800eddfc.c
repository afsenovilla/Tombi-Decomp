// FUNC 800eddfc 408 X000
extern void FUN_800202b4(unsigned char *);
extern unsigned char DAT_800a60d6;
extern unsigned short DAT_800a6066;

void FUN_800eddfc(unsigned char *o)
{
    unsigned char s, st;
    unsigned char *p;
    short v, w;
    st = o[5];
    switch (st) {
    case 0:
    {
        p = *(unsigned char **)(o + 0x94);
        FUN_800202b4(p);
        o[1] = p[1];
        if (o[6] == 0)
            o[6] = 1;
        if ((o[0x69] & 2) == 0)
            return;
        p = *(unsigned char **)(o + 0x94);
        p[5] = 1;
        p[6] = 0;
        o[5] = 1;
        goto done;
    }
    case 1:
    {
    p = *(unsigned char **)(o + 0x94);
    FUN_800202b4(p);
    s = o[6];
    o[1] = p[1];
    if (s == 1) {
    L:
        v = 1;
        if ((DAT_800a6066 & 1) == 0)
            v = -1;
        *(short *)(o + 0x7a) = v;
        o[6] = o[6] + 1;
    } else {
        if (s < 2) {
            if (s != 0)
                return;
            v = (*(short **)(o + 0x40))[1];
            w = *(short *)(o + 0x16);
            *(short *)(o + 0x80) = 0;
            o[6] = o[6] + 1;
            *(unsigned short *)(o + 0x7e) = w;
            *(int *)(o + 0x8c) = 0x1000;
            *(short *)(o + 0x7c) = v;
            goto L;
        }
        if (s != 2)
            return;
    }
    *(int *)(o + 0x8c) = ((*(int *)(*(int *)(o + 0x94) + 0x84) >> 3) + 0x1000) & 0xfff;
    if (DAT_800a60d6 != 0)
        return;
    *(int *)(o + 0x8c) = 0;
    o[0x69] = 0;
    o[5] = 0;
    }
    default:
        return;
    }
done:
    o[6] = 0;
}
