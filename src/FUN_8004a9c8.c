// FUNC 8004a9c8 1204 MAIN0
// MATCHING 8004a9c8 1204
#include "TOBJ.H"
typedef struct { TObj o; char pad[0x32]; short wf2; } BigObj;
extern BigObj DAT_800a6038;
extern unsigned char DAT_800a6038b[];
extern unsigned char DAT_8009cff9;
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_800e8a98(void *, int, int);
extern void FUN_800e96e8(void *, int, int);
extern void FUN_800e940c(void *, int, int);

#define SETANIM(o, a, b) \
    switch ((o)->b6a - 1) { \
    case 0: FUN_800e8a98(DAT_800a6038b, a, b); break; \
    case 1: FUN_800e96e8(DAT_800a6038b, a, b); break; \
    case 2: FUN_800e940c(DAT_800a6038b, a, b); break; \
    }

static __inline__ void SetAnim(TObj *self, short n)
{
    self->wac = n;
    self->anim = (*(void ***)&self->wa8)[n];
    AnimLoadDuration(self);
}

void FUN_8004a9c8(TObj *o)
{
    TObj *p = &DAT_800a6038.o;
    int d;

    switch (o->state) {
    case 0:
        if ((*(int *)&p->b04 & 0xffffff) == 0x30504) {
            o->h->p.whole = p->h->p.whole;
            o->d34 = ((BigObj *)p)->wf2;
            o->y.p.whole = p->y.p.whole + 0x14;
            p->b04 = 4;
            p->step = 6;
            p->state = 0;
            SETANIM(o, 0, 6);
            SetAnim(o, 1);
            o->velV = 0x100;
            o->state++;
            o->b0f = p->b0f - 1;
        }
        break;
    case 1:
        o->y.raw += (short)(o->velV += 8) << 8;
        p->y.raw += o->velV << 8;
        if (o->y.p.whole > (d = o->d34)) {
            o->timer = 0x1e;
            o->y.p.whole = d;
            o->state++;
            SetAnim(o, 0);
        }
        break;
    case 2:
        if (--o->timer == -1)
            goto next;
        break;
    case 3:
        p->y.raw += 0x8000;
        if (o->d34 < p->y.p.whole) {
            o->state++;
            SetAnim(o, 0);
        }
        break;
    case 4:
        o->timer = 0x10;
        p->animFrame = 0;
        SETANIM(o, 1, 4);
    next:
        o->state++;
        break;
    case 5:
        AnimAdvance(p);
        p->h->p.whole++;
        if (--o->timer == -1) {
            o->timer = 0x1e;
            o->state++;
            p->animFrame = 0;
            SETANIM(o, 0, 6);
        }
        break;
    case 6:
        if (--o->timer == -1) {
            o->animFrame = 0;
            o->state++;
            SetAnim(o, 3);
        }
        break;
    case 7:
        AnimAdvance(o);
        o->h->p.whole++;
        if (o->h->p.whole >= p->h->p.whole) {
            p->b04 = 4;
            p->step = 5;
            p->state = 2;
            DAT_8009cff9 = 0;
            o->active = 2;
            o->b04 = 3;
        }
        break;
    }
}
