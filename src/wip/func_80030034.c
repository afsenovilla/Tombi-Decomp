// FUNC 80030034 976 MAIN0
// wip: only the sext temp/constant 2 registers differ (a0<->v1)
#include "TOBJ.H"
typedef struct { int a, y, b; } P3;
extern P3 D_800A6048;
extern unsigned char D_800A6038, D_800A6104;
extern TObj *ObjAlloc(void);
extern void func_80111A50(int a, int b, int c);

static __inline__ void spawn(int sub, int idx)
{
    TObj *o = ObjAlloc();
    if (o != 0) {
        o->active = 1;
        o->type = 0x4e;
        o->subtype = sub;
        o->b0c = idx;
        o->a.raw = D_800A6048.a;
        o->y.raw = D_800A6048.y;
        o->b.raw = D_800A6048.b;
        o->step = 0;
    }
}

void func_80030034(int n)
{
    if ((short)n != 2) D_800A6038 = 7;
    D_800A6104 = 2;
    switch ((short)n) {
    case 0:
        func_80111A50(0, 0, 0);
        spawn(n, 1);
        break;
    case 1:
        func_80111A50(1, 0, 0);
        spawn(n, 5);
        spawn(n, 6);
        spawn(n, 7);
        spawn(n, 8);
        spawn(n, 9);
        spawn(n, 10);
        break;
    case 2:
        spawn(n, 0);
        spawn(n, 1);
        spawn(n, 2);
        break;
    }
}
