// FUNC 8011b0d4 268 X000
// MATCHING 8011b0d4 268
#include "TOBJ.H"
extern int DAT_1f8002d4[];
extern void *DAT_8013b11c[];
extern int FUN_800202b4();
extern void FUN_8001fec0();
extern void FUN_8001fe6c();
extern void FUN_800187e4();

void FUN_8011b0d4(TObj *o)
{
    unsigned char t = o->b04;
    int iv;
    void *pv;
    switch (t) {
    case 0:
        o->b04 = t + 1;
        o->b0a = 1;
        o->d64 = 0x1400;
        o->animFrame = 1;
        o->b0d = 0;
        o->w1e = 0xc;
        iv = DAT_1f8002d4[0];
        o->b0f = 0;
        pv = DAT_8013b11c[0];
        o->d3c = iv;
        o->anim = pv;
        FUN_8001fe6c(o);
        o->timer = 0x3c;
        break;
    case 1:
        if (FUN_800202b4(o)) {
            FUN_8001fec0(o);
            if (--o->timer != -1)
                break;
        }
        o->b04 = 3;
        break;
    case 2:
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
