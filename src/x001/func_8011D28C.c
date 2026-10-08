// FUNC 8011d28c 580 X001
// MATCHING 8011d28c 580
#include "TOBJ.H"
typedef struct {
    short w00, w02, w04, w06, w08, w0a, w0c, w0e, w10, w12, w14, w16, w18, w1a, w1c, w1e, w20;
} D22;

extern D22 D_800A4470[];
extern void playSFX(int);
extern int rcos(int);
extern int rsin(int);

void func_8011D28C(TObj *o)
{
    D22 *e;

    e = &D_800A4470[o->subtype];
    if (e->w1a < e->w1c) {
        if (e->w1e == 1) playSFX(0x4b);
        if (e->w1e < 0x20) e->w1e++;
        e->w1a += e->w1e >> 2;
        e->w02 = e->w08 - rcos(e->w1a) * e->w0c / 4096;
        e->w00 = e->w06 + rsin(e->w1a) * e->w0c / 4096;
        e->w12 = (e->w1a - 0x1000) / 2 + 100;
    }
    e = &D_800A4470[o->subtype + 1];
    if (e->w1a > e->w18) {
        if (e->w1e == 1) playSFX(0x4c);
        if (e->w1e < 0x20) e->w1e++;
        e->w1a -= e->w1e >> 2;
        e->w02 = e->w08 - rcos(e->w1a) * e->w0c / 4096;
        e->w00 = e->w06 + rsin(e->w1a) * e->w0c / 4096;
        e->w12 = (e->w1a - 0x1000) / 2 + 100;
    }
}
