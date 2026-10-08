// FUNC 8004b6a0 292 MAIN0
extern short DAT_1f8003bc;

int FUN_8004b6a0(char *a, char *b)
{
    char pad;
    int ad, v, w;
    short d;
    unsigned short e;
    if ((unsigned short)(*(unsigned short *)(*(char **)(a + 0x44) + 2) - *(unsigned short *)(*(char **)(b + 0x44) + 2) + 0x2d) < 0x5b) {
        e = *(unsigned short *)(a + 0xe8);
        d = e - *(unsigned short *)(*(char **)(a + 0x40) + 2);
        ad = d < 0 ? -d : d;
        if (*(unsigned short *)(a + 0x2e) & 1)
            v = *(unsigned short *)(b + 0x6c) + ad;
        else
            v = *(unsigned short *)(b + 0x6c);
        w = e - *(unsigned short *)(*(char **)(b + 0x40) + 2);
        if (*(short *)(b + 0x6e) + (short)ad < (unsigned short)(v + w))
            return 0;
        w = *(unsigned short *)(b + 0x70) + (*(unsigned short *)(a + 0xea) - *(unsigned short *)(b + 0x16));
        if (*(short *)(b + 0x72) < (unsigned short)w)
            return 0;
        if (*(unsigned short *)(a + 0x2e) & 1)
            DAT_1f8003bc = *(short *)(b + 0x6e) - *(unsigned short *)(b + 0x6c);
        else
            DAT_1f8003bc = -*(unsigned short *)(b + 0x6c);
        return 1;
    }
    return 0;
}
