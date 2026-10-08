// FUNC 8002a3d4 252 MAIN0
// MATCHING 8002a3d4 252
#include "TOBJ.H"
extern unsigned char DAT_800a6038;
extern unsigned char DAT_800a6038b[];
extern short *DAT_800a6078;
extern int DAT_8009c960;
extern void FUN_80027c74(TObj *);
extern void FUN_8002795c(TObj *);
static __inline__ void lim(short *q, short x, int k) { if (*q < x - k) *q = x - k; }
void FUN_8002a3d4(TObj *o)
{
    short a, b;
    unsigned char *g;
    FUN_80027c74(o);
    FUN_8002795c(o);
    g = DAT_800a6038b;
    if (DAT_800a6038 < 4 || DAT_800a6038 == 7) {
        b = *(short *)(o->d34 + 2);
        if (DAT_800a6078[1] < (a = b - 0x90)) DAT_800a6078[1] = a;
        else if ((a = b + 0x90) < DAT_800a6078[1]) DAT_800a6078[1] = a;
        if (DAT_8009c960 == 0x2000e) {
            lim((short *)(g + 0x16), *(short *)&o->d30, 0x5c);
        } else {
            lim((short *)(g + 0x16), *(short *)&o->d30, 0x90);
        }
    }
}
