// VERSION SLPS_01144 x10 80119B8C 528
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "tobj.h"
#include "spad.h"

extern unsigned char D_8009EE6B[];
extern void *D_8012DE7C;
extern void *D_8012DE34[];
extern int D_800A60BC;
extern int D_800A60B8;
extern unsigned short GAME_SELECTED_SECTION;
void ObjListPush_1F80022C(TObj *o);
void freeObjectLayer7(TObj *o);
void AnimLoadDuration(TObj *o);
int AnimAdvance(TObj *o);

void func_80119B8C_slps01144(TObj *o)
{
    short x, y;
    switch (o->b04) {
    case 0:
        o->b0d = 0;
        if (o->subtype == 99) {
            if (D_8009EE6B[0] & 0x40) {
                o->b04 = 3;
                break;
            }
            o->b0f = 0;
            o->w1e = 12;
            o->anim = D_8012DE7C;
            AnimLoadDuration(o);
        } else {
            o->w1e = 13;
            o->anim = D_8012DE34[o->subtype];
            o->b0f = 3;
        }
        o->d3c = SPAD.i2d4;
        o->d30 = o->a.p.whole;
        o->d34 = o->y.p.whole;
        o->b04++;
        break;
    case 1:
        if (o->subtype == 99) AnimAdvance(o);
        x = o->d30 - *(unsigned short *)((char *)&SPAD.i174 + 2) + ((D_800A60BC >> 8) << 2);
        y = o->d34 - SPAD.s17a - ((D_800A60B8 >> 8) << 3);
        switch (GAME_SELECTED_SECTION) {
        case 0:
        case 2:
        case 4:
        case 6:
            x >>= 2;
            y >>= 1;
            break;
        case 1:
        case 5:
            x >>= 1;
            y >>= 1;
            break;
        }
        o->a.p.whole = x;
        o->y.p.whole = y;
        o->b.p.whole = 0;
        if (o->a.p.whole - 0x40 < 0x141 && o->a.p.whole + 0x40 >= 0) {
            o->visible = 1;
            ObjListPush_1F80022C(o);
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer7(o);
        break;
    }
}
#endif
