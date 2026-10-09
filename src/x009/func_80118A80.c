// FUNC 80118a80 376 X009
// MATCHING 80118a80 376
#include "TOBJ.H"
typedef struct { char p[0xe]; unsigned char b0e; char q[0x5c - 0xf]; int d5c; } X;
#define B0E(o) (((X *)(o))->b0e)
#define D5C(o) (((X *)(o))->d5c)
extern int D_1F8002E8[];
extern void *D_8012E9C0[];
extern void *D_8012E9BC[];
extern void AnimLoadDuration(TObj *);
extern void func_80118970(TObj *);
extern TObj *ObjAlloc(void);

void func_80118A80(TObj *o)
{
    TObj *e;
    int *g = D_1F8002E8;

    o->w1e = 1;
    o->b0d = 0;
    B0E(o) = 0;
    o->d3c = g[0];
    o->subtype = 1;
    o->b0c = 1;
    o->anim = D_8012E9C0[0];
    AnimLoadDuration(o);
    func_80118970(o);
    o->timer = 0;
    e = ObjAlloc();
    if (e == 0) return;
    e->active = 1;
    e->b1d = o->b1d;
    e->category = o->category;
    e->type = o->type;
    e->subtype = o->subtype;
    e->d30 = 0;
    e->d34 = 0;
    e->d38 = 0;
    e->d90 = (int)o;
    e->h->raw = o->h->raw;
    e->y.raw = o->y.raw;
    e->d->raw = o->d->raw;
    e->b0a = o->b0a;
    e->animFrame = 1;
    e->b0f = 0;
    e->d84 = 0;
    e->d88 = 0;
    e->d8c = 0;
    D5C(e) = e->a.p.whole;
    e->d60 = e->y.p.whole;
    e->d64 = e->b.p.whole;
    e->w1e = 1;
    e->b0d = 0;
    B0E(e) = 0;
    e->b04++;
    e->d3c = g[0];
    e->subtype = 0;
    e->b0c = 0;
    e->anim = D_8012E9BC[0];
    func_80118970(e);
}
