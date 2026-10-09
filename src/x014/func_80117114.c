// FUNC 80117114 456 X014
// MATCHING 80117114 456
#include "TOBJ.H"
extern void *D_80129FF4[];
extern int D_1F8002DC[];
extern unsigned short D_80125C20[], D_80125C28[];
extern int FUN_8001f9e0(void);
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);
extern int FUN_800202b4(TObj *);
extern void FUN_800187e4(TObj *);

void func_80117114(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->b0a = 2;
        o->w1e = 1;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b0f = 0;
        o->b0d = 0x80;
        o->d3c = D_1F8002DC[0];
        o->wac = 0;
        o->anim = D_80129FF4[o->subtype];
        FUN_8001fe6c(o);
        break;
    case 1:
        FUN_800202b4(o);
        switch (o->subtype) {
        case 0:
            if (FUN_8001fec0(o)) o->b04 = 3;
            break;
        case 1:
        case 2:
        case 3:
            switch (o->step) {
            case 0:
                o->timer = D_80125C20[FUN_8001f9e0() & 3];
                o->step++;
                o->velV = D_80125C28[o->subtype];
                break;
            case 1:
                o->y.raw -= o->velV << 8;
                if (--o->timer == -1) o->b04 = 3;
                break;
            }
            FUN_8001fec0(o);
            break;
        }
        break;
    case 2:
    case 3:
        FUN_800187e4(o);
        break;
    }
}
