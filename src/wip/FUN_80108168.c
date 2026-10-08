// FUNC 80108168 464 X000
extern unsigned char *DAT_9c330;
extern int DAT_9c960;
extern unsigned char DAT_9cda2, DAT_93a;
extern void FUN_800eeb5c(void *, int);
extern void FUN_8001fec0(void *);

void FUN_80108168(unsigned char *o)
{
    unsigned short u;
    unsigned char t;
    switch (o[6]) {
    case 0:
        DAT_9c330[8] = o[0];
        u = *(unsigned short *)(o + 0x2e);
        *(short *)(o + 0x7c) = 0x5a;
        o[0] = 2;
        o[0xa2] = 2;
        *(int *)(o + 0x8c) = 0;
        *(short *)(o + 0x7e) = 0;
        o[0x9c] = 0;
        o[0x9d] = 0;
        o[0x9e] = 0;
        o[0x9f] = 0;
        o[0xad] = 0;
        o[0x69] = 0;
        *(signed char *)(o + 0xf) = -0x14;
        *(unsigned short *)(o + 0x2e) = u & 1;
        FUN_800eeb5c(o, 0x2b);
        o[6] = o[6] + 1;
    case 1:
        FUN_8001fec0(o);
        if (DAT_9cda2 == 0)
            o[6] = o[6] + 1;
        break;
    case 2:
        FUN_8001fec0(o);
        *(unsigned short *)(*(unsigned char **)(o + 0x44) + 2) += 5;
        *(short *)(o + 0x7c) -= 5;
        if (DAT_9c960 == 0x40001) {
            *(int *)(o + 0x14) += -0x60000;
            if (*(short *)(o + 0x16) < -0x20f)
                *(short *)(o + 0x16) = -0x20f;
        }
        if (*(short *)(o + 0x7c) == 0) {
            *(signed char *)(o + 0xf) = -8;
            t = DAT_9c330[8];
            o[0x9c] = 0;
            o[0] = t;
            *(short *)(o + 0xb2) = 0;
            *(short *)(o + 0x7c) = 0;
            *(short *)(o + 0x7e) = 0;
            *(short *)(DAT_9c330 + 0x20) = 0;
            o[4] = 1;
            o[5] = 0;
            o[6] = 0;
            o[7] = 0;
            *(short *)(o + 0x20) = 0;
            DAT_93a = 1;
        }
        break;
    }
}
