// FUNC 800eb0c8 468 X001
// MATCHING 800eb0c8 468
#include "TOBJ.H"
extern int DAT_1f8002d0;
extern unsigned short DAT_8009c960;
extern unsigned char DAT_80114ab8[], DAT_80114ac0[];
extern void **PTR_PTR_80114ac8[];
extern char DAT_80077cdc[];
extern void FUN_8001e4f0(int);
extern void FUN_8001fe6c(TObj *);
extern void FUN_800187e4(TObj *);
extern int FUN_8001fec0(TObj *);
extern void FUN_8001fa88(TObj *, unsigned short);
extern int FUN_800202b4(TObj *);

void FUN_800eb0c8(TObj *o)
{
    int a, v;
    switch (o->b04) {
    case 0:
        o->b04 = o->b04 + 1;
        v = DAT_1f8002d0;
        o->w1e = 1;
        *(signed char *)&o->b0f = -30;
        o->b0a = 0;
        o->movetab = DAT_80077cdc;
        o->d3c = v;
        if (o->subtype == 0) {
            a = 0x10;
            o->b0c = DAT_80114ab8[DAT_8009c960];
            o->timer = 0x5a;
        } else {
            a = 0x27;
            o->b0c = DAT_80114ac0[DAT_8009c960];
        }
        FUN_8001e4f0(a);
        o->anim = PTR_PTR_80114ac8[DAT_8009c960][o->b0c];
        FUN_8001fe6c(o);
        break;
    case 1:
        if (o->subtype == 0) {
            if (FUN_8001fec0(o) != 0)
                o->b04 = 3;
            FUN_8001fa88(o, 1 - o->animFrame);
            o->timer = o->timer - 1;
            if (o->timer == -1)
                o->b04 = 3;
        } else {
            if (FUN_8001fec0(o) != 0)
                o->b04 = 3;
        }
        if (FUN_800202b4(o) == 0)
            o->b04 = 3;
        break;
    case 2:
        o->b04 = o->b04 + 1;
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
