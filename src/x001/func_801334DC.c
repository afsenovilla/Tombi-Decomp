// FUNC 801334dc 1352 X001
// MATCHING 801334dc 1352
#include "TOBJ.H"

extern short D_8007A5F0[], D_8007A3F0[];
extern void *D_8013F034[];
extern void *D_8013F064[];
extern void AnimLoadDuration(TObj *);
extern void func_80132A3C(TObj *);

#define MOVE() \
    o->h->raw += (D_8007A5F0[*(unsigned char *)&o->d88] * o->velH) >> 4; \
    o->y.raw += (D_8007A3F0[*(unsigned char *)&o->d88] * o->velH) >> 4; \
    o->wac = ((o->d88 + 8) & 0xff) >> 4; \
    o->anim = D_8013F034[o->wac];

void func_801334DC(TObj *o)
{
    TObj *p;

    switch (o->state) {
    case 0:
        o->active = 1;
        o->w7a = 0x10;
        o->w74 = 0x20;
        o->d84 = 0xc0;
        o->d88 = 0xc0;
        o->timer = 0x3c;
        o->velH = 0x100;
        o->substep = 0;
        o->d8c = 0;
        o->wac = 0xc;
        o->state++;
        o->anim = D_8013F064[0];
        AnimLoadDuration(o);
        for (p = (TObj *)o->d94; p; p = (TObj *)p->d94) {
            p->d84 = 0xc0;
            p->timer = 0x10;
            p->active = 1;
        }
        break;
    case 1:
        func_80132A3C(o);
        MOVE();
        break;
    case 2:
        o->d30 = 0;
        o->state++;
    case 3:
        switch (o->substep) {
        case 0:
            {
                unsigned char v = o->d30 - o->d88;
                if (v == 0) o->substep = 3;
                else if (v < 0x80) o->substep++;
                else o->substep = 2;
            }
            break;
        case 1:
            o->d88 = (o->d88 + 1) & 0xff;
            if (o->d88 == o->d30) o->substep = 3;
            break;
        case 2:
            o->d88 = (o->d88 - 1) & 0xff;
            if (o->d88 == o->d30) o->substep = 3;
            break;
        case 3:
            o->state++;
            o->substep = 0;
            break;
        }
        o->d84 = o->d88;
        MOVE();
        break;
    case 4:
        o->timer = -0x1000;
        o->state++;
    case 5:
        func_80132A3C(o);
        MOVE();
        if (o->h->p.whole >= 0x551) {
            o->state++;
            o->substep = 0;
        }
        break;
    case 6:
        o->d30 = 0x80;
        o->state++;
    case 7:
        switch (o->substep) {
        case 0:
            if (o->d30 == o->d88) o->substep = 2;
            else o->substep++;
            break;
        case 1:
            o->d88 = (o->d88 - 1) & 0xff;
            if (o->d88 == o->d30) o->substep = 2;
            break;
        case 2:
            o->state++;
            o->substep = 0;
            break;
        }
        o->d84 = o->d88;
        MOVE();
        break;
    case 8:
        o->d30 = 0x80;
        o->state++;
    case 9:
        switch (o->substep) {
        case 0:
            if (o->d30 == o->d88) o->substep = 2;
            else o->substep++;
            break;
        case 1:
            o->d88 = (o->d88 + 1) & 0xff;
            if (o->d88 == o->d30) o->substep = 2;
            break;
        case 2:
            o->state++;
            o->substep = 0;
            break;
        }
        o->d84 = o->d88;
        MOVE();
        break;
    case 10:
        o->timer = -0x1000;
        o->state++;
    case 11:
        func_80132A3C(o);
        MOVE();
        if (o->h->p.whole < 0x118) {
            o->state++;
            o->substep = 0;
        }
        break;
    case 12:
        o->d30 = 0;
        o->state++;
    case 13:
        switch (o->substep) {
        case 0:
            if (o->d30 == o->d88) o->substep = 2;
            else o->substep++;
            break;
        case 1:
            o->d88 = (o->d88 - 1) & 0xff;
            if (o->d88 == o->d30) o->substep = 2;
            break;
        case 2:
            o->state++;
            o->substep = 0;
            break;
        }
        o->d84 = o->d88;
        MOVE();
        break;
    }
}
