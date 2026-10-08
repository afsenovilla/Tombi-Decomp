// FUNC 8002c73c 484 MAIN0
// MATCHING 8002c73c 484
#include "TOBJ.H"
extern char D_80014764[];
extern unsigned short D_800A604A;
extern unsigned short D_800A603C[];
extern short D_800A604E[];
extern unsigned short D_800A6052;
extern unsigned short D_800A6066[];
extern Fix16 *D_800A6078[];
extern Fix16 *D_800A607C;
extern void AnimLoadDuration(TObj *o);
extern void ObjCullRegister(TObj *o);
extern void ObjFree(TObj *o);
void func_8002C73C(TObj *o)
{
    unsigned char s = o->b04;
    short t;
    switch (s) {
    case 0:
        if (o->step != 0) break;
        o->anim = D_80014764;
        AnimLoadDuration(o);
        o->a.p.whole = D_800A604A;
        if (D_800A603C[0] == 0x405) {
            o->y.p.whole = -0x24;
        } else if (D_800A604E[0] < -0xf1 && D_800A6078[0]->p.whole >= 0xf3) {
            o->y.p.whole = -0xf0;
        } else {
            o->y.p.whole = -0x28;
        }
        t = D_800A6052;
        o->b04 = 1;
        o->step = 0;
        o->b.p.whole = t;
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            o->step++;
        case 1:
            o->h->p.whole = D_800A6078[0]->p.whole;
            if (D_800A604E[0] < -0xf1 && D_800A6078[0]->p.whole >= 0xf3) {
                o->y.p.whole = -0xf0;
            } else {
                o->y.p.whole = -0x28;
            }
            o->d->p.whole = D_800A607C->p.whole;
            o->animFrame = D_800A6066[0] & 1;
            break;
        }
        break;
    case 2:
        o->b04 = s + 1;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
