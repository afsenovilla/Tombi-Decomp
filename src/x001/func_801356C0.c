// FUNC 801356c0 400 X001
// MATCHING 801356c0 400
#include "TOBJ.H"

extern unsigned short D_8009C960[], D_8009C962;
extern int D_8009C960A[]; /* second name for D_8009C960 (int read; debt) */
extern unsigned short D_8009D2C0;
extern int D_1F8002D0, D_1F8002E4;
extern void *D_8013DE38, *D_8013DE34, *D_8013DE08;
extern void *D_8013DDDC, *D_8013DDD8, *D_8013DD50;
extern void FUN_8001fe6c(TObj *);

void func_801356C0(TObj *o)
{
    TObj *p;

    o->box0 = 8;
    o->box1 = 0x10;
    o->box2 = 8;
    o->box3 = 0x10;
    if (D_8009C960A[0] == 0x20001) o->w1e = 2;
    else o->w1e = 1;
    if (D_8009C960[0] == 1 && D_8009C962 < 2) {
        o->d3c = D_1F8002D0;
        switch (o->subtype) {
        case 2:
            o->anim = D_8013DE38;
            break;
        case 3:
            o->anim = D_8013DE34;
            break;
        default:
            o->anim = D_8013DE08;
            break;
        }
    } else {
        o->d3c = D_1F8002E4;
        switch (o->subtype) {
        case 2:
            o->anim = D_8013DDDC;
            break;
        case 3:
            o->anim = D_8013DDD8;
            break;
        default:
            o->anim = D_8013DD50;
            break;
        }
    }
    o->b0d = 0;
    o->category |= 0x80;
    FUN_8001fe6c(o);
    o->b04++;
    if ((D_8009D2C0 >> (o->b0c - 1)) & 1) {
        p = (TObj *)o->d94;
        o->b04 = 3;
        p->b04 = 3;
    }
}
