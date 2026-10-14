// JPDEMO slpm80165 800280FC 368
// Demo version of PAL main0 8002809C (FUN_8002809c.c): no +0x44 clamp flags.
typedef struct {
    char p0[0x18]; int v; char p1[0x30 - 0x1c]; short s30, s32; char p2[0x44 - 0x34]; unsigned short f44;
} O;
typedef union { int i; struct { short lo, hi; } s; } FX;
extern FX D_1f8000f0;
extern int D_1f800190;
#define VF (*(volatile int *)&D_1f8000f0.i)

static __inline__ void inl(O *o)
{
    int d = D_1f800190 - VF;
    if (d >= 0) {
        if (d > 0x10000) {
            if (d < o->v) {
                if (d < 0x40000) o->v = d;
                else o->v = 0x40000;
            } else {
                if (o->v < 0) o->v = 0;
                o->v += 0x8000;
            }
            VF += o->v;
            if (D_1f8000f0.s.hi > o->s32) {
                D_1f8000f0.i = o->s32 << 16;
                o->v = 0;
            }
        } else {
            VF = D_1f800190;
            if (D_1f8000f0.s.hi > o->s32) {
                D_1f8000f0.i = o->s32 << 16;
                o->v = 0;
            }
        }
    } else {
        if (d < -0x40000) {
            if (o->v < d) {
                if (d > -0x40000) o->v = d;
                else o->v = -0x40000;
            } else {
                if (o->v > 0) o->v = 0;
                o->v -= 0x4000;
            }
        } else {
            o->v = d;
        }
        VF += o->v;
        if (D_1f8000f0.s.hi < o->s30) {
            D_1f8000f0.i = o->s30 << 16;
            o->v = 0;
        }
    }
}

void FUN_8002809c(O *o)
{
    inl(o);
}
