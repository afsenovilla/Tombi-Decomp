// FUNC 801246d0 516 X009
// MATCHING 801246d0 516
#include "TOBJ.H"

extern unsigned char D_8009D11D[];
extern int D_1F8002D4;
extern void *D_8012E04C;
extern unsigned char D_8009CFE5[];
extern unsigned char D_8009CDCA;
extern unsigned char D_8009C964;
extern unsigned char D_8009C93A;
extern void FUN_8001fe6c(TObj *);
extern void AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern void func_8012455C(TObj *);
extern void FUN_80018790(TObj *);

void func_801246D0(TObj *o)
{
    int t = o->b04;
    int s;

    switch (t) {
    case 0:
        o->active = 1;
        o->box0 = 0x10;
        o->box1 = 0x20;
        o->box2 = 8;
        o->box3 = 0x10;
        o->w1e = 9;
        o->b0d = 0;
        o->b0a = 0;
        if (D_8009D11D[0] == 0) {
            o->b0d = 0x80;
            o->active = 2;
            o->b6b = 0x4f;
            o->b0a = 0xd;
        }
        o->d3c = D_1F8002D4;
        o->anim = D_8012E04C;
        *(signed char *)&o->b0f = -9;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        FUN_8001fe6c(o);
        o->b04++;
        if (D_8009CFE5[o->b0c]) o->b04 = 3;
        if (D_8009CDCA == 0xff) o->b04 = 3;
        break;
    case 1:
        if (D_8009C964 == 0x21 && D_8009C93A == 1) break;
        AnimAdvance(o);
        ObjCullRegister(o);
        break;
    case 2:
        ObjCullRegister(o);
        s = o->step;
        switch (s) {
        case 0:
        case 1:
            if (o->state == 0) AnimAdvance(o);
            break;
        case 2:
            func_8012455C(o);
            if (o->visible == 0) o->b04 = 3;
            break;
        }
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
