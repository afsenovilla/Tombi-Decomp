// FUNC 800ede4c 364 I0NFO
// MATCHING 800ede4c 364
#include "TOBJ.H"
extern void func_80018790(TObj *o);
extern void func_800202B4(TObj *o);
extern void overheadActorStep(TObj *o);
extern void overheadActorInit(TObj *o);
extern void overheadActorInitSimple(TObj *o);
extern void overheadFollowerStep(TObj *o);
extern signed char D_8009D2B0[];

void overheadActorHandler(TObj *o)
{
    TObj *p;
    unsigned char v;
    switch (o->b04) {
    case 0:
        if (o->subtype == 1 || o->subtype == 3) {
            overheadActorInitSimple(o);
        } else {
            overheadActorInit(o);
        }
        break;
    case 1:
        switch (o->subtype) {
        case 1:
            func_800202B4(o);
            p = (TObj *)o->d90;
            o->a.raw = p->a.raw;
            if (D_8009D2B0[0] != 3) {
                o->y.raw = p->y.raw;
            }
            o->b.raw = p->b.raw;
            if (p->category == 0) v = 1;
            else v = p->visible;
            o->visible = v;
            o->b0f = 0x10;
            break;
        case 3:
            overheadFollowerStep(o);
            break;
        default:
            overheadActorStep(o);
            break;
        }
        break;
    case 2:
        func_800202B4(o);
        break;
    case 3:
        o->visible = 1;
        func_80018790(o);
        break;
    }
}
