// FUNC 801228bc 212 X000
// MATCHING 801228bc 212
#include "TOBJ.H"
extern unsigned short DAT_8009c962;
extern int DAT_8009c984;
extern unsigned char DAT_8009cda6;
extern unsigned char DAT_8009d2af;
extern int DAT_8009f0ec;
extern void FUN_801340c4(int, int, int);
extern void FUN_8011a148(int, int, int, int);

void FUN_801228bc(TObj *o)
{
    unsigned short *p = &DAT_8009c962;
    if (*p == 4)
        o->b0f = 8;
    if (*p == 5)
        *(signed char *)&o->b0f = -5;
    if ((DAT_8009c984 & 2) != 0)
        FUN_801340c4(o->a.p.whole, o->y.p.whole, o->b.p.whole);
    if (DAT_8009cda6 != 0xff)
        FUN_8011a148(0, 0, 0, 6);
    if (DAT_8009d2af == 0) {
        o->b04 = 5;
        o->step = 0;
        o->state = 0;
        o->substep = 0;
    }
    DAT_8009f0ec = 0;
}
