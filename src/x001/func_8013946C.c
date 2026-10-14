// FUNC 8013946c 772 X001
// MATCHING 8013946c 772
#include "spad.h"
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"

extern void *D_8013FA90[];
extern void *D_8013FAB8[], *D_8013FABC[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void setEventComplete(int, int);
extern void func_80138E7C(TObj *);
extern void func_80139174(TObj *);

void func_8013946C(TObj *o)
{
    switch (o->step) {
    case 0:
        if (GCD.bd081 == 1) {
            o->timer = 0;
            o->step++;
        }
        break;
    case 1:
        if (((SPAD.s1f8 + SPAD.i198) & 3) == 0) {
            short w = o->wac + 4;
            o->wac = w;
            o->anim = D_8013FA90[w];
            o->step++;
        }
        break;
    case 2:
        if (((SPAD.s1f8 + SPAD.i198) & 3) == 0) {
            if (++o->timer >= 0x10) {
                o->step++;
                switch (o->wac) {
                case 4:
                    o->wac = 10;
                    o->anim = D_8013FAB8[0];
                    AnimLoadDuration(o);
                    break;
                case 5:
                    o->wac = 11;
                    o->anim = D_8013FABC[0];
                    AnimLoadDuration(o);
                    break;
                case 6:
                    break;
                }
            } else {
                short w = o->wac - 4;
                o->wac = w;
                o->anim = D_8013FA90[w];
                o->step--;
            }
        }
        break;
    case 3:
        setEventComplete(0x1b, 0);
        o->timer = 0xb4;
        o->step++;
        break;
    case 4:
        AnimAdvance(o);
        if (--o->timer == -1) {
            o->step++;
            GCD.bd081 = 2;
        }
        break;
    case 5:
        AnimAdvance(o);
        if (GCD.bd081 >= 5) o->step++;
        break;
    case 6:
        switch (o->b0c) {
        case 0:
            o->state = 0;
            o->step++;
            break;
        case 1:
            func_80138E7C(o);
            break;
        case 2:
            func_80139174(o);
            break;
        }
        break;
    case 7:
        if (o->b0c == 2) {
            GCD.bd081 = 6;
            GVARS.b40 = 0;
            GVARS.b3f = 0;
            GVARS.b42 = 0;
            GVARS.b3e = 0;
        }
        o->step++;
        break;
    case 8:
        if (o->b0c) o->b04 = 3;
        else o->b6b = 1;
        break;
    }
}
