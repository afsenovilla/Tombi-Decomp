// FUNC 8012cfcc 496 X010
// MATCHING 8012cfcc 496
#include "TOBJ.H"
typedef struct { short x, y, z, pad; } P4;

extern unsigned char D_8009C93A, D_8009C938;
extern short D_1F80016A_S; /* second name for D_1F80016A: keeps the reload in the loop (debt) */
extern short D_1F80016A[], D_1F80016E[], D_1F800172[];
extern P4 D_8012F4AC[];
extern TObj *FUN_800183b8(void);
extern void PoolFree_1F800210(TObj *);

void func_8012CFCC(TObj *o)
{
    TObj *e;
    int i;
    P4 *p;

    switch (o->b04) {
    case 0:
        if (D_8009C93A) o->b04++;
        break;
    case 1:
        if (D_8009C938 == 1) {
            o->b04++;
            o->w08 = 0x3c;
        }
        break;
    case 2:
        if (--o->w08 == 0) {
            p = D_8012F4AC;
            for (i = 0; i < 8; i++) {
                e = FUN_800183b8();
                if (e) {
                    e->type = 0x3a;
                    e->active = 2;
                    e->b0c = i;
                    e->subtype = 3;
                    e->b0a = 2;
                    e->d8c = 0;
                    e->a.p.whole = D_1F80016A[0] + p->x;
                    e->y.p.whole = D_1F80016E[0] + p->y;
                    e->b.p.whole = D_1F800172[0] + p->z;
                    e->w74 = D_1F80016A_S + p->x / 2;
                    p++;
                }
            }
            o->w08 = 300;
            o->b04++;
        }
        break;
    case 3:
        if (--o->w08 == -1) {
            D_8009C938 = 2;
            PoolFree_1F800210(o);
        }
        break;
    }
}
