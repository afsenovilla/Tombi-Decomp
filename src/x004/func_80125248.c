// FUNC 80125248 472 X004
// MATCHING 80125248 472
#include "TOBJ.H"
typedef struct { int x, y, z; } V3;

extern int DAT_1f8002d4[];
extern void *D_8013B104[];
extern int FUN_800202b4(TObj *);
extern void FUN_8001fe6c(TObj *);
extern TObj *FUN_800182ac(void);
extern void FUN_80018790(TObj *);

void func_80125248(TObj *o)
{
    TObj *n;

    switch (o->b04) {
    case 0:
        o->box0 = 0x14;
        o->box1 = 0x28;
        o->box2 = 0x18;
        o->box3 = 0x30;
        o->d3c = DAT_1f8002d4[0];
        o->w1e = 10;
        o->b0d = 0;
        o->b0a = 0;
        o->b0f = 0;
        o->b04++;
        break;
    case 1:
        FUN_800202b4(o);
        break;
    case 2:
        FUN_800202b4(o);
        if (o->visible == 0)
            break;
        switch (o->step) {
        case 0:
            o->anim = D_8013B104[0];
            FUN_8001fe6c(o);
            o->step++;
            break;
        case 1:
            break;
        case 2:
            n = FUN_800182ac();
            if (n != 0) {
                n->active = 1;
                n->type = 2;
                n->animFrame = (o->animFrame & 1) | 2;
                *(V3 *)&n->a = *(V3 *)&o->a;
                n->subtype = o->subtype;
                n->b0c = o->b0c;
                n->b6b = o->b6b;
                n->b1d = o->b1d;
            }
            o->b04 = 3;
            break;
        }
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
