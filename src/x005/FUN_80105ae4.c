// FUNC 80105ae4 232 X005
// MATCHING 80105ae4 232
#include "TOBJ.H"
extern char *DAT_8009c330;
extern char *DAT_8009d2e8;
extern unsigned char DAT_8009d2b1;
extern unsigned int DAT_8009c984;
extern volatile unsigned short DAT_8009d670;
extern unsigned short DAT_1f8003c4;
extern void FUN_800ee428(TObj *o);

void FUN_80105ae4(TObj *o)
{
    char *p, *q;
    int t;
    volatile unsigned short *r;
    DAT_8009c330[8] = 0;
    *((unsigned char *)o + 0xad) = 0;
    p = DAT_8009c330;
    q = DAT_8009d2e8;
    *(short *)((char *)o + 0xb2) = 0;
    *(short *)(p + 0x20) = 0xe;
    if (q[2] == 0x1c && (t = DAT_8009d2b1) < 3 && t != 0)
        *((unsigned char *)o + 0xab) |= 0x80;
    if ((DAT_8009c984 & 0x40) && (*(r = &DAT_8009d670) & DAT_1f8003c4))
        *((unsigned char *)o + 0xa7) = 1;
    FUN_800ee428(o);
    o->step = 2;
    o->state = 3;
}
