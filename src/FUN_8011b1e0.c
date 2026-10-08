// FUNC 8011b1e0 264 X000
// MATCHING 8011b1e0 264
#include "TOBJ.H"
extern void *PTR_DAT_8013b130;
extern int DAT_1f8002e8;
extern void FUN_800202b4(TObj *o);
extern void FUN_800187e4(TObj *o);

void FUN_8011b1e0(TObj *o)
{
    unsigned char b = o->b04;
    int v;
    void *p;
    switch (b) {
    case 0:
        switch (o->step) {
        case 0:
            v = DAT_1f8002e8;
            p = PTR_DAT_8013b130;
            o->w1e = 1;
            o->b0a = 2;
            o->b0d = 0;
            *((unsigned char *)o + 0x68) = 0;
            o->b0f = 5;
            o->animFrame = 0;
            o->d3c = v;
            o->anim = p;
            o->step++;
            break;
        case 1:
            o->b04 = b + 1;
            o->step = 0;
            o->state = 0;
            o->substep = 0;
        }
        break;
    case 1:
        FUN_800202b4(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        FUN_800187e4(o);
    }
}
