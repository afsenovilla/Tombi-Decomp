// FUNC 8002eddc 116 MAIN0
// MATCHING 8002eddc 116
// VERSION SLPM_80165 slpm80165 8002E9E0 116
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 8002EDDC (FUN_8002eddc.c): clears +0xa/+0xb/+0xd (PAL sets +0xd to 0x80).
#include "TOBJ.H"
extern int DAT_1f8002d8;
extern void *PTR_80012208[];
extern void func_8001fe6c(TObj *o);

void FUN_8002eddc(TObj *o)
{
    volatile TObj *v = o;
    int i;
    v->active = 1;
    v->type = 0x32;
    v->animFrame = 1;
    *(volatile signed char *)&v->b0f = -30;
    v->w1e = 0x14;
    i = o->wac;
    v->b0a = 0;
    v->b0b = 0;
    v->b0d = 0;
    o->d3c = DAT_1f8002d8;
    o->anim = PTR_80012208[i];
    func_8001fe6c(o);
}
#else
#include "TOBJ.H"
extern int DAT_1f8002d8;
extern void *PTR_80012208[];
extern void func_8001fe6c(TObj *o);

void initPoolType32Object(TObj *o)
{
    volatile TObj *v = o;
    int i;
    v->active = 1;
    v->type = 0x32;
    v->animFrame = 1;
    *(volatile signed char *)&v->b0f = -30;
    v->w1e = 0x14;
    v->b0d = 0x80;
    i = o->wac;
    v->b0b = 0;
    o->d3c = DAT_1f8002d8;
    o->anim = PTR_80012208[i];
    func_8001fe6c(o);
}
#endif
