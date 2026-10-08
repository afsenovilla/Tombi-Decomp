// FUNC 8012c598 852 X000
// MATCHING 8012c598 852
#include "TOBJ.H"
extern char D_80077CDC[];
extern unsigned char D_801390EC[];
extern void *D_8013A160[];
extern void *D_8013A224[];
extern void *D_8013A268[];
extern unsigned char D_80138FD8[];
extern int AnimAdvanceWithBox(TObj *);

#define SETBOX(o) \
    { \
        unsigned char *b = D_80138FD8 + ((unsigned short *)o->anim)[1] * 4; \
        o->box0 = *b++; \
        o->box1 = *b++; \
        o->box2 = b[0]; \
        o->box3 = b[1]; \
        o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff; \
    }

#define FOLLOW(o, p) \
    p = (TObj *)o->d94; \
    o->h->p.whole = p->h->p.whole; \
    o->y.p.whole = p->y.p.whole - 30; \
    o->d8c = -(p->d8c >> 4) & 0xff;

void func_8012C598(TObj *o)
{
    TObj *p;
    char k;

    switch (o->state) {
    case 0:
        o->movetab = D_80077CDC;
        o->timer = 0;
        o->w22 = 0;
        o->b69 = 0;
        o->state++;
        o->wac = D_801390EC[o->subtype];
        o->anim = D_8013A160[o->wac];
        SETBOX(o);
        break;
    case 1:
        AnimAdvanceWithBox(o);
        FOLLOW(o, p);
        if ((unsigned short)(o->d->p.whole - *(unsigned short *)0x1F800172 + 45) >= 91
            || (unsigned short)(o->h->p.whole - *(unsigned short *)0x1F80016A + 64) >= 129
            || (unsigned short)(o->y.p.whole - *(unsigned short *)0x1F80016E + 32) >= 65) {
            o->timer = 0;
        } else if (o->timer++ > 0x78) {
            o->state++;
        }
        break;
    case 2:
        FOLLOW(o, p);
        o->state++;
        o->timer = 0xb4;
        o->wac = 0x31;
        o->anim = D_8013A224[0];
        SETBOX(o);
        break;
    case 3:
        FOLLOW(o, p);
        AnimAdvanceWithBox(o);
        k = 0x42;
        if (--o->timer == -1) {
            o->wac = k;
            o->state++;
            o->anim = D_8013A268[0];
            SETBOX(o);
        }
        break;
    case 4:
        FOLLOW(o, p);
        if (AnimAdvanceWithBox(o)) {
            o->state = 0;
            o->step++;
        }
        break;
    }
}
