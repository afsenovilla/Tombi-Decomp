// FUNC 80120e24 296 X001
// MATCHING 80120e24 296
#include "TOBJ.H"

extern unsigned short D_1F800186;
extern void ObjListPush_1F80022C(TObj *);

void func_80120E24(TObj *o)
{
    switch (o->b0c) {
    case 0:
        o->b0d = 0x80;
        if (o->d30 < -0x3c0000) o->d30 = 0x17c0000;
        o->d30 -= o->animFrame << 4;
        o->a.raw = o->d30;
        break;
    case 1:
        if (o->d30 < -0x280000) o->d30 = 0x1680000;
        o->d30 -= o->animFrame << 4;
        o->a.raw = o->d30;
        break;
    case 2:
        o->a.p.whole = (unsigned int)o->d30 >> 4;
        break;
    case 3:
        o->a.p.whole = (unsigned int)o->d30 >> 3;
        break;
    case 4:
        o->b0d = 0x80;
        *(signed char *)&o->b0f = -1;
        if (o->d30 < -0x3c0000) {
            o->d30 = 0x17c0000;
        } else if (o->d30 < 0xa0) {
            o->animFrame = 0x800;
        }
        o->d30 -= o->animFrame << 4;
        o->a.raw = o->d30;
        break;
    }
    {
        int y = o->d34;
        o->y.p.whole = ((unsigned int)(y - D_1F800186) & 0xfff) >> 4;
    }
    o->visible = 1;
    ObjListPush_1F80022C(o);
}
