// FUNC 80119e08 392 X003
/* score 21: only difference is the game loads o->d90 into v1 for the test and copies it to s2 (move s2,v1) before the RotMatrix call; ours loads straight into s2. Tried: test/assign split with q temp, volatile load, inline parent()/follow() helpers, p assigned after stores/call, reuse of p/q in case 1. */
#include "TOBJ.H"
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { long vx, vy, vz, pad; } VECTOR;
typedef struct { short m[3][3]; long t[3]; } MATRIX;

extern unsigned int FUN_8001f9e0(void);
extern short FUN_8001fe0c(unsigned char, int);
extern void FUN_8006424c(SVECTOR *, MATRIX *);
extern void FUN_80063a6c(MATRIX *, VECTOR *, VECTOR *);
extern void FUN_800202b4(TObj *);

void func_80119E08(TObj *o)
{
    TObj *p;
    VECTOR in;
    VECTOR out;
    SVECTOR v;
    MATRIX m;

    switch (o->step) {
    case 0:
        o->velH = 0;
        if (o->subtype == 0)
            o->step++;
        break;
    case 1:
        if (o->d90 == 0)
            o->velX += FUN_8001f9e0() & 0x1f;
        else
            o->velX = ((TObj *)o->d90)->velX;
        o->velH = FUN_8001fe0c(o->velX, 0x1000);
        o->d8c = (o->velH >> 8) & 0xfff;
        break;
    }
    p = (TObj *)o->d90;
    if (p != 0) {
        v.vx = 0;
        v.vy = 0;
        v.vz = o->d8c;
        FUN_8006424c(&v, &m);
        in.vx = o->d30;
        in.vy = o->d34;
        in.vz = o->d38;
        FUN_80063a6c(&m, &in, &out);
        o->a.raw = p->a.raw + out.vx;
        o->y.raw = p->y.raw + out.vy;
        o->b.raw = p->b.raw + out.vz;
        if (o->d94 == 0)
            o->d8c = FUN_8001fe0c(o->velH, 0x40);
    }
    FUN_800202b4(o);
}
