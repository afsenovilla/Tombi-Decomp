// FUNC 8004a4cc 1276 MAIN0
// MATCHING 8004a4cc 1276
#include "TOBJ.H"
extern TObj DAT_800a6038;
extern unsigned char DAT_800a6038b[];
extern unsigned char DAT_8009cff9;
extern unsigned char DAT_8009c93c;
extern unsigned char DAT_8009c975;
extern short DAT_1f800176;
extern TObj *DAT_1f8001d4;
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

void FUN_8004a4cc(TObj *o)
{
    TObj *p = &DAT_800a6038;

    switch (o->state) {
    case 0:
        if ((*(int *)&p->b04 & 0xffffff) == 0x23001) {
            p->active = 5;
            p->visible = 1;
            p->b04 = 4;
            p->step = 0;
            p->state = 0;
            o->timer = 0x32;
            o->state++;
        }
        break;
    case 1:
        AnimAdvance(o);
        if (--o->timer == -1) {
            o->timer = 0x10;
            o->state++;
            p->step = 6;
            SETANIM(o, 0, 6);
        }
        break;
    case 2:
        o->h->p.whole++;
        if (o->h->p.whole > DAT_1f800176 + 0x136)
            o->h->p.whole = DAT_1f800176 + 0x136;
        AnimAdvance(o);
        if (--o->timer == -1) {
            o->timer = 0x28;
            o->state++;
            SetAnim(o, 0);
            o->b0f = p->b0f - 2;
        }
        break;
    case 3:
        if (--o->timer == -1)
            goto next;
        break;
    case 4:
        p->animFrame = 0;
        SETANIM(o, 1, 0);
    next:
        o->state++;
        break;
    case 5:
        AnimAdvance(p);
        p->h->p.whole++;
        if (p->h->p.whole >= o->h->p.whole) {
            p->h->p.whole = o->h->p.whole;
            o->timer = 0x1e;
            o->state++;
            SETANIM(o, 0, 6);
        }
        break;
    case 6:
        if (--o->timer == -1)
            o->state++;
        p->y.raw -= 0x8000;
        break;
    case 7:
        SetAnim(o, 1);
        o->timer = 0x46;
        o->velV = 0;
        o->state++;
        break;
    case 8:
        if (--o->timer == -1) {
            o->state++;
            DAT_8009c93c = 0;
            DAT_8009c975 = 3;
        }
        if ((o->velV += 0x20) > 0x600)
            o->velV = 0x600;
        o->y.raw -= o->velV << 8;
        p->y.raw -= o->velV << 8;
        break;
    case 9:
        if ((o->velV += 0x20) > 0x600)
            o->velV = 0x600;
        o->y.raw -= o->velV << 8;
        p->y.raw -= o->velV << 8;
        if (DAT_8009c975 == 1) {
            DAT_8009cff9 = 1;
            DAT_1f8001d4->w4c = 7;
            DAT_1f8001d4->w4e = 0;
        }
        break;
    }
}
