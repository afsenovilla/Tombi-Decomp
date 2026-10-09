// FUNC 80126ce0 1456 X009
// MATCHING 80126ce0 1456
#include "TOBJ.H"

extern void *D_8012EEA0[], *D_8012EEA4[], *D_8012EE80[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern short func_8004065C(TObj *, short, short, short);
extern short TileCollideAt(TObj *, short, short);

static __inline__ int wall(TObj *o)
{
    unsigned char b = o->b9d;
    unsigned short f = o->animFrame;
    short v, s;

    if ((b & 2) && f == (b & 1))
        return 1;
    s = f;
    v = 0x10;
    if (s)
        v = -0x10;
    return func_8004065C(o, o->h->p.whole + v, o->y.p.whole + 0x30, s);
}

static __inline__ void setanim(TObj *o, void *a)
{
    o->anim = a;
    AnimLoadDuration(o);
}

void func_80126CE0(TObj *o)
{
    switch (o->state) {
    case 0:
        o->b6b = 1;
        o->active = 4;
        o->w9a = 1;
        o->state++;
        if (o->animFrame) {
            o->velH = -0x300;
            o->velX = 0x10;
        } else {
            o->velH = 0x300;
            o->velX = -0x10;
        }
        o->timer = 0x1c;
        o->b0a = 2;
        o->d8c = 0;
        o->wac = 0;
        setanim(o, D_8012EEA0[0]);
        break;
    case 1:
        if (--o->timer == -1) {
            o->state++;
        }
        o->h->raw += o->velH << 8;
        if (o->animFrame & 1) {
            o->d8c = (o->d8c + 0x14) & 0xff;
        } else {
            o->d8c = (o->d8c - 0x14) & 0xff;
        }
        if (wall(o)) {
            o->state = 3;
            o->animFrame = 1 - o->animFrame;
        } else {
            TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x30);
        }
        break;
    case 2:
        o->h->raw += o->velH << 8;
        if (o->velH != 0) {
            o->velH += o->velX;
        } else {
            o->w9a = 0;
        }
        if (o->animFrame & 1) {
            o->d8c = (o->d8c + 0xa) & 0xff;
        } else {
            o->d8c = (o->d8c - 0xa) & 0xff;
        }
        if (wall(o)) {
            o->state = 3;
            o->animFrame = 1 - o->animFrame;
        } else {
            TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x30);
        }
        if (o->d8c == 0) {
            o->state = 5;
            o->b0a = 0;
            o->wac = 1;
            setanim(o, D_8012EEA4[0]);
        }
        break;
    case 3:
        o->velV = 0x200;
        o->velY = -0x10;
        o->timer = 0;
        o->d8c = 0;
        o->state++;
        if (o->animFrame) {
            o->velH = -0x100;
        } else {
            o->velH = 0x100;
        }
        o->b6b = 0;
        o->w9a = 0;
    case 4:
        o->h->raw += o->velH << 8;
        wall(o);
        o->y.raw -= o->velV << 8;
        o->velV += o->velY;
        if (o->animFrame & 1) {
            o->d8c = (o->d8c + 8) & 0xff;
        } else {
            o->d8c = (o->d8c - 8) & 0xff;
        }
        if (o->velV < -0x1ff) {
            o->state = 5;
            o->wac = 1;
            setanim(o, D_8012EEA4[0]);
        }
        break;
    case 5:
        if (AnimAdvance(o)) {
            o->d8c = 0;
            o->b0a = 0;
            o->timer = 0x20;
            o->wac = 0;
            o->state++;
            setanim(o, D_8012EE80[0]);
            o->w9a = 0;
        }
        break;
    case 6:
        AnimAdvance(o);
        o->h->raw += o->velH << 8;
        if (!wall(o)) {
            TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x30);
        }
        if (--o->timer == -1) {
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
            o->active = 1;
            o->b6b = 0;
            o->b0f -= 2;
        }
        break;
    }
}
