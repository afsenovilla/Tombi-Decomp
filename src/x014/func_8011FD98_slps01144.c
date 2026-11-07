// VERSION SLPS_01144 x14 8011FD98 808
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"
extern unsigned char D_8009E4EA;
extern unsigned short D_8009E50A;
extern TObj *ObjAlloc(void);
extern int ObjCullRegister(TObj *);
extern void playSFX(int);
extern void func_80115B08_slps01144(short, int);
void func_8011FD98_slps01144(TObj *o)
{
    TObj *n;
    int i;
    short f;
    int d;

    if (D_8009E4EA == 1) {
        if (o->visible)
            ObjCullRegister(o);
        return;
    }
    {
        switch (o->state) {
        case 0:
            o->timer = 60;
            o->state++;
            break;
        case 1:
            if (--o->timer == -1) {
                o->visible = 1;
                o->timer = 8;
                o->velV = ((o->velY << 16) - o->y.raw) >> 11;
                o->state++;
            }
            break;
        case 2:
            o->d88 = (o->d88 - -0x80) & 0xfff;
            o->y.raw += o->velV << 8;
            if (--o->timer == 0) {
                o->state++;
                if (D_8009E50A == 7)
                    o->timer = 60;
                else
                    o->timer = 30;
                for (i = 0; i < 2; i++) {
                    n = ObjAlloc();
                    if (n) {
                        n->active = 2;
                        n->type = 0x25;
                        n->animFrame = i;
                        n->a.raw = o->a.raw;
                        n->y.raw = o->y.raw;
                        n->b.raw = o->b.raw;
                    }
                }
                if (D_8009E50A == 7)
                    playSFX(246);
                else
                    playSFX(234);
            }
            break;
        case 3:
            d = o->d88;
            f = SPAD.s1f8 & 1;
            o->d88 = d; o->d88 = (o->d88 + 0x80) & 0xfff;
            if (f)
                o->visible = 1;
            else
                o->visible = 0;
            if (--o->timer == 0) {
                if (D_8009E50A == 7) {
                    o->state = 5;
                } else {
                    o->timer = 60;
                    o->state++;
                }
            }
            break;
        case 4:
            o->subtype = 1;
            o->visible = 1;
            f = SPAD.s1f8 & 1;
            o->da0 = 0;
            if (f)
                func_80115B08_slps01144(o->wac, 0);
            else
                func_80115B08_slps01144(o->wac, 1);
            if (--o->timer == 0)
                o->state++;
            break;
        case 5:
            o->visible = 0;
            if (D_8009E50A != 7)
                func_80115B08_slps01144(o->wac, 0);
            o->active = 2;
            o->step = 0;
            o->b04++;
            break;
        }
    }
    if (o->visible)
        ObjCullRegister(o);
}
#endif
