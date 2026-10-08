// FUNC 800ebc64 220 X018
// MATCHING 800ebc64 220
#include "TOBJ.H"
extern unsigned short DAT_800a6066;
extern int DAT_1f8002c8[];
extern char DAT_80114c20[];
extern char DAT_80114c24[];
extern void FUN_8001fe6c(TObj *);

void FUN_800ebc64(TObj *o)
{
    unsigned short u;
    o->w1e = 0x13;
    o->d3c = DAT_1f8002c8[*(short *)(DAT_80114c20 + o->subtype * 12)];
    o->anim = *(void **)(*(int *)(DAT_80114c24 + o->subtype * 12) + o->b0c * 4);
    FUN_8001fe6c(o);
    u = DAT_800a6066;
    o->b0a = 2;
    o->b0d = 0x80;
    *(signed char *)&o->b0f = -7;
    o->d8c = 0;
    o->b6b = 0;
    o->animFrame = u & 1;
    o->category |= 0x80;
    o->b04++;
}
