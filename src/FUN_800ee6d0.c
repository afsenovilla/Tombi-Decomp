// FUNC 800ee6d0 272 X000
// MATCHING 800ee6d0 272
extern unsigned short DAT_8009d670;
extern unsigned short DAT_8009c960;
extern short FUN_8001fe3c(int, int);

void FUN_800ee6d0(char *o)
{

    volatile unsigned short *pad = &DAT_8009d670;
    int a = 10;
    short d;
    int t;
    if ((*pad & 0xa0) == 0)
        *(short *)(o + 0x76) = 0;
    if ((*pad & 0x40) != 0)
        a = 8;
    if ((*pad & 0x80) != 0)
        *(short *)(o + 0x76) = 0xf0;
    if ((*pad & 0x20) != 0)
        *(short *)(o + 0x76) = 0x10;
    t = *(int *)(o + 0x88);
    d = (*(unsigned short *)(o + 0x76) - t) & 0xff;
    if (d != 0) {
        unsigned char e = d;
        if (e < 0x80)
            *(int *)(o + 0x88) = t + 1;
        else
            *(int *)(o + 0x88) = t - 1;
    }
    if (DAT_8009c960 == 3)
        *(int *)(o + 0x84) = *(int *)(o + 0x84) + 6;
    else
        *(int *)(o + 0x84) = *(int *)(o + 0x84) + 4;
    *(int *)(o + 0x8c) = (*(int *)(o + 0x88) + FUN_8001fe3c(*(unsigned char *)(o + 0x84), a)) & 0xff;
}
