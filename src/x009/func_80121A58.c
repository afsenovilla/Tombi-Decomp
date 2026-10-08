// FUNC 80121a58 724 X009
// MATCHING 80121a58 724
#include "TOBJ.H"

extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern short D_800A4582;
extern void *D_8012EA38[];
extern void *D_8012EA3C[];
extern void *D_8012E9F8[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern int func_800203DC(TObj *);
extern void playSFX(int);

void func_80121A58(TObj *o)
{
    TObj *p;

    switch (o->state) {
    case 0:
        o->active = 7;
        o->b6a = 1;
        o->wb4 = 3;
        o->w22 = 1;
        o->w76 = 0;
        o->velV = 0;
        o->wac = 0x10;
        o->state++;
        o->anim = D_8012EA38[0];
        AnimLoadDuration(o);
        if (func_800203DC(o)) {
            playSFX(0x98);
        }
        if ((o->subtype & 0x7f) == 0) {
            p = (TObj *)o->d90;
            o->h->p.whole = o->wb6 + p->h->p.whole;
            o->y.p.whole = o->wb8 + p->y.p.whole;
            o->d->p.whole = o->wba + p->d->p.whole;
        } else {
            o->h->p.whole = o->wb6;
            o->y.p.whole = o->wb8;
            o->d->p.whole = o->wba;
        }
        break;
    case 1:
        if ((D_1F8001F8 + D_1F800198) & 3) {
            ObjCullRegister(o);
        }
        AnimAdvance(o);
        o->velV += 0x20;
        if (o->velV > 0x400) {
            o->velV = 0x400;
        }
        o->w76 += o->velV;
        if (o->w76 > 0x3000) {
            o->w22 = 0;
        }
        o->y.raw -= o->velV << 8;
        if (o->w22 == 0) {
            o->wac = 0x11;
            o->state++;
            o->anim = D_8012EA3C[0];
            AnimLoadDuration(o);
        }
        if (((D_1F8001F8 + D_1F800198) & 0xf) == 0 && D_800A4582 + 0xa0 < o->y.p.whole) {
            o->b04 = 3;
            o->step = 0;
            o->state = 0;
        }
        break;
    case 2:
        if (AnimAdvance(o)) {
            o->wb4 = 3;
            o->state = 0;
            o->wac = 0;
            o->step++;
            o->anim = D_8012E9F8[0];
            AnimLoadDuration(o);
        }
        if ((D_1F8001F8 + D_1F800198) & 3) {
            ObjCullRegister(o);
        }
        break;
    }
}
