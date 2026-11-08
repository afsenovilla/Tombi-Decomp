// JP-only (x06), written from the disassembly
// VERSION SLPS_01144 x06 8011DD3C 644
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"
extern char D_800716F4[];
extern void *D_8011FBD4[];
extern void *D_8011FBD8;
extern char D_8012302C[];
extern char D_8012303C[];
extern char D_80123070[];
extern char D_80123084[];
extern char D_801230A8[];
extern char D_801230CC[];
extern char D_801230F0[];
extern char D_80123118[];
extern char D_80123134[];
extern void *D_8011FBE0[9];
extern void func_80020DE4(TObj *);
extern int func_80020E38(TObj *);
extern void applyAnimVelocityX(TObj *, unsigned short);
extern void freeObjectLayer2(TObj *);
extern void cullObjectToDrawList(TObj *, int);

static __inline__ void setbox(TObj *o, short a, short b, short c, short d)
{
    o->box0 = a;
    o->box1 = b;
    o->box2 = c;
    o->box3 = d;
}

void func_8011DD3C_slps01144(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 3;
        *(signed char *)&o->b0f = -0x10;
        o->b0a = 2;
        o->b0d = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        setbox(o, 0x10, 0x20, 0x10, 0x20);
        o->d3c = ((int*)&SPAD.i2d0)[0];
        o->anim = D_8011FBD4[o->b0c];
        func_80020DE4(o);
        break;
    case 1:
        cullObjectToDrawList(o, 160);
        func_80020E38(o);
        break;
    case 2:
        cullObjectToDrawList(o, 160);
        switch (o->step) {
        case 0:
            o->b0b = 1;
            o->b0f = 4;
            o->active = 2;
            o->velV = -0x400;
            o->movetab = D_800716F4;
            o->step++;
            o->anim = D_8011FBE0[0];
            func_80020DE4(o);
            break;
        case 1:
            if (o->velV > 0) {
                o->anim = D_8011FBD8;
                func_80020DE4(o);
                o->step++;
            }
        case 2:
            func_80020E38(o);
            applyAnimVelocityX(o, 1 - o->animFrame);
            o->velV += 0x40;
            if (o->velV > 0x400)
                o->velV = 0x400;
            o->y.raw += o->velV << 8;
            if (!o->visible)
                o->b04++;
            break;
        }
        if (o->animFrame & 1)
            o->d8c = (o->d8c + 0x14) & 0xff;
        else
            o->d8c = (o->d8c - 0x14) & 0xff;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
#endif
