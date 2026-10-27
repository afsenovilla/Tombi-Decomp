// FUNC 8012c598 852 X000
// MATCHING 8012c598 852
#include "TOBJ.H"
extern char MOVE_TABLE_0[];
extern unsigned char KOMA_PIG_IDLE_ANIM_BY_SUBTYPE[];
extern void *KOMA_PIG_ANIMS[];
extern char D_8013A8F4[];
extern char D_8013A908[];
extern char D_8013A488[];
extern char D_8013A49C[];
extern char D_8013A7AC[];
extern char D_8013A7C0[];
extern char D_8013AB78[];
extern char D_8013A92C[];
static void *D_8013A224[9] = {
    D_8013A8F4, D_8013A908, D_8013A488, D_8013A49C,
    D_8013A7AC, D_8013A7C0, D_8013AB78, D_8013A7C0,
    D_8013A92C,
};
extern void *D_8013A268[];
extern unsigned char KOMA_PIG_ANIM_BOXES[];
extern int AnimAdvanceWithBox(TObj *);

#define SETBOX(o) \
    { \
        unsigned char *b = KOMA_PIG_ANIM_BOXES + ((unsigned short *)o->anim)[1] * 4; \
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

void komaPigSubtype04Step(TObj *o)
{
    TObj *p;
    char k;

    switch (o->state) {
    case 0:
        o->movetab = MOVE_TABLE_0;
        o->timer = 0;
        o->w22 = 0;
        o->b69 = 0;
        o->state++;
        o->wac = KOMA_PIG_IDLE_ANIM_BY_SUBTYPE[o->subtype];
        o->anim = KOMA_PIG_ANIMS[o->wac];
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
