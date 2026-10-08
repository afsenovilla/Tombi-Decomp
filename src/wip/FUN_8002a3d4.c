// FUNC 8002a3d4 252 MAIN0
#include "TOBJ.H"
extern unsigned char DAT_800a6038;
extern unsigned char DAT_800a6038b[];
extern short *DAT_800a6078;
extern int DAT_8009c960;
extern void FUN_80027c74(TObj *);
extern void FUN_8002795c(TObj *);

void FUN_8002a3d4(TObj *o)
{
    short a, b;
    short c;
    short *q;
    unsigned char *g;
    FUN_80027c74(o);
    FUN_8002795c(o);
    g = DAT_800a6038b;
    if (DAT_800a6038 < 4 || DAT_800a6038 == 7) {
        b = *(short *)(o->d34 + 2);
        a = b - 0x90;
        b = b + 0x90;
        if (*(short *)(DAT_800a6078 + 1) < a || (a = b, b < *(short *)(DAT_800a6078 + 1)))
            *(short *)(DAT_800a6078 + 1) = a;
        if (DAT_8009c960 == 0x2000e) {
            c = *(short *)&o->d30;
            q = (short *)(g + 0x16);
            if (*q < c - 0x5c)
                *q = c - 0x5c;
        } else {
            c = *(short *)&o->d30;
            q = (short *)(g + 0x16);
            if (*q < c - 0x90)
                *q = c - 0x90;
        }
    }
}
