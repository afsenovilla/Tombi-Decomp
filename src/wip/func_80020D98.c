// FUNC 80020d98 276 MAIN0
// r11: score 6; only diff: 'and v1,v1,v0' operand order in the bit test (ours and v0,v0,v1). D-first form turns into srav/la.
#include "TOBJ.H"
typedef struct { unsigned char b0, b1, b2, b3, b4; } SP;
extern unsigned short D_8009C960;
extern unsigned short D_8009C962;
extern unsigned short D_80078790[];
extern unsigned int D_8009CB94[];
extern TObj *FUN_800183b8(void);

TObj *func_80020D98(SP *p)
{
    int i, n, k;
    TObj *o;
    char pad;
    k = p->b3;
    n = 0;
    for (i = 0; i < D_8009C960; i++) {
        n += D_80078790[i];
    }
    n += D_8009C962;
    if (k >= 32) n++;
    if ((1 << (k % 32)) & D_8009CB94[n]) return 0;
    if (p->b4 != 2) return 0;
    o = FUN_800183b8();
    if (o == 0) return 0;
o->type = p->b0;
o->subtype = p->b1;
o->active = 2;
o->b0c = p->b2 | 0x80;
    o->b6b = p->b3;
    return o;
}
