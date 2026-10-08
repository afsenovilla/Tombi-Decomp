// FUNC 801064d0 500 X013
// MATCHING 801064d0 500
extern void FUN_8001e560(int, int), FUN_800ee680(unsigned char *), FUN_8001fec0(unsigned char *), FUN_800ee428(unsigned char *), FUN_80040278(unsigned char *, int, int);
extern int FUN_8003facc(unsigned char *);
extern unsigned char *DAT_8009d2e8;
extern short DAT_8009c944, DAT_8009c946;
extern int DAT_8009c984;
extern unsigned short DAT_8009d670;

void FUN_801064d0(unsigned char *o)
{
    unsigned char *q;
    switch (o[7]) {
    case 0:
        q = DAT_8009d2e8;
        *(short *)(o + 0x7c) = 0;
        if (q[2] == 0x29)
            q[5] = 3;
        else {
            q[5] = 1;
            o[0xac] = 0;
        }
        FUN_8001e560(0x22, 0x23);
        o[7] = o[7] + 1;
    case 1:
        **(int **)(o + 0x40) += DAT_8009c944 << 8;
        *(int *)(o + 0x14) += DAT_8009c946 << 8;
        FUN_800ee680(o);
        FUN_8001fec0(o);
        if (*(short *)(o + 0x7e) > 0) {
            *(int *)(o + 0x84) = 0;
            o[0x9c] = 2;
            *(short *)(o + 0x7e) = 0;
            o[0xac] = 1;
            if ((DAT_8009c984 & 0x40) != 0 && (*(volatile unsigned short *)&DAT_8009d670 & *(unsigned short *)0x1f8003c4) != 0)
                o[0xa7] = 1;
            FUN_800ee428(o);
            o[5] = 2;
            o[6] = 3;
            o[7] = 0;
        }
        FUN_80040278(o, (*(short **)(o + 0x40))[1], (short)(*(unsigned short *)(o + 0x16) + 0x10));
        if (FUN_8003facc(o) != 0) {
            *(int *)(o + 0x84) = 0;
            o[0x9c] = 2;
            o[0xac] = 1;
            *(short *)(o + 0x7e) = 0;
            if ((DAT_8009c984 & 0x40) != 0 && (*(volatile unsigned short *)&DAT_8009d670 & *(unsigned short *)0x1f8003c4) != 0)
                o[0xa7] = 1;
            FUN_800ee428(o);
            o[5] = 2;
            o[6] = 3;
            o[7] = 0;
        }
    }
}
