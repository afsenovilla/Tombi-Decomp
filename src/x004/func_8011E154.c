// FUNC 8011e154 920 X004
// MATCHING 8011e154 920
#include "TOBJ.H"

typedef struct {
    short t;
    short dx;
    short dy;
    short pad;
} Ent;

extern Ent D_80130F14[];
extern void *D_8013B0E0, *D_80134DC4;
extern void *D_1F8002D4[];
extern unsigned char D_8009D0AA[], D_8009D0C8[];
extern void AnimLoadDuration(TObj *);
extern int ObjCullRegister(TObj *);
extern void playSFX(int);
extern void FUN_800188e0(TObj *);

void func_8011E154(TObj *o)
{
    TObj *p;
    Ent *t;
    Ent *e;

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
            o->anim = D_8013B0E0;
            o->d3c = (int)D_1F8002D4[0];
            break;
        case 1:
            o->b0d = 0;
            o->w1e = 8;
            o->anim = D_80134DC4;
            o->d3c = (int)D_1F8002D4[0];
            o->wb8 = 0;
            break;
        }
        AnimLoadDuration(o);
        o->b04++;
        break;
    case 1:
        if (!ObjCullRegister(o)) break;
        p = (TObj *)o->d94;
        switch (o->subtype) {
        case 0:
            o->h->p.whole = p->h->p.whole + 0xc;
            o->y.p.whole = p->y.p.whole;
            o->d->p.whole = p->d->p.whole;
            if (D_8009D0AA[0]) o->b04++;
            break;
        case 1:
            t = D_80130F14;
            e = &t[(unsigned short)o->wb8];
            o->h->p.whole = p->h->p.whole + e->dx;
            o->y.p.whole = p->y.p.whole + e->dy;
            o->d->p.whole = p->d->p.whole;
            switch (o->step) {
            case 0:
                e = &t[(unsigned short)o->wb8];
                o->w22 = e->t;
                o->step++;
                break;
            case 1:
                if (((TObj *)o->d94)->step == 1) {
                    o->step++;
                    o->wb8 = 2;
                    e = &t[2];
                    o->w22 = e->t;
                }
                break;
            case 2:
                if (--o->w22 == 0) {
                    if (((TObj *)o->d94)->step == 2) {
                        o->wb8 = 0xb;
                        o->step++;
                        e = &t[0xb];
                        o->w22 = e->t;
                    } else {
                        o->wb8++;
                        e = &t[(unsigned short)o->wb8];
                o->w22 = e->t;
                        if (o->w22 < 0) {
                            o->wb8 = 2;
                            e = &t[2];
                            o->w22 = e->t;
                        }
                    }
                }
                break;
            case 3:
                if (--o->w22 == 0) {
                    o->wb8++;
                    e = &t[(unsigned short)o->wb8];
                o->w22 = e->t;
                }
                break;
            }
            if (D_8009D0C8[0]) o->b04++;
            break;
        }
        break;
    case 2:
        playSFX(9);
        o->b04++;
        break;
    case 3:
        FUN_800188e0(o);
        break;
    }
}
