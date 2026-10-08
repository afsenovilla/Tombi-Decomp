// FUNC 800f6e28 272 X011
// MATCHING 800f6e28 272
extern char *DAT_8009f0ec;
extern int FUN_80122cbc(void);
extern int FUN_8011ca00(void);

short FUN_800f6e28(char *o)
{
    char *e = DAT_8009f0ec;
    int r;
    *(int *)(o + 0x30) = *(short *)(*(char **)(e + 0x40) + 2);
    *(int *)(o + 0x34) = *(short *)(e + 0x16);
    r = 0;
    switch (*(unsigned char *)(e + 2)) {
    case 0xe:
        *(short *)(o + 0xb8) = *(int *)(e + 0x30) - *(unsigned short *)(*(char **)(e + 0x40) + 2);
        *(short *)(o + 0xba) = *(int *)(e + 0x34) - *(unsigned short *)(e + 0x16);
        break;
    case 0x14:
        r = FUN_80122cbc();
        break;
    case 0x34:
        r = FUN_8011ca00();
        break;
    case 0x1f:
        if (*(unsigned char *)(e + 0x6a) != 0) {
            if ((*(unsigned short *)(e + 0x2e) & 1) == (*(unsigned short *)(o + 0x2e) & 1)) {
                *(unsigned char *)(o + 0x69) = 1;
                r = 1;
            }
        }
        break;
    }
    return r;
}
