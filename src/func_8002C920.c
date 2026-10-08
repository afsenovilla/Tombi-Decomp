// FUNC 8002c920 360 MAIN0
// MATCHING 8002c920 360
#include "TOBJ.H"
extern void *D_800121F4[];
extern unsigned short D_800A6066;
extern short D_800A604A;
extern unsigned short D_800A604E;
extern unsigned short D_800A6052[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjCullRegister(TObj *);
extern void ObjFree(TObj *);

void func_8002C920(TObj *o)
{
    unsigned char t = o->b04;
    int x;

    switch (t) {
    case 0:
        if (o->step == 0) {
            o->anim = D_800121F4[o->subtype];
            AnimLoadDuration(o);
            x = D_800A604A;
            if (D_800A6066 & 1) {
                o->a.p.whole = x + 8;
            } else {
                o->a.p.whole = x - 8;
            }
            o->y.p.whole = D_800A604E + 0x10;
            o->b.p.whole = D_800A6052[0];
            o->b04 = 1;
            o->step = 0;
        }
        break;
    case 1:
        ObjCullRegister(o);
        if (o->visible != 0) {
            switch (o->step) {
            case 0:
                o->step++;
            case 1:
                if (AnimAdvance(o)) {
                    o->b04 = 2;
                }
                break;
            }
        }
        break;
    case 2:
        o->b04 = t + 1;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
