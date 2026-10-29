// FUNC 8002809c 436 MAIN0
// MATCHING 8002809c 436
// VERSION SLPM_80165 slpm80165 800280FC 368
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 8002809C (FUN_8002809c.c): no +0x44 clamp flags.
typedef struct {
    char p0[0x18]; int v; char p1[0x30 - 0x1c]; short s30, s32; char p2[0x44 - 0x34]; unsigned short f44;
} O;
typedef union { int i; struct { short lo, hi; } s; } FX;
extern FX VIEW_CAMERA_Y_FIX;
extern int CAMERA_TARGET_Y_FIX;
#define VF (*(volatile int *)&VIEW_CAMERA_Y_FIX.i)

static __inline__ void inl(O *o)
{
    int d = CAMERA_TARGET_Y_FIX - VF;
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
            if (VIEW_CAMERA_Y_FIX.s.hi > o->s32) {
                VIEW_CAMERA_Y_FIX.i = o->s32 << 16;
                o->v = 0;
            }
        } else {
            VF = CAMERA_TARGET_Y_FIX;
            if (VIEW_CAMERA_Y_FIX.s.hi > o->s32) {
                VIEW_CAMERA_Y_FIX.i = o->s32 << 16;
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
        if (VIEW_CAMERA_Y_FIX.s.hi < o->s30) {
            VIEW_CAMERA_Y_FIX.i = o->s30 << 16;
            o->v = 0;
        }
    }
}

void FUN_8002809c(O *o)
{
    inl(o);
}
#else
typedef struct {
    char p0[0x18]; int v; char p1[0x30 - 0x1c]; short s30, s32; char p2[0x44 - 0x34]; unsigned short f44;
} O;
typedef union { int i; struct { short lo, hi; } s; } FX;
extern FX VIEW_CAMERA_Y_FIX;
extern int CAMERA_TARGET_Y_FIX;
#define VF (*(volatile int *)&VIEW_CAMERA_Y_FIX.i)

static __inline__ void inl(O *o)
{
    int d = CAMERA_TARGET_Y_FIX - VF;
    if (d >= 0) {
        if (d > 0x10000) {
            if (d < o->v) {
                if (d > 0x3ffff) o->v = 0x40000;
                else o->v = d;
            } else {
                if (o->v < 0) o->v = 0;
                o->v += 0x8000;
            }
            VF += o->v;
            if (VIEW_CAMERA_Y_FIX.s.hi > o->s32) {
                VIEW_CAMERA_Y_FIX.i = o->s32 << 16;
                o->v = 0;
                o->f44 |= 8;
            }
        } else {
            VF = CAMERA_TARGET_Y_FIX;
            if (VIEW_CAMERA_Y_FIX.s.hi > o->s32) {
                VIEW_CAMERA_Y_FIX.i = o->s32 << 16;
                o->v = 0;
                o->f44 |= 8;
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
        if (VIEW_CAMERA_Y_FIX.s.hi < o->s30) {
            VIEW_CAMERA_Y_FIX.i = o->s30 << 16;
            o->v = 0;
            o->f44 |= 4;
        }
    }
}

void cameraFollowTargetYB(O *o)
{
    inl(o);
}
#endif
