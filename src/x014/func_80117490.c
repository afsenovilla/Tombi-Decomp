// FUNC 80117490 272 X014
// MATCHING 80117490 272
#include "TOBJ.H"

extern int D_1F8002DC[];
extern void *D_8012A014;
extern void AnimLoadDuration(TObj *o);
extern int AnimAdvance(TObj *o);
extern void ObjCullRegister(TObj *o);
extern void ObjFree(TObj *o);

void func_80117490(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        *(signed char *)&o->b0f = -12;
        o->w1e = 1;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b0a = 0;
        o->b0d = 0x80;
        o->d3c = D_1F8002DC[0];
        o->wac = 0;
        o->anim = D_8012A014;
        AnimLoadDuration(o);
        o->timer = 0x78;
        break;
    case 1:
        if (--o->timer == -1) {
            o->b04 = 3;
            break;
        }
        ObjCullRegister(o);
        AnimAdvance(o);
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
