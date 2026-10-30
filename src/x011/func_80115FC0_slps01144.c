// VERSION SLPS_01144 x11 80115FC0 336
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"
extern unsigned char D_8009ECA8;
extern unsigned char D_8009E9E2;
extern int ObjCullRegister(TObj *);
extern void AnimAdvance(TObj *);
extern void addItemToInventory(int, int, int);
extern void freeObjectLayer5(TObj *);

void func_80115FC0_slps01144(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (D_8009ECA8 != 0 || D_8009E9E2 == 0) {
            o->b04 = 3;
            break;
        }
        {
            volatile int *g = &SPAD.i334;
            int *q = (int *)*g;
            int v;
            unsigned char t;
            q += 8;
            v = *g + *q;
            o->ba4 = 0;
            o->box0 = 8;
            o->box1 = 0x10;
            o->box2 = 8;
            o->box3 = 0x10;
            o->active = 1;
            o->d84 = 0x80;
            o->b69 = 0;
            t = *(volatile unsigned char *)&o->b04;
            t++;
            o->b04 = t;
            o->da0 = v;
        }
        break;
    case 1:
        if (ObjCullRegister(o)) AnimAdvance(o);
        break;
    case 2:
        if (D_8009ECA8 == 0) addItemToInventory(0x5c, 1, 1);
        o->b04++;
        break;
    case 3:
        freeObjectLayer5(o);
        break;
    }
}
#endif
