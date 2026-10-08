// FUNC 8011ebd0 340 X006
// MATCHING 8011ebd0 340
#include "TOBJ.H"
extern unsigned short D_1F80017E, D_1F800176, D_1F800186;
extern void ObjListPush_1F800218(TObj *), ObjListPush_1F80021C(TObj *),
    ObjListPush_1F800220(TObj *), ObjListPush_1F800224(TObj *),
    ObjListPush_1F800228(TObj *), ObjListPush_1F80022C(TObj *),
    ObjListPush_1F800230(TObj *);

int func_8011EBD0(TObj *o)
{
    if (o->active == 0) return 0;
    o->visible = 0;
    if ((unsigned short)(o->d->p.whole - D_1F80017E + 0x190) >= 0x321) return 0;
    if ((unsigned short)(o->h->p.whole - D_1F800176 + 0xc8) >= 0x385) return 0;
    if ((unsigned short)(D_1F800186 - o->y.p.whole + 0xa0) >= 0x231) return 0;
    o->visible = 1;
    switch (o->category & 0x7f) {
    case 1: ObjListPush_1F800218(o); return 1;
    case 2: ObjListPush_1F80021C(o); return 1;
    case 3: ObjListPush_1F800220(o); return 1;
    case 4: ObjListPush_1F800224(o); return 1;
    case 5: ObjListPush_1F800228(o); return 1;
    case 7: ObjListPush_1F80022C(o); return 1;
    case 8: ObjListPush_1F800230(o);
    default: return 1;
    }
}
