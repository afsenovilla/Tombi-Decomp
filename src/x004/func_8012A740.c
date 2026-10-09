// FUNC 8012a740 376 X004
// MATCHING 8012a740 376
#include "TOBJ.H"
typedef struct { unsigned char b0, b1, b2, b3; unsigned short w; short idx; void **ptrs; } E;
extern E D_801311C8[];
extern int D_1F8002C8[];
extern unsigned char D_800A6047;
extern unsigned char D_8009CF2D;
extern void FUN_8001fe6c(TObj *);

void func_8012A740(TObj *o)
{
    if (D_8009CF2D == 0) {
        o->box0 = D_801311C8[o->subtype].b0;
        o->box1 = D_801311C8[o->subtype].b1;
        o->box2 = D_801311C8[o->subtype].b2;
        o->box3 = D_801311C8[o->subtype].b3;
        o->w1e = D_801311C8[o->subtype].w;
        o->d3c = D_1F8002C8[D_801311C8[o->subtype].idx];
        o->anim = D_801311C8[o->subtype].ptrs[o->b6b];
        FUN_8001fe6c(o);
        o->b0a = 2;
        o->d8c = 0;
        o->b0d = 0;
        o->b68 = 0;
        o->category |= 0x80;
        o->b0f = D_800A6047 + 10;
        o->b04++;
    }
}
