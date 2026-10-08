// FUNC 8002e83c 624 MAIN0
#include "TOBJ.H"
extern short D_8007A5F0[];
extern short D_8007A3F0[];
extern void *D_80012204;
extern TObj *ObjAlloc();
extern int Rand();
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjCullRegister(TObj *);
extern void ObjFree(TObj *);

#define SETUP(p) \
    p->type = 0x31; \
    p->animFrame = 1; \
    p->b04 = 1; \
    p->active = 1; \
    p->b0a = 0; \
    p->b0b = 0; \
    p->b0f = 0; \
    p->w1e = 0x14; \
    p->b0d = 1; \
    p->w08 = ((p->b0c + 0x1fc) << 6) | 0x17; \
    p->d3c = *(int *)0x1F8002D8; \
    p->anim = D_80012204; \
    AnimLoadDuration(p);

void func_8002E83C(TObj *o)
{
    register TObj *n;
    int r, amp, idx;

    switch (o->b04) {
    case 0:
        switch (o->subtype) {
        case 0:
            if ((*(unsigned short *)0x1F8001F8 + *(int *)0x1F800198) & 1) {
                if (--o->timer == -1) {
                    o->b04 = 3;
                    break;
                }
                n = ObjAlloc();
                if (n != 0) {
                    r = Rand();
                    amp = ((r & 1) << 11) + 0x800;
                    n->h->raw = o->h->raw;
                    n->y.raw = o->y.raw;
                    n->d->raw = o->d->raw;
                    idx = r & 0xf0;
                    n->h->raw += (D_8007A5F0[idx] * amp) >> 4;
                    n->y.raw += (D_8007A3F0[idx] * amp) >> 4;
                    SETUP(n)
                }
            }
            break;
        case 1:
            SETUP(o)
            break;
        }
        break;
    case 1:
        ObjCullRegister(o);
        if (AnimAdvance(o)) {
            o->b04 = 3;
        }
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
