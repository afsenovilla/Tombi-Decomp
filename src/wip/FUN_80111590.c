// FUNC 80111590 388 X000
extern void FUN_8001fe6c(unsigned char *);
extern void FUN_8001fec0(unsigned char *);
extern char DAT_8011592c[];
extern unsigned char DAT_800a6039, DAT_800a6047;
extern unsigned short DAT_800a6066, DAT_800a604e;
extern unsigned short *DAT_800a6078, *DAT_800a607c;

void FUN_80111590(unsigned char *o)
{
    unsigned c;
    int b;
    short v;
    switch (o[6]) {
    case 0:
        *(int *)(o + 0x24) = *(int *)(*(int *)(DAT_8011592c + o[3] * 12) + o[0xc] * 4);
        FUN_8001fe6c(o);
        *(short *)(o + 0x20) = 0xb4;
        o[0xa5] = 1;
        o[6] = o[6] + 1;
    case 1:
        FUN_8001fec0(o);
        *(short *)(o + 0x20) = *(short *)(o + 0x20) - 1;
        o[1] = DAT_800a6039;
        if (*(short *)(o + 0x20) >= 0x3d) {
            c = o[0x6b];
            b = c + 8;
            if (c > 0x7e)
                b = c;
        } else {
            if (*(short *)(o + 0x20) < 0) {
                o[0x6b] = 0;
                o[4] = 3;
                goto L;
            }
            b = o[0x6b] - 2;
            if (o[0x6b] == 0)
                b = 0;
        }
        o[0x6b] = b;
    L:
        v = 2;
        if ((DAT_800a6066 & 1) == 0)
            v = -2;
        (*(short **)(o + 0x40))[1] = DAT_800a6078[1] + v;
        *(short *)(o + 0x16) = DAT_800a604e - 8;
        (*(short **)(o + 0x44))[1] = DAT_800a607c[1];
        o[0xf] = DAT_800a6047 + 1;
    }
}
