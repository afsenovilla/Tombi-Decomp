// FUNC 800f0374 220 X000
extern char *DAT_8009c330;
extern void FUN_8003fd78(char *, int, int);

int FUN_800f0374(char *o)
{
    o[0xad] = 0;
    FUN_8003fd78(o, 0, 0);
    if ((o[0x69] | o[0x9c] | o[0x9e] | o[0x9f] | o[0xbe]) == 0) {
        o[0xad] = 1;
        *(unsigned short *)(o + 0x7e) = *(unsigned short *)(o + 0x7e) + 0x223;
        *(int *)(o + 0x14) = *(int *)(o + 0x14) + (*(short *)(o + 0x7e) << 8);
        *(short *)(DAT_8009c330 + 0x20) = 0x21;
        if (*(short *)(o + 0x7e) >= 0x447) {
            o[0xcd] = 0;
            o[0xce] = 0;
            o[0xad] = 0;
            *(short *)(o + 0x7e) = 0;
            o[0x9c] = 2;
            o[0xc3] = 0;
            *(int *)(o + 0x8c) = 0;
            return 1;
        }
        return 0;
    }
    *(short *)(o + 0x7e) = 0;
    o[0x9c] = 0;
    return 0;
}
