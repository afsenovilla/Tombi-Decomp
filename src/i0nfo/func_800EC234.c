// FUNC 800ec234 292 I0NFO
// MATCHING 800ec234 292
#include "TOBJ.H"
extern void func_800EB384();
extern int func_8005EFE4(void);
extern void func_800EBEDC(TObj *o);
extern void func_800EC084(TObj *o);
extern void func_80018DA4(TObj *o);
extern void func_80018934(TObj *o);

void func_800EC234(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_800EB384();
        o->b0a = 3;
        o->w1e = (func_8005EFE4() == 1 || func_8005EFE4() == 2) ? 0x20C : 0x8C;
        o->w08 = 0x7F88;
        o->d30 = 0x20;
        o->d34 = 0x20;
        o->b0f = 0;
        o->h->p.whole = 0;
        o->y.p.whole = 0;
        func_800EBEDC(o);
        o->b6b = 0x80;
        o->b04++;
        break;
    case 1:
        func_800EC084(o);
        o->visible = 1;
        func_80018DA4(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_80018934(o);
        break;
    }
}
