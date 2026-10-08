// FUNC 801196f8 320 X003
// MATCHING 801196f8 320
#include "TOBJ.H"

extern unsigned short D_8009C962;
extern int D_1F8002D4;
extern void *D_80138EDC[];
extern void ObjListPush_1F800220(TObj *);
extern void func_801191C4(TObj *);
extern void func_801193D0(TObj *);
extern void ObjFree(TObj *);

void func_801196F8(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->b0d = 0x81;
        if (D_8009C962 == 3) {
            o->w1e = 8;
            o->w08 = 0x780c;
        } else {
            o->w1e = 9;
            o->w08 = 0x780b;
        }
        o->d3c = D_1F8002D4;
        o->anim = D_80138EDC[o->b0c];
        o->d->p.whole -= o->subtype * 4;
        break;
    case 1:
        o->visible = 1;
        ObjListPush_1F800220(o);
        if (D_8009C962 != 3 && D_8009C962 < 4) func_801191C4(o);
        else func_801193D0(o);
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
