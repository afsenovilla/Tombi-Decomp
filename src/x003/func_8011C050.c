// FUNC 8011c050 904 X003
// MATCHING 8011c050 904
#include "TOBJ.H"

extern TObj D_800A6038;
extern void *D_80139874[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void playSFX(int);

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    AnimLoadDuration(o);
}

void func_8011C050(TObj *o)
{
    TObj *p = &D_800A6038;

    switch (o->state) {
    case 0:
        if (o->b0c & 2) o->d34 = (o->b0c & 1) * 64 + 0x20;
        else o->d34 = (o->subtype & 1) << 7;
        if (o->d34 <= 0x40) o->velH = 4;
        else o->velH = -4;
        o->state++;
        break;
    case 1:
        o->d38 = (o->d38 + o->velH) & 0xff;
        if (o->velH < 0) {
            if (o->d38 < 0x10) {
                o->d38 = 0x10;
                o->timer = 0x1e;
                o->velH = 8;
                o->state++;
            }
        } else {
            if (o->d38 > 0x70) {
                o->d38 = 0x70;
                o->timer = 0x1e;
                o->velH = -8;
                o->state++;
            }
        }
        break;
    case 2:
        if (--o->timer == -1) {
            if (o->d34 > 0x40) o->state = 3;
            else o->state = 4;
        }
        break;
    case 3:
        o->d38 = (o->d38 + o->velH) & 0xff;
        if (o->d34 < o->d38) {
            o->d38 = o->d34;
            o->timer = 8;
            o->state = 5;
        }
        break;
    case 4:
        o->d38 += o->velH;
        if (o->d38 < o->d34) {
            o->d38 = o->d34;
            o->timer = 8;
            o->state = 5;
        }
        break;
    case 5:
        o->timer = 2;
        o->wac = 4;
        o->state++;
        setAnim(o, D_80139874[0]);
        break;
    case 6:
        if (o->timer == 0) {
            playSFX(0x72);
            p->active = 1;
            p->timer = 0x12;
            p->b9c = 1;
            p->b04 = 1;
            p->step = 0x34;
            p->state = 1;
            p->h->p.whole = o->h->p.whole;
            p->y.p.whole = o->y.p.whole;
            if (o->b0c & 2) {
                p->animFrame = o->b0c & 1;
                p->velY = -0x200;
                p->y.p.whole -= 0x30;
                if (o->b0c & 1) {
                    p->h->p.whole -= 0x30;
                    p->velX = -0x400;
                } else {
                    p->h->p.whole += 0x30;
                    p->velX = 0x400;
                }
            } else {
                p->animFrame = o->subtype & 1;
                p->velY = 0;
                if (o->subtype & 1) {
                    p->h->p.whole -= 0x38;
                    p->velX = -0x400;
                } else {
                    p->h->p.whole += 0x38;
                    p->velX = 0x400;
                }
            }
            o->state++;
        } else {
            o->timer--;
        }
    case 7:
        if (AnimAdvance(o)) {
            o->w74 = 0;
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
