// FUNC 8011ad24 376 X004
// MATCHING 8011ad24 376
#include "TOBJ.H"

extern int D_1F8002D4[];
extern void *D_80134DC8[];
extern unsigned char D_8009CEFB;
extern void FUN_8001fe6c(TObj *);
extern int FUN_800202b4(TObj *);
extern void FUN_8001ffa4(TObj *);
extern void FUN_800187e4(TObj *);

void func_8011AD24(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        if (o->subtype == 0) {
            o->w1e = 1;
            o->b0f = 2;
        } else {
            o->w1e = 0x11;
            o->b0a = 1;
            o->d64 = 0x1400;
            *(signed char *)&o->b0f = -12;
        }
        o->timer = 2;
        o->b0d = 0x80;
        {
            int t = D_1F8002D4[0];
            void *a = D_80134DC8[0];
            o->d3c = t;
            o->anim = a;
        }
        FUN_8001fe6c(o);
        break;
    case 1:
        if (o->subtype == 0) {
            if (D_8009CEFB >= 0x17) o->b04++;
        } else {
            o->d64 -= 0x18;
            if (--o->w22 == -1) o->b04 = 3;
        }
        if (FUN_800202b4(o)) FUN_8001ffa4(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
