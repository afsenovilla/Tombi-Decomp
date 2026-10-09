// FUNC 8011a7e0 548 X009
// MATCHING 8011a7e0 548
#include "TOBJ.H"

extern unsigned char D_8009C93F, D_8009C940, D_8009C941, D_8009C942;
extern int D_1F8002D4[];
extern void *D_8012E060[];
extern void FUN_8001fe6c(TObj *);
extern void AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern int func_8011A6A0(TObj *);
extern void FUN_800187e4(TObj *);

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    FUN_8001fe6c(o);
}

void func_8011A7E0(TObj *o)
{
    unsigned char t = o->b04;
    TObj *p;

    switch (t) {
    case 0:
        o->b04 = t + 1;
        *(signed char *)&o->b0f = -11;
        o->w1e = 11;
        o->b0d = 0;
        o->d3c = D_1F8002D4[0];
        setAnim(o, D_8012E060[0]);
        break;
    case 1:
        switch (o->step) {
        case 0:
            if (D_8009C940 == 0) break;
            if (D_8009C941 != 0x75) break;
            if (func_8011A6A0(o)) {
                o->timer = 0x1e;
                o->step++;
            }
            break;
        case 1:
            p = (TObj *)o->d90;
            o->a.p.whole = p->a.p.whole;
            o->y.p.whole = p->y.p.whole - 0x10;
            o->b.p.whole = p->b.p.whole;
            AnimAdvance(o);
            if (--o->timer == -1) {
                o->step++;
                p->active = 2;
                p->step = 8;
                p->state = 0;
                p->subtype |= 0x80;
                o->timer = 100;
            }
            ObjCullRegister(o);
            break;
        case 2:
            AnimAdvance(o);
            if (--o->timer == -1) {
                o->step = 0;
                D_8009C942 = 0;
                D_8009C93F = 0;
                D_8009C940 = 0;
            }
            ObjCullRegister(o);
            break;
        }
        break;
    case 2:
    case 3:
        FUN_800187e4(o);
        break;
    }
}
