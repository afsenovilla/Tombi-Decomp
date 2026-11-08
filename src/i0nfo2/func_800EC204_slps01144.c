// VERSION SLPS_01144 info2 800EC204 356
// JP version of PAL i0nfo2 800eaaa0 (topdownPlayerDropExitStep.c)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
extern void func_8001F050(int a);
extern void func_80020E38(TObj *o);
extern TObj *D_8009DEE0;
extern unsigned char D_8009E51D[];
extern unsigned char D_8009EE58[];

void func_800EC204(TObj *o)
{
    TObj *p;
    switch (o->state) {
    case 0:
        o->active = 4;
        D_8009DEE0->timer = 0;
        o->timer = 0x14;
        o->d8c = 0;
        *(short *)((char *)o + 0xD0) = 0;
        o->w22 = 0;
        o->d8c = 0;
        o->velX = 0;
        o->velY = 0;
        D_8009EE58[0] = 3;
        o->d8c = 0;
        func_8001F050(1);
        o->state++;
        break;
    case 1:
        func_80020E38(o);
        o->y.raw += o->velY << 8;
        o->velY -= 8;
        if (--o->timer <= 0) {
            D_8009E51D[0] = 3;
            o->state++;
        }
        break;
    case 2:
        func_80020E38(o);
        o->y.raw += o->velY << 8;
        o->velY -= 8;
        if (D_8009E51D[0] == 1) {
            p = *(TObj **)0x1F8001D4;
            p->w4c = 7;
            p->w4e = 0;
        }
        break;
    }
}
#endif
