// VERSION SLPS_01144 info3 800EB6CC 412
// JP version of PAL i0nfo3 800e9e68 (tunnelPlayerDropExitStep.c)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
#include "raw7.h"
extern int func_80020E38(TObj *o);
extern void func_8001E2D4(int s);
extern void func_8001F050(int a);
extern TObj *D_8009DEE0;
extern unsigned char D_8009E51D[];
extern unsigned char D_8009EE58;

void func_800EB6CC(TObj *o)
{
    void *p;
    switch (o->state) {
    case 0:
        D_8009DEE0->timer = 0;
        o->timer = 0x1E;
        o->d8c = 0;
        S16(o, 0xD0) = 0;
        o->w22 = 0;
        o->d8c = 0;
        o->velX = 0;
        o->velY = 0;
        D_8009EE58 = 3;
        o->d8c = 0;
        func_8001F050(1);
        o->state++;
        break;
    case 1:
        if (o->substep != 0 && (*(unsigned short *)0x1F8001F8 & 3) == 0) {
            func_8001E2D4(0x16);
        }
        func_80020E38(o);
        o->y.raw += o->velY << 8;
        o->velY -= 0x30;
        if (--o->timer <= 0) {
            o->timer = 0x3C;
            D_8009E51D[0] = 3;
            o->state++;
        }
        break;
    case 2:
        func_80020E38(o);
        o->y.raw += o->velY << 8;
        o->velY -= 0x30;
        if (o->timer != 0) {
            if (--o->timer <= 0) {
                p = *(void **)0x1F8001D4;
                S16(p, 0x4C) = 7;
                S16(p, 0x4E) = 0;
            }
        }
        break;
    }
}
#endif
