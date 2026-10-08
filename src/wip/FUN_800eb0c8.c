// FUNC 800eb0c8 468 X000
#include "TOBJ.H"
extern int DAT_1f8002d0;
extern unsigned short DAT_8009c960;
extern unsigned char DAT_80114ab8[], DAT_80114ac0[];
extern int *PTR_PTR_80114ac8[];
extern char DAT_80077cdc[];
extern void SfxPlay(int);
extern void FUN_8001fe6c(TObj *);
extern void FUN_800187e4(TObj *);
extern int FUN_8001fec0(TObj *);
extern void FUN_8001fa88(TObj *, int);
extern int FUN_800202b4(TObj *);

void FUN_800eb0c8(TObj *o)
{
    int v, a;
    short t;
    unsigned char b;
    b = o->b04;
    if (b != 1) {
        if (b < 2) {
            if (b != 0)
                return;
            o->b04 = 1;
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
            SfxPlay(a);
            o->anim = (void *)PTR_PTR_80114ac8[DAT_8009c960][o->b0c];
            FUN_8001fe6c(o);
        return;
        }
        if (b == 2) {
            o->b04 = 3;
            return;
        }
        if (b != 3)
            return;
        FUN_800187e4(o);
        return;
    }
    if (o->subtype == 0) {
        if (FUN_8001fec0(o) != 0)
            o->b04 = 3;
        FUN_8001fa88(o, 1 - o->animFrame);
        t = o->timer - 1;
        o->timer = t;
        if (t == -1)
            o->b04 = 3;
    } else {
        if (FUN_8001fec0(o) != 0)
            o->b04 = 3;
    }
    if (FUN_800202b4(o) == 0)
        o->b04 = 3;
    return;
}
