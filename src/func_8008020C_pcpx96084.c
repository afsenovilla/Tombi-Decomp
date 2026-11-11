// VERSION PCPX_96084 tomba 8008020C 264
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Six-part chain (head kind 0): steps the head (func_80080314) and the parts along +0x94 (func_80080584, the head's
// state copied), then pushes them to the draw list head first or, mirrored, back from the tail along +0x90.
#include "TOBJ.H"
extern void func_80080314(TObj *);
extern void func_80080584(TObj *);
extern void ObjListPush_1F80021C(TObj *);

void func_8008020C_pcpx96084(TObj *o)
{
    TObj *p;
    int i;
    if (o->b0c != 0) return;
    func_80080314(o);
    p = (TObj *)o->d94;
    for (i = 1; i < 6; i++) {
        func_80080584(p);
        p->state = o->state;
        if (p->d94) p = (TObj *)p->d94;
    }
    if ((o->animFrame >> 1) & 1) {
        ObjListPush_1F80021C(o);
        p = (TObj *)o->d94;
        for (i = 1; i < 6; i++) {
            ObjListPush_1F80021C(p);
            if (p->d94) p = (TObj *)p->d94;
        }
    } else {
        for (i = 6; i != 0; i--) {
            if (p->visible == 1) ObjListPush_1F80021C(p);
            p = (TObj *)p->d90;
        }
    }
}
#endif
