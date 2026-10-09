// FUNC 8012b388 176 X004
// MATCHING 8012b388 176
#include "TOBJ.H"
extern int D_800A6048[], D_800A604C[], D_800A6050[];
extern unsigned short D_800A6066[];
extern void *D_80134CE0;
extern int D_1F8002D4[];
extern TObj *FUN_800183b8(void);
extern void AnimLoadDuration(TObj *);

void func_8012B388(void)
{
    TObj *e = FUN_800183b8();

    if (e) {
        e->active = 1;
        e->type = 0x33;
        e->a.raw = D_800A6048[0];
        e->y.raw = D_800A604C[0];
        e->b.raw = D_800A6050[0];
        e->animFrame = D_800A6066[0] & 1;
        e->w1e = 6;
        e->subtype = 0;
        e->b0c = 0;
        e->b0a = 2;
        e->d8c = 0;
        e->d3c = D_1F8002D4[0];
        e->anim = D_80134CE0;
        AnimLoadDuration(e);
    }
}
