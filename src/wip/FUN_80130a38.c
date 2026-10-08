// FUNC 80130a38 376 X000
extern int DAT_8013ad4c;
extern unsigned short DAT_8013b240[];
extern char *DAT_8009607c;
extern char *DAT_80096078;
extern unsigned short DAT_800a604e;
extern void FUN_8001fe6c(void *);
extern void FUN_8001e4f0(int);
extern unsigned FUN_8001f9e0(void);
extern void FUN_8001fec0(void *);

void FUN_80130a38(unsigned char *o)
{
    switch (o[7]) {
    case 0:
        *(short *)(o + 0x20) = 0x8a;
        *(short *)(o + 0x22) = 1;
        *(short *)(o + 0xb4) = 0;
        o[7] = o[7] + 1;
    case 1:
        *(short *)(o + 0x22) = *(short *)(o + 0x22) - 1;
        if (*(short *)(o + 0x22) == 0) {
            *(short *)(o + 0xac) = 7;
            *(int *)(o + 0x24) = DAT_8013ad4c;
            FUN_8001fe6c(o);
            if (*(unsigned short *)(o + 0xca) != 0)
                FUN_8001e4f0(0x13);
            *(short *)(o + 0x22) = 0x2e;
        }
        *(short *)(o + 0x20) = *(short *)(o + 0x20) - 1;
        if (*(short *)(o + 0x20) == 0) {
            o[6] = DAT_8013b240[FUN_8001f9e0() & 0xf];
            o[7] = 0;
            if (*(short *)(*(char **)(o + 0x44) + 2) == *(short *)(DAT_8009607c + 2)) {
                unsigned short a = DAT_800a604e - *(unsigned short *)(o + 0x16);
                if ((unsigned short)(*(unsigned short *)(DAT_80096078 + 2) - *(unsigned short *)(*(char **)(o + 0x40) + 2) + 0x80) < 0x100) {
                    a = a + 0x30;
                    if (a < 0xf8) {
                        o[6] = 4;
                        o[7] = 0;
                    }
                }
            }
        }
        break;
    }
    FUN_8001fec0(o);
}
