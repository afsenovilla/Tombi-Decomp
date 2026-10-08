// FUNC 8003c338 536 MAIN0
// MATCHING 8003c338 536
extern void FUN_8001faf4(unsigned char *), FUN_8001fe6c(unsigned char *);
extern short FUN_800411cc(unsigned char *, int, int);
extern unsigned short DAT_8009c960;
extern int DAT_8013b104, DAT_80134d0c;
extern char DAT_80077d54[];

void FUN_8003c338(unsigned char *o)
{
    unsigned x;
    short v, w;
    switch (o[5]) {
    case 0:
        if ((*(unsigned short *)(o + 0x2e) & 2) == 0)
            *(short *)(o + 0x7e) = -0x200;
        else
            *(short *)(o + 0x7e) = 0x200;
        *(char **)(o + 0x28) = DAT_80077d54;
        o[5] = o[5] + 1;
        if (DAT_8009c960 == 0)
            *(int *)(o + 0x24) = DAT_8013b104;
        else
            *(int *)(o + 0x24) = DAT_80134d0c;
        FUN_8001fe6c(o);
    case 1:
        if (*(unsigned short *)(o + 0x2e) & 1)
            *(int *)(o + 0x8c) = (*(int *)(o + 0x8c) - 0x14) & 0xff;
        else
            *(int *)(o + 0x8c) = (*(int *)(o + 0x8c) + 0x14) & 0xff;
        if ((*(unsigned short *)(o + 0x2e) & 2) == 0)
            FUN_8001faf4(o);
        *(int *)(o + 0x14) = *(int *)(o + 0x14) + (*(short *)(o + 0x7e) << 8);
        v = *(short *)(o + 0x7e);
        v += (*(unsigned short *)(o + 0x2e) & 2) ? 0x50 : 0x20;
        *(short *)(o + 0x7e) = v;
        if (v > 0)
            o[5] = o[5] + 1;
        break;
    case 2:
        if (*(unsigned short *)(o + 0x2e) & 1)
            *(int *)(o + 0x8c) = (*(int *)(o + 0x8c) - 0x14) & 0xff;
        else
            *(int *)(o + 0x8c) = (*(int *)(o + 0x8c) + 0x14) & 0xff;
        if ((*(unsigned short *)(o + 0x2e) & 2) == 0)
            FUN_8001faf4(o);
        *(int *)(o + 0x14) = *(int *)(o + 0x14) + (*(short *)(o + 0x7e) << 8);
        v = *(unsigned short *)(o + 0x7e) + 0x20;
        *(short *)(o + 0x7e) = v;
        if (FUN_800411cc(o, (*(short **)(o + 0x40))[1], (short)(*(unsigned short *)(o + 0x16) + 0x14)) != 0) {
            o[4] = 2;
            o[5] = 0;
        }
        break;
    }
}
