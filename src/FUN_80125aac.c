// FUNC 80125aac 368 X000
// MATCHING 80125aac 368
#include "TOBJ.H"
extern short FUN_8004461c(TObj *a, TObj *b);
extern void FUN_8001f96c(int a, int b, int c, int d);
extern short DAT_1f80019e;

void FUN_80125aac(TObj *o, TObj *p)
{
    if (FUN_8004461c(o, p) == -1)
        return;
    switch (o->type) {
    case 0:
    case 9:
        o->wa8 -= 0x200;
        p->b6a = 1;
        if (o->wa8 < 0x500) {
            o->wa8 = 0x4ff;
            o->ba5 = 0;
            o->active = 2;
            DAT_1f80019e = 0;
        }
        break;
    case 1:
        o->ba5 = 0;
        o->active = 2;
        p->b6a = 1;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        break;
    case 5:
    case 6:
    case 7:
        o->b6a = 1;
        o->active = 2;
        o->wa8 -= 0x200;
        p->b6a = 1;
        if (o->wa8 < 0x500) {
            o->wa8 = 0x4ff;
            o->ba5 = 0;
            o->active = 2;
            DAT_1f80019e = 0;
        }
        break;
    case 10:
        if (o->subtype != 2) {
            o->b6a = 1;
            o->active = 2;
            DAT_1f80019e = 0;
        }
        break;
    case 2:
    case 3:
    case 4:
    case 8:
        break;
    }
    if (o->type != 8) {
        p->active = 2;
        p->b04 = 2;
        p->step = 0;
        p->state = 0;
        FUN_8001f96c(0, o->a.p.whole, o->y.p.whole, o->b.p.whole);
    }
}
