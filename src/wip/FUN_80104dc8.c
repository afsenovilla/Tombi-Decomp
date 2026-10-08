// FUNC 80104dc8 216 X000
extern char *DAT_8009d2e8;
extern void FUN_800ef490(void);
extern void FUN_8010f0f4(char *);
extern void FUN_8001fd94(char *);
extern void FUN_8001fec0(char *);
extern void FUN_80040278(char *, int, int);
extern int FUN_8003facc(char *);

void FUN_80104dc8(char *o)
{
    char *p = DAT_8009d2e8;
    *(unsigned short *)(*(char **)(p + 0x40) + 2) = *(unsigned short *)(*(char **)(o + 0x40) + 2);
    *(unsigned short *)(p + 0x16) = *(unsigned short *)(o + 0x16) + *(unsigned short *)(p + 0x70);
    FUN_800ef490();
    FUN_8010f0f4(o);
    FUN_8001fd94(o);
    FUN_8001fec0(o);
    if (*(short *)(o + 0x7e) > 0) {
        o[0x9c] = 2;
        *(int *)(o + 0x84) = 0;
        *(short *)(o + 0x7e) = 0;
        o[7] = 1;
    }
    FUN_80040278(o, *(short *)(*(char **)(o + 0x40) + 2), (short)(*(unsigned short *)(o + 0x16) + 0x10));
    if (FUN_8003facc(o) != 0) {
        o[0x9c] = 2;
        *(int *)(o + 0x84) = 0;
        *(short *)(o + 0x7e) = 0;
        o[7] = 1;
    }
}
