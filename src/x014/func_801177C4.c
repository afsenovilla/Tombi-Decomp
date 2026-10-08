// FUNC 801177c4 264 X014
// MATCHING 801177c4 264
#include "TOBJ.H"
extern int D_1F8002D4[];
extern void *D_8012A028[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_80018ca4(TObj *);
extern void FUN_800187e4(TObj *);

void func_801177C4(TObj *o)
{
    unsigned char b = o->b04;

    switch (b) {
    case 0:
        o->b04 = b + 1;
        o->w1e = 0xc;
        o->velX = 0x80;
        if (o->subtype == 0) {
            o->ba5 = 0;
            o->ba6 = 0x40;
        } else {
            *(signed char *)&o->ba5 = -0x50;
            o->ba6 = 0;
        }
        o->d8c = 0;
        o->b0d = 0;
        o->d3c = D_1F8002D4[0];
        o->anim = D_8012A028[o->subtype];
        AnimLoadDuration(o);
        break;
    case 1:
        AnimAdvance(o);
        o->visible = 1;
        FUN_80018ca4(o);
        break;
    case 2:
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
