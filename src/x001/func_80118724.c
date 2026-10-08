// FUNC 80118724 328 X001
// MATCHING 80118724 328
#include "TOBJ.H"
#include "raw7.h"

extern void *D_8013E5CC[];
extern int D_1F8002D4[];
extern void func_8011886C(TObj *);
extern int ObjCullRegister(TObj *);
extern void ObjFree(TObj *);

void func_80118724(TObj *o)
{
    short t;

    switch (o->b04) {
    case 0:
        t = o->w22;
        if (t != 0) {
            o->w22 = t - 1;
            break;
        }
        o->b04++;
        o->w1e = 0xb;
        o->b0d = 0;
        o->d3c = D_1F8002D4[0];
        o->anim = D_8013E5CC[o->subtype];
        if (o->subtype == 2) {
            S16(o, 0xb8) = -2;
            S16(o, 0xc8) = -2;
            S16(o, 0xb6) = 0;
            S16(o, 0xb4) = 0;
            S16(o, 0xc0) = 2;
            S16(o, 0xbe) = 0;
            S16(o, 0xbc) = 0;
            S16(o, 0xc6) = 0x20;
            S16(o, 0xc4) = 0;
            S16(o, 0xd0) = 2;
            S16(o, 0xce) = 0x20;
            S16(o, 0xcc) = 0;
        }
        break;
    case 1:
        switch (o->subtype) {
        case 1:
            func_8011886C(o);
            break;
        case 2:
            ObjCullRegister(o);
            break;
        }
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
