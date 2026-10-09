// FUNC 8011ed24 640 X006
// MATCHING 8011ed24 640
#include "TOBJ.H"
extern int D_1F8002D0[];
extern char D_80077D0C[];
extern void *D_80122FB8[];
extern void *D_80122FBC;
extern void *D_80122FC4[];
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);
extern void FUN_8001fa88(TObj *, unsigned short);
extern void FUN_80018790(TObj *);
extern void func_8011EBD0(TObj *);

static __inline__ void setbox(TObj *o, short a, short b, short c, short d)
{
    o->box0 = a;
    o->box1 = b;
    o->box2 = c;
    o->box3 = d;
}

void func_8011ED24(TObj *o)
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
        o->d3c = D_1F8002D0[0];
        o->anim = D_80122FB8[o->b0c];
        FUN_8001fe6c(o);
        break;
    case 1:
        func_8011EBD0(o);
        FUN_8001fec0(o);
        break;
    case 2:
        func_8011EBD0(o);
        switch (o->step) {
        case 0:
            o->b0b = 1;
            o->b0f = 4;
            o->active = 2;
            o->velV = -0x400;
            o->movetab = D_80077D0C;
            o->step++;
            o->anim = D_80122FC4[0];
            FUN_8001fe6c(o);
            break;
        case 1:
            if (o->velV > 0) {
                o->anim = D_80122FBC;
                FUN_8001fe6c(o);
                o->step++;
            }
        case 2:
            FUN_8001fec0(o);
            FUN_8001fa88(o, 1 - o->animFrame);
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
        FUN_80018790(o);
        break;
    }
}
