// FUNC 801174c4 216 X009
// MATCHING 801174c4 216
#include "TOBJ.H"
typedef struct { short vx, vy, vz, pad; } SV;
typedef struct { int vx, vy, vz; } LV;
extern void FUN_80021f5c(void *);
extern void FUN_80063ddc(void *);
extern void FUN_80063bcc(SV *, LV *);

#define V(o) ((LV *)&(o)->w5c)

void func_801174C4(TObj *o)
{
    char m[32];
    SV r;
    TObj *p;

    o->a.p.whole = V(o)->vx;
    o->y.p.whole = V(o)->vy;
    o->b.p.whole = V(o)->vz;
    p = (TObj *)o->d90;
    FUN_80021f5c(&o->w48);
    r.vx = o->d30 >> 16;
    r.vy = o->d34 >> 16;
    r.vz = o->d38 >> 16;
    FUN_80063ddc(&p->w48);
    FUN_80063bcc(&r, V(o));
    V(o)->vx += p->a.p.whole;
    V(o)->vy += p->y.p.whole;
    V(o)->vz += p->b.p.whole;
    o->velX = V(o)->vx - o->a.p.whole;
    o->velY = V(o)->vy - o->y.p.whole;
}
