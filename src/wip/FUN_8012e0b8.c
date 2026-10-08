// FUNC 8012e0b8 264 X000
#include "TOBJ.H"
extern TObj *FUN_800183b8(void);
extern unsigned short FUN_8001f9e0(void);
extern void FUN_8001fe6c(TObj *o);
extern unsigned char DAT_800a6047;
extern int D_1f8002d4;
extern void *PTR_8013b26c;

void FUN_8012e0b8(int x, int y, int z)
{
    TObj *o = FUN_800183b8();
    if (o != 0) {
        o->box1 = 12;
        o->box3 = 12;
        o->active = o->b0a = 2;
        o->box0 = 6;
        o->box2 = 6;
        o->type = 3;
        o->animFrame = FUN_8001f9e0() & 1;
        o->a.raw = x << 16;
        o->y.raw = y << 16;
        o->b.raw = z << 16;
        o->w1e = 11;
        o->b0c = 1;
        o->b0d = 0;
        o->b69 = 0;
        o->w7a = 1;
        o->b0f = DAT_800a6047 - 1;
        o->d3c = D_1f8002d4;
        o->anim = PTR_8013b26c;
        FUN_8001fe6c(o);
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
    }
}
