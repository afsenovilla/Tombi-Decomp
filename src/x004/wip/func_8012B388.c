/* score 20: the three player-position copies (lw/nop/sw each, never hoisted) and the constant stores interleave differently; game keeps natural store order active,type,a,y,b,w1e,subtype,b0c,b0a,d8c,animFrame,anim,d3c with only the 6066/anim/2d4 loads hoisted. Tried struct/[0]/scalar combos for every global, V3 block copy, -fno-schedule-insns (33, gets e in a0), statement hill-climb. */
// FUNC 8012b388 176 X004
#include "TOBJ.H"
extern int D_800A6048[];
extern int D_800A604C[];
extern int D_800A6050[];
extern unsigned short D_800A6066;
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
        e->d3c = D_1F8002D4[0];
        e->y.raw = D_800A604C[0];
        e->b.raw = D_800A6050[0];
        e->a.raw = D_800A6048[0];
        e->w1e = 6;
        e->subtype = 0;
        e->b0c = 0;
        e->b0a = 2;
        e->d8c = 0;
        e->animFrame = D_800A6066 & 1;
        e->anim = D_80134CE0;
        AnimLoadDuration(e);
    }
}
