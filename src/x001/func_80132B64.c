// FUNC 80132b64 2424 X001
// MATCHING 80132b64 2424
#include "TOBJ.H"

extern short D_8007A5F0[], D_8007A3F0[];
extern void *D_8013F034[];
extern void *D_8013F064[];
extern void AnimLoadDuration(TObj *);
extern void func_80132A3C(TObj *);
extern int func_80132964(TObj *);

#define MOVE() \
    o->h->raw += (D_8007A5F0[*(unsigned char *)&o->d88] * o->velH) >> 4; \
    o->y.raw += (D_8007A3F0[*(unsigned char *)&o->d88] * o->velH) >> 4; \
    o->wac = ((o->d88 + 8) & 0xff) >> 4; \
    o->anim = D_8013F034[o->wac];

#define TURN3() \
    switch (o->substep) { \
    case 0: \
        { \
            unsigned char v = o->d30 - o->d88; \
            if (v == 0) o->substep = 3; \
            else if (v < 0x80) o->substep++; \
            else o->substep = 2; \
        } \
        break; \
    case 1: \
        o->d88 = (o->d88 + 1) & 0xff; \
        if (o->d88 == o->d30) o->substep = 3; \
        break; \
    case 2: \
        o->d88 = (o->d88 - 1) & 0xff; \
        if (o->d88 == o->d30) o->substep = 3; \
        break; \
    case 3: \
        o->state++; \
        o->substep = 0; \
        break; \
    } \
    o->d84 = o->d88; \
    MOVE();

#define TURN2(step) \
    switch (o->substep) { \
    case 0: \
        if (o->d30 == o->d88) o->substep = 2; \
        else o->substep++; \
        break; \
    case 1: \
        o->d88 = (o->d88 step 1) & 0xff; \
        if (o->d88 == o->d30) o->substep = 2; \
        break; \
    case 2: \
        o->state++; \
        o->substep = 0; \
        break; \
    } \
    o->d84 = o->d88; \
    MOVE();

void func_80132B64(TObj *o)
{
    TObj *p;
    int r;

    switch (o->state) {
    case 0:
        o->active = 1;
        o->w7a = 0x10;
        o->w74 = 0x20;
        o->d84 = 0xc0;
        o->d88 = 0xc0;
        o->timer = 0x6e;
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
        TURN3();
        break;
    case 4:
        o->timer = -0x1000;
        o->state++;
    case 5:
        func_80132A3C(o);
        MOVE();
        if (o->h->p.whole >= 0x551) {
            if (o->y.p.whole < -0x140) o->state = 0x15;
            else o->state = 0x1b;
            o->substep = 0;
        }
        r = func_80132964(o);
        if (r == 1) {
            o->state = 6;
            o->substep = 0;
        } else if (r == 2) {
            if (o->y.p.whole < -0x140) o->state = 0x15;
            else o->state = 0x1b;
            o->substep = 0;
        }
        break;
    case 6:
        o->d30 = (o->d88 + 0x10) & 0xff;
        o->state++;
    case 7:
        TURN3();
        break;
    case 8:
        o->timer = -0x1000;
        o->state++;
    case 9:
        func_80132A3C(o);
        MOVE();
        if (o->y.p.whole < -0x140) {
            o->state = 0x12;
            o->substep = 0;
        }
        break;
    case 10:
        o->d30 = (o->d88 - 0x10) & 0xff;
        o->state++;
    case 11:
        TURN3();
        break;
    case 12:
        o->timer = -0x1000;
        o->state++;
    case 13:
        func_80132A3C(o);
        MOVE();
        if (o->y.p.whole < -0x140) {
            o->state++;
            o->substep = 0;
        }
        break;
    case 14:
        o->d30 = 0x80;
        o->state++;
    case 15:
        TURN3();
        break;
    case 16:
        o->timer = -0x1000;
        o->state++;
    case 17:
        func_80132A3C(o);
        MOVE();
        if (o->h->p.whole < 0x118) {
            if (o->y.p.whole < -0x140) o->state = 0x18;
            else o->state = 0x12;
            o->substep = 0;
        }
        r = func_80132964(o);
        if (r == 1) {
            o->state = 0xa;
            o->substep = 0;
        } else if (r == 2) {
            if (o->y.p.whole < -0x140) o->state = 0x18;
            else o->state = 0x12;
            o->substep = 0;
        }
        break;
    case 18:
        o->d30 = 0;
        o->state++;
    case 19:
        TURN2(-);
        break;
    case 20:
        o->state = 4;
        o->substep = 0;
        break;
    case 21:
        o->d30 = 0x80;
        o->state++;
    case 22:
        TURN2(-);
        break;
    case 23:
        o->state = 0x10;
        o->substep = 0;
        break;
    case 24:
        o->d30 = 0;
        o->state++;
    case 25:
        TURN2(+);
        break;
    case 26:
        o->state = 4;
        o->substep = 0;
        break;
    case 27:
        o->d30 = 0x80;
        o->state++;
    case 28:
        TURN2(+);
        break;
    case 29:
        o->state = 0x10;
        o->substep = 0;
        break;
    }
}
