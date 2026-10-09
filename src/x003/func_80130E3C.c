// FUNC 80130e3c 1024 X003
// MATCHING 80130e3c 1024
#include "TOBJ.H"

typedef struct { short x, y, z; unsigned char st, c; } R;
extern R D_80135F00[];
extern R D_80135ED0[];
extern unsigned short D_8009C962;
extern TObj *FUN_800184d8(void);
extern void PoolFree_1F800210(TObj *);

static __inline__ TObj *spawn(R *r, TObj *link)
{
    TObj *e = FUN_800184d8();

    if (e) {
        e->active = 1;
        e->type = 0x2e;
        e->b0a = 7;
        e->a.p.whole = r->x;
        e->y.p.whole = r->y;
        e->b.p.whole = r->z;
        e->subtype = r->st;
        e->b0c = r->c;
        e->d90 = (int)link;
        return e;
    }
    return 0;
}

void func_80130E3C(TObj *o)
{
    TObj *p;
    R *r;

    switch (D_8009C962) {
    case 0:
    case 4:
        r = D_80135F00;
        spawn(r, 0);
        spawn(r + 1, 0);
        break;
    case 1:
    case 5:
        r = D_80135ED0;
        p = spawn(r, 0);
        spawn(r + 1, p);
        p = spawn(r + 2, 0);
        spawn(r + 3, p);
        spawn(r + 4, p);
        spawn(r + 5, 0);
        break;
    }
    PoolFree_1F800210(o);
}
