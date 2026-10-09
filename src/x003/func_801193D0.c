// FUNC 801193d0 808 X003
// MATCHING 801193d0 808
#include "TOBJ.H"

typedef struct { short vx, vy, vz, pad; } SV;
typedef struct { char p[0xb4]; SV v[4]; } OX;

extern short D_8013589C[];
extern short D_1F800176[], D_1F800186;
extern unsigned short D_8009C962[];
extern unsigned int FUN_8001f9e0(void);

#define V(o) (((OX *)(o))->v)

#define INITV(o) { V(o)[0].vx = D_8013589C[0]; V(o)[0].vy = D_8013589C[1]; V(o)[0].vz = 0; V(o)[1].vx = D_8013589C[2]; V(o)[1].vy = D_8013589C[3]; V(o)[1].vz = 0; V(o)[2].vx = D_8013589C[4]; V(o)[2].vy = D_8013589C[5]; V(o)[2].vz = 0; V(o)[3].vx = D_8013589C[6]; V(o)[3].vy = D_8013589C[7]; V(o)[3].vz = 0; }

void func_801193D0(TObj *o)
{
    int k;

    switch (o->step) {
    case 0:
        o->step = 2;
        o->velX = 0x100;
        o->timer = 0x168;
        INITV(o);
        o->a.raw = (D_1F800176[0] + o->subtype * 52) << 16;
        k = 0x60;
        if (D_8009C962[0] == 3)
            o->y.raw = (-0x272 - (FUN_8001f9e0() & 0xf) * 42) << 16;
        else
            o->y.raw = (D_1F800186 - ((FUN_8001f9e0() & 7) * 32 + k)) << 16;
        o->velH = o->subtype * 24;
        break;
    case 1:
        o->step++;
        o->velX = 0x100;
        o->timer = 0x168;
        INITV(o);
        o->a.raw = (D_1F800176[0] - 0xa0) << 16;
        k = 0x18;
        if (D_8009C962[0] == 3)
            o->y.raw = (-0x272 - (FUN_8001f9e0() & 0xf) * 42) << 16;
        else
            o->y.raw = (D_1F800186 - ((FUN_8001f9e0() & 7) * 32 + k)) << 16;
        o->velH = o->subtype * 24;
        break;
    case 2:
        o->h->raw += 0x10000 + (o->velH << 8);
        o->y.raw -= 0x1000;
        if (o->timer != 0) {
            V(o)[0].vx += o->velX >> 8;
            V(o)[1].vx += o->velX >> 8;
            o->timer--;
        }
        if (o->h->p.whole + V(o)[2].vx > D_1F800176[0] + 0x26c)
            o->step = 1;
        break;
    }
}
