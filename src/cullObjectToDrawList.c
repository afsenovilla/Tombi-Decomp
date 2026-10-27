// FUNC 80020078 308 MAIN0
// MATCHING 80020078 308
#include "TOBJ.H"
extern unsigned short SCREEN_ORIGIN_X;
extern unsigned short SCREEN_ORIGIN_Y;
void ObjListPush_1F800218(TObj *o);
void ObjListPush_1F80021C(TObj *o);
void ObjListPush_1F800220(TObj *o);
void ObjListPush_1F800224(TObj *o);
void ObjListPush_1F800228(TObj *o);
void ObjListPush_1F80022C(TObj *o);
void ObjListPush_1F800230(TObj *o);

int cullObjectToDrawList(TObj *o, int x)
{
    if (o->active == 0) return 0;
    o->visible = 0;
    if ((unsigned short)(o->h->p.whole - SCREEN_ORIGIN_X + x) > (short)x * 2 + 0x140) return 0;
    if ((unsigned short)(SCREEN_ORIGIN_Y - o->y.p.whole + x) > (short)x * 2 + 0xf0) return 0;
    o->visible = 1;
    switch (o->category & 0x7f) {
    case 1: ObjListPush_1F800218(o); return 1;
    case 2: ObjListPush_1F80021C(o); return 1;
    case 3: ObjListPush_1F800220(o); return 1;
    case 4: ObjListPush_1F800224(o); return 1;
    case 5: ObjListPush_1F800228(o); return 1;
    case 7: ObjListPush_1F80022C(o); return 1;
    case 8: ObjListPush_1F800230(o);
    }
    return 1;
}
