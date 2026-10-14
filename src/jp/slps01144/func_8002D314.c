// JP slps01144 8002D314 484
// Japanese version of PAL main0 8002C73C (func_8002C73C.c): -40 -> -36.
#include "player.h"
#include "TOBJ.H"
extern char D_80014764[];
extern unsigned short D_800A603C[];
extern Fix16 *D_800A6078[];
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
        o->a.p.whole = (*(unsigned short*)&PLAYER.obj.a.p.whole);
        if (D_800A603C[0] == 0x405) {
            o->y.p.whole = -0x24;
        } else if (((short*)&PLAYER.obj.y.p.whole)[0] < -0xf1 && D_800A6078[0]->p.whole >= 0xf3) {
            o->y.p.whole = -0xf0;
        } else {
            o->y.p.whole = -0x24;
        }
        t = (*(unsigned short*)&PLAYER.obj.b.p.whole);
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
            if (((short*)&PLAYER.obj.y.p.whole)[0] < -0xf1 && D_800A6078[0]->p.whole >= 0xf3) {
                o->y.p.whole = -0xf0;
            } else {
                o->y.p.whole = -0x24;
            }
            o->d->p.whole = (*(Fix16 **)&PLAYER.obj.d)->p.whole;
            o->animFrame = ((unsigned short*)&PLAYER.obj.animFrame)[0] & 1;
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
