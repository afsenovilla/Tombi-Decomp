// FUNC 80118bf8 520 X009
// MATCHING 80118bf8 520
#include "TOBJ.H"

typedef struct { short x, y, z, pad; } SV;
#define V(o) ((SV *)((char *)(o) + 0xb4))
extern int D_1F8002E8;
extern void *D_8012E9EC[];
extern void *D_8012E9E4;
extern short GetClut(int, int);
extern TObj *ObjAlloc(void);

void func_80118BF8(TObj *o)
{
    TObj *e;
    int *s;
    int d;
    void *a;

    V(o)[0].x = -0x20;
    V(o)[0].y = -0x24;
    V(o)[0].z = 0;
    V(o)[1].x = 0x10;
    V(o)[1].y = -0x24;
    V(o)[1].z = 0;
    V(o)[2].x = -0x20;
    V(o)[2].y = 0xc;
    V(o)[2].z = 0;
    V(o)[3].x = 0x10;
    V(o)[3].y = 0xc;
    V(o)[3].z = 0;
    o->w1e = 0xb;
    o->b0d = 1;
    o->w08 = GetClut(0x120, 0x1e1);
    s = &D_1F8002E8;
    o->_pad0e[0] = 0;
    o->d3c = *s;
    o->subtype = 1;
    o->anim = D_8012E9EC[0];
    o->timer = 0;
    e = ObjAlloc();
    if (e) {
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
        *(int *)&e->w5c = e->a.p.whole;
        e->animFrame = 0x81;
        e->b0f = 0;
        e->d84 = 0;
        e->d88 = 0;
        e->d8c = 0;
        e->d60 = e->y.p.whole;
        e->d64 = e->b.p.whole;
        e->b04++;
        e->w1e = 0xb;
        e->b0d = 1;
        e->w08 = GetClut(0x120, 0x1ee);
        e->_pad0e[0] = 0;
        d = *s;
        e->subtype = 0;
        a = D_8012E9E4;
        V(e)[0].x = -0x2b;
        V(e)[0].y = -0x4c;
        V(e)[0].z = 0;
        V(e)[1].x = 0x1e;
        V(e)[1].y = -0x4c;
        V(e)[1].z = 0;
        V(e)[2].x = -0x2b;
        V(e)[2].y = 0xc;
        V(e)[2].z = 0;
        V(e)[3].x = 0x1e;
        V(e)[3].y = 0xc;
        V(e)[3].z = 0;
        e->d3c = d;
        e->anim = a;
    }
}
