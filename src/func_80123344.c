// FUNC 80123344 772 X000
// MATCHING 80123344 772
#include "TOBJ.H"
typedef struct { short t; unsigned short x, y, z; } E;
extern int D_8013B0E0;
extern int D_80134DC4;
extern int D_1F8002D4;
extern unsigned char D_8009D0AA[];
extern unsigned char D_8009D0C8[];
extern unsigned short D_80138F0C[];
extern E D_80138F14[];
extern void AnimLoadDuration(TObj *o);
extern int ObjCullRegister(TObj *o);
extern void playSFX(int n);
extern void FUN_800188e0(TObj *o);

void func_80123344(TObj *o)
{
    TObj *p;
    E *e;
    switch (o->b04) {
    case 0:
        o->box0 = 8;
        o->box1 = 0x10;
        o->box2 = 8;
        o->box3 = 0x10;
        o->b0a = 0;
        switch (o->subtype) {
        case 0:
            o->w1e = 0xb;
            o->b0d = 1;
            o->w08 = 0x79cf;
            *(int *)&o->anim = D_8013B0E0;
            goto common;
        case 1:
            o->b0d = 0;
            o->w1e = 8;
            *(int *)&o->anim = D_80134DC4;
        common:
            o->d3c = D_1F8002D4;
            break;
        }
        AnimLoadDuration(o);
        goto next;
    case 1:
        if (ObjCullRegister(o) == 0) break;
        p = (TObj *)o->d94;
        switch (o->subtype) {
        case 0:
            o->h->p.whole = p->h->p.whole + 12;
            o->y.p.whole = p->y.p.whole;
            o->d->p.whole = p->d->p.whole;
            if (D_8009D0AA[0] == 0) break;
            goto next;
        case 1:
            switch (o->step) {
            case 0: {
                unsigned short v = D_80138F0C[(unsigned short)p->wb8];
                o->wb8 = v;
                o->w22 = D_80138F14[v].t;
                o->step++;
            }
            case 1:
                e = &D_80138F14[(unsigned short)o->wb8];
                if (e->t == -2) {
                    o->wb8--;
                    e = &D_80138F14[(unsigned short)o->wb8];
                }
                if (e->t == -1) {
                    unsigned short v = D_80138F0C[(unsigned short)p->wb8];
                    e = &D_80138F14[v];
                    o->wb8 = v;
                }
                if (--o->w22 == 0) {
                    o->wb8++;
                    o->w22 = e->t;
                }
                o->h->p.whole = p->h->p.whole + e->x;
                o->y.p.whole = p->y.p.whole + e->y;
                o->d->p.whole = p->d->p.whole;
                break;
            }
            if (D_8009D0C8[0] == 0) break;
            goto next;
        }
        break;
    case 2:
        playSFX(9);
    next:
        o->b04++;
        break;
    case 3:
        FUN_800188e0(o);
        break;
    }
}
