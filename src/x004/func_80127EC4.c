// FUNC 80127ec4 508 X004
// MATCHING 80127ec4 508
#include "TOBJ.H"

extern void *D_80134D84;
extern int DAT_1f8002d4[];
extern short D_1F80016A;
extern int FUN_800202b4(TObj *);
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);
extern void func_80127B14(TObj *);
extern void func_80127D54(TObj *);
extern void FUN_80018790(TObj *);

void func_80127EC4(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->b0d = 0;
        o->animFrame = 1;
        o->w1e = 9;
        o->anim = D_80134D84;
        o->d3c = DAT_1f8002d4[0];
        o->d30 = o->h->p.whole;
        o->d34 = o->y.p.whole;
        o->d38 = o->d->p.whole;
        break;
    case 1:
        if (FUN_800202b4(o) == 0)
            break;
        switch (o->step) {
        case 0:
            switch (o->state) {
            case 0:
                o->state++;
                o->velV = -0x20;
                o->velY = 1;
                o->anim = D_80134D84;
                FUN_8001fe6c(o);
            case 1:
                FUN_8001fec0(o);
                o->y.raw += o->velV << 8;
                o->velV += o->velY;
                if ((unsigned short)(o->velV + 0x1f) >= 0x3f)
                    o->velY *= -1;
                if (D_1F80016A >= 0x83) {
                    o->step = 1;
                    o->state = 0;
                }
                break;
            }
            break;
        case 1:
            func_80127B14(o);
            break;
        case 2:
            func_80127D54(o);
            break;
        }
        break;
    case 2:
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
