// FUNC 801380b8 352 X000
// MATCHING 801380b8 352
extern unsigned char DAT_a, DAT_b, DAT_c;
extern unsigned char *FUN_800183b8(void);
extern short FUN_80137458(void *);
extern void FUN_801377b8(void *);
extern void FUN_80018980(void *);

void FUN_801380b8(unsigned char *o)
{
    unsigned char *q;
    unsigned char **p;
    switch (o[4]) {
    case 0:
        p = (unsigned char **)(o + 0x1c);
        if (DAT_a == 0xff)
            o[4] = 3;
        if (DAT_b < 6) {
            q = FUN_800183b8();
            if (q != 0) {
                q[0] = 1;
                q[2] = 0x15;
                if (DAT_c == 0) {
                    *(short *)(q + 0x2e) = 1;
                    *(short *)(q + 0x12) = 0x140;
                    *(short *)(q + 0x16) = -0x32;
                    *(short *)(q + 0x1a) = 0;
                    q[3] = 0x63;
                } else {
                    *(short *)(q + 0x12) = 0x138;
                    *(short *)(q + 0x2e) = 0;
                    *(short *)(q + 0x16) = -0x11c;
                    *(short *)(q + 0x1a) = 0;
                    q[3] = 0;
                }
                *p = q;
            }
            o[4] = 1;
        } else
            o[4] = 3;
        break;
    case 1:
        if (FUN_80137458(o) != 0)
            FUN_801377b8(o);
        break;
    case 2:
        o[4] = 3;
        break;
    case 3:
        FUN_80018980(o);
        break;
    }
}
