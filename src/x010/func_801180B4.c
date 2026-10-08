// FUNC 801180b4 212 X010
// MATCHING 801180b4 212
#include "TOBJ.H"

extern void *D_80131C60;
extern int D_1F8002D4[];
extern void AnimLoadDuration(TObj *);
extern int ObjCullRegister(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjFree(TObj *);

void func_801180B4(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 0xc;
        o->b0d = 0x80;
        o->d3c = D_1F8002D4[0];
        o->anim = D_80131C60;
        AnimLoadDuration(o);
        *(signed char *)&o->b0f = 4;
        break;
    case 1:
        if (ObjCullRegister(o))
            AnimAdvance(o);
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
