// VERSION PCPX_96084 tomba 800183FC 340
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of cullObjectToDrawList (src/cullObjectToDrawList.c): screen origin at 0x1F80028A / 0x1F800392, nine draw lists.
#include "TOBJ.H"
#define SCREEN_ORIGIN_X D_1F80028A
#define SCREEN_ORIGIN_Y D_1F800392
extern unsigned short SCREEN_ORIGIN_X;
extern unsigned short SCREEN_ORIGIN_Y;
void func_8001ABE4_pcpx96084(TObj *o);
void func_8001AC20_pcpx96084(TObj *o);
void func_8001AC5C_pcpx96084(TObj *o);
void ObjListPush_1F800218(TObj *o);
void ObjListPush_1F80021C(TObj *o);
void ObjListPush_1F800220(TObj *o);
void ObjListPush_1F800224(TObj *o);
void ObjListPush_1F800228(TObj *o);
void ObjListPush_1F80022C(TObj *o);
void ObjListPush_1F800230(TObj *o);

int func_800183FC(TObj *o, int x)
{
    if (o->active == 0) return 0;
    o->visible = 0;
    if ((unsigned short)(o->h->p.whole - SCREEN_ORIGIN_X + x) > (short)x * 2 + 0x140) return 0;
    if ((unsigned short)(SCREEN_ORIGIN_Y - o->y.p.whole + x) > (short)x * 2 + 0xf0) return 0;
    o->visible = 1;
    switch (o->category & 0x7f) {
    case 1: ObjListPush_1F800218(o); return 1;
    case 2: ObjListPush_1F80021C(o); return 1;
    case 3: ObjListPush_1F800224(o); return 1;
    case 4: ObjListPush_1F800230(o); return 1;
    case 5: ObjListPush_1F800228(o); return 1;
    case 6: func_8001ABE4_pcpx96084(o); return 1;
    case 7: func_8001AC20_pcpx96084(o); return 1;
    case 8: func_8001AC5C_pcpx96084(o); return 1;
    case 9: ObjListPush_1F80022C(o);
    }
    return 1;
}
#endif
