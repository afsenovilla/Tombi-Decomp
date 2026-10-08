// FUNC 80030034 976 MAIN0
// wip: only the sext temp/constant 2 registers differ (a0<->v1)
// r2: m = n; n = (short)n; spawn(m,..) and short-param variants still 16 (short param n makes gcc const-propagate n into the cases and drops the s0 copy).
// b25: game's sll reads the incoming a0 (not the s0 copy), so the sext pseudo ties to a0; ours reads pseudo 72 (s0). Tried short m = n / int m / K&R short n (386) / inline(int n, short m) wrappers with various n types: all 16 or worse. b46: -dg: sext pseudo (6 refs/13 insns) outranks const 2 (3/22) in global-alloc; game has const 2 first (v1) and the sext in a0. int m = n copies get merged into one pseudo by cse; char*/unsigned/long copies: 16.
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
