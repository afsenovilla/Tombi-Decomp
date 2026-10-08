// FUNC 801340c4 204 X000
#include "TOBJ.H"
extern TObj *FUN_800183b8(void);
extern void FUN_8001fe6c(TObj *);
extern unsigned short DAT_800a6066;
extern int DAT_1f8002d4;
extern void * volatile PTR_DAT_8013b184;

void FUN_801340c4(int x, int y, int z)
{
    TObj *o = FUN_800183b8();
    if (o) {
        unsigned short u;
        o->active = 1;
        o->type = 0x12;
        u = DAT_800a6066;
        o->a.raw = x << 16;
        o->y.raw = y << 16;
        o->b.raw = z << 16;
        o->w1e = 0xb;
        o->b0d = 0;
        o->animFrame = u & 1;
        o->d3c = DAT_1f8002d4;
        o->b0a = 2;
        o->anim = PTR_DAT_8013b184;
        FUN_8001fe6c(o);
        o->b04 = 2;
        o->step = 3;
        o->state = 0;
    }
}
