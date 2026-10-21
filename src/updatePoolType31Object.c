// FUNC 8002e83c 624 MAIN0
// MATCHING 8002e83c 624
#include "TOBJ.H"
extern short D_8007A5F0[];
extern short D_8007A3F0[];
extern void *D_80012204;
extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern TObj *ObjAlloc(void);
extern int Rand(void);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjCullRegister(TObj *);
extern void ObjFree(TObj *);

static __inline__ void setup(TObj *p)
{
    p->b04 = 1;
    p->type = 0x31;
    p->animFrame = 1;
    p->active = 1;
    p->b0a = 0;
    p->b0b = 0;
    p->b0f = 0;
    p->w1e = 0x14;
    p->b0d = 1;
    p->w08 = ((p->b0c + 0x1fc) << 6) | 0x17;
    p->d3c = ((struct { char pad[0x2d8]; int v; } *)0x1F800000)->v;
    p->anim = D_80012204;
    AnimLoadDuration(p);
}

void updatePoolType31Object(TObj *o)
{
    TObj *p;
    int r;
    int s;

    switch (o->b04) {
    case 0:
        switch (o->subtype) {
        case 0:
            if ((D_1F8001F8 + D_1F800198) & 1) {
                if (--o->timer == -1) {
                    o->b04 = 3;
                    break;
                }
                p = ObjAlloc();
                if (p != 0) {
                    r = Rand();
                    s = ((r & 1) << 11) + 0x800;
                    p->h->raw = o->h->raw;
                    p->y.raw = o->y.raw;
                    p->d->raw = o->d->raw;
                    p->h->raw += (D_8007A5F0[r & 0xf0] * s) >> 4;
                    p->y.raw += (D_8007A3F0[r & 0xf0] * s) >> 4;
                    setup(p);
                }
            }
            break;
        case 1:
            setup(o);
            break;
        }
        break;
    case 1:
        ObjCullRegister(o);
        if (AnimAdvance(o) != 0) o->b04 = 3;
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
