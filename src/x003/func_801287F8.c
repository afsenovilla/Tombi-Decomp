// FUNC 801287f8 464 X003
// MATCHING 801287f8 464
#include "TOBJ.H"

extern int DAT_1f8002dc[];
extern void *D_80139514[];
extern char D_80077D0C[];
extern void FUN_8001fe6c(TObj *);
extern int FUN_800202b4(TObj *);
extern void FUN_8001fa88(TObj *, unsigned short);
extern void FUN_800187e4(TObj *);

void func_801287F8(TObj *o)
{
    int t;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 0xb;
        o->b0a = 2;
        o->d84 = 0;
        o->d88 = 0;
        o->d3c = DAT_1f8002dc[0];
        o->wac = 5;
        o->anim = D_80139514[0];
        FUN_8001fe6c(o);
        break;
    case 1:
        if (FUN_800202b4(o) != 0) {
            switch (o->step) {
            case 0:
                o->b0b = 1;
                o->b0f = 4;
                o->velV = -0x400;
                o->movetab = D_80077D0C;
                o->wac = 5;
                o->step++;
                o->anim = D_80139514[0];
                FUN_8001fe6c(o);
                break;
            case 1:
                FUN_8001fa88(o, 1 - o->animFrame);
                o->velV += 0x40;
                if (o->velV > 0x400)
                    o->velV = 0x400;
                o->y.raw += o->velV << 8;
                break;
            }
            if (o->animFrame & 1)
                t = o->d8c + 0x14;
            else
                t = o->d8c - 0x14;
            o->d8c = t & 0xff;
        } else {
            o->b04 = 3;
        }
        break;
    case 2:
    case 3:
        FUN_800187e4(o);
        break;
    }
}
