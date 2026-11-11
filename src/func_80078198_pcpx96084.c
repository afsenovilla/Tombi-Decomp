// VERSION PCPX_96084 tomba 80078198 860
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of komaPigSubtype04Step (src/komaPigSubtype04Step.c): halfword tables, field +0xA8, player position at 0x1F8002BA..
#include "TOBJ.H"
extern char D_800A1898[];
extern unsigned short D_800AEBB4[];
extern void *D_800133C8[];
extern void *D_8001348C[];
extern void *D_800134D0[];
extern unsigned short D_800AE9A4[];
extern int func_800707EC(TObj *);

#define SETBOX(o) \
    { \
        unsigned short *b = D_800AE9A4 + ((unsigned short *)o->anim)[1] * 4; \
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

void func_80078198_pcpx96084(TObj *o)
{
    TObj *p;
    char k;

    switch (o->state) {
    case 0:
        o->movetab = D_800A1898;
        o->timer = 0;
        o->w22 = 0;
        o->b69 = 0;
        o->state++;
        o->wa8 = D_800AEBB4[o->subtype];
        o->anim = D_800133C8[o->wa8];
        SETBOX(o);
        break;
    case 1:
        func_800707EC(o);
        FOLLOW(o, p);
        if ((unsigned short)(o->d->p.whole - *(unsigned short *)0x1F8002C2 + 45) >= 91
            || (unsigned short)(o->h->p.whole - *(unsigned short *)0x1F8002BA + 64) >= 129
            || (unsigned short)(o->y.p.whole - *(unsigned short *)0x1F8002BE + 32) >= 65) {
            o->timer = 0;
        } else if (o->timer++ > 0x78) {
            o->state++;
        }
        break;
    case 2:
        FOLLOW(o, p);
        o->state++;
        o->timer = 0xb4;
        o->wa8 = 0x31;
        o->anim = D_8001348C[0];
        SETBOX(o);
        break;
    case 3:
        FOLLOW(o, p);
        func_800707EC(o);
        k = 0x42;
        if (--o->timer == -1) {
            o->wa8 = k;
            o->state++;
            o->anim = D_800134D0[0];
            SETBOX(o);
        }
        break;
    case 4:
        FOLLOW(o, p);
        if (func_800707EC(o)) {
            o->state = 0;
            o->step++;
        }
        break;
    }
}
#endif
