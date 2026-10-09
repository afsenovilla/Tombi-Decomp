// FUNC 80139914 364 X001
/* score 32: logic and cases 1-3 match. Case 0 scheduling: the game stores box0/box2 (li 8 stolen into the dispatch
   delay slot) before b04++ yet still reuses the switch value (andi v1,a0,0xff) for b04++; here b04++ must be first to
   reuse it, and then it is scheduled first. Tried: hill-climb/random orders, setbox() inline, wac/b0c forms. */
#include "TOBJ.H"

extern void *D_8013FA90[];
extern unsigned char D_8009CDBF;
extern int D_1F8002F0;
extern void AnimLoadDuration(TObj *);
extern int FUN_800202b4(TObj *);
extern void FUN_80018790(TObj *);
extern void func_8013946C(TObj *);
extern void func_80139770(TObj *);

void func_80139914(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->d3c = D_1F8002F0;
        o->box0 = 8;
        o->box2 = 8;
        o->active = 2;
        *(signed char *)&o->b0f = -6;
        o->box1 = 0x10;
        o->box3 = 0x10;
        o->w1e = 5;
        o->w98 = o->animFrame;
        o->b0d = 0;
        o->anim = D_8013FA90[o->wac = o->b0c];
        o->b6b = 0;
        if (D_8009CDBF != 0xff) break;
        if (o->b0c) {
            o->b04 = 3;
            break;
        }
        o->wac = 10;
        o->anim = D_8013FA90[10];
        AnimLoadDuration(o);
        o->b6b = 1;
        break;
    case 1:
        FUN_800202b4(o);
        if (o->b6b == 0)
            func_8013946C(o);
        else
            func_80139770(o);
        break;
    case 2:
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
