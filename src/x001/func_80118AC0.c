// FUNC 80118ac0 348 X001
// MATCHING 80118ac0 348
#include "TOBJ.H"
#include "raw7.h"

extern void *D_8013E694[];
extern int D_1F8002D4;
extern void AnimLoadDuration(TObj *);
extern void ObjListPush_1F800220(TObj *);
extern void ObjFree(TObj *);

void func_80118AC0(TObj *o)
{
    unsigned char h;

    switch (o->b04) {
    case 0:
        if (o->subtype < 2) {
            o->w1e = 8;
        } else {
            o->w1e = 10;
        }
        o->b0d = 0;
        o->d3c = D_1F8002D4;
        o->anim = D_8013E694[o->subtype];
        h = U8(o, 0x6c) >> 4;
        o->b04 = o->b04 + 1;
        if (h < 12) {
            o->d64 = (h << 10) + 0x1000;
        } else {
            o->d64 = 0x1000 - ((h - 11) << 9);
        }
        h = U8(o, 0x6c) & 0xf;
        if (h < 8) {
            o->d8c = h << 2;
        } else {
            o->d8c = (-((int)(h - 7) << 5) / 8) & 0xff;
        }
        AnimLoadDuration(o);
        break;
    case 1:
        o->visible = 1;
        ObjListPush_1F800220(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
