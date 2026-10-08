// FUNC 800eb9f0 344 X003
// MATCHING 800eb9f0 344
#include "TOBJ.H"
extern void FUN_800202b4(TObj *o);
extern void FUN_8001fe6c(TObj *o);
extern void FUN_800187e4(TObj *o);
extern void *TBL_80012150[];
extern int D_1f8002d8[];
extern unsigned short D_1f8001f8;

void FUN_800eb9f0(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 0x14;
        *(signed char *)&o->b0f = -0x14;
        o->animFrame = 1;
        o->b0d = 0;
        o->d3c = D_1f8002d8[0];
        o->anim = TBL_80012150[o->b0c];
        FUN_8001fe6c(o);
        break;
    case 1:
        FUN_800202b4(o);
        break;
    case 2:
        switch (o->step) {
        case 0:
            o->timer = 0x1e;
            o->step++;
            break;
        case 1:
            if ((D_1f8001f8 >> 2) & 1)
                FUN_800202b4(o);
            if (--o->timer == -1)
                o->b04 = 3;
            break;
        }
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
