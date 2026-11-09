// FUNC 8012ddd4 1444 X003
// MATCHING 8012ddd4 1444
// VERSION SLPS_01144 x03 8012CEF4 1632
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"

extern unsigned short D_80135064[], D_80076F50[];
extern unsigned char D_8013505C[];
extern void *D_80137478[], *D_80137484[], *D_8013748C[];
extern void *D_80137474[];
extern int func_80020958(void);
extern void func_80020DE4(TObj *);
extern int func_80020E38(TObj *);
extern short func_80041224(TObj *, short, short, short);
static __inline__ int wingedForkPigCheckWall(TObj *o)
{
    short d;

    if ((o->b9d & 2) && o->animFrame == (o->b9d & 1))
        return 1;
    if (o->animFrame & 1)
        d = -16;
    else
        d = 16;
    return func_80041224(o, o->h->p.whole + d, o->y.p.whole, o->animFrame) != 0;
}

#define WOBBLE() \
    o->ba7 += 2; \
    o->y.raw += (short)(D_80076F50[o->ba7] << 2);

static __inline__ short chk2(TObj *o)
{
    short r;
    if (o->animFrame & 1) r = o->a.p.whole < o->wb6;
    else r = o->a.p.whole > o->wb8;
    if (r) return 1;
    return 0;
}

#define chk(o) (wingedForkPigCheckWall(o) || chk2(o))

void func_8012CEF4(TObj *o)
{
    switch (o->state) {
    case 0:
        o->velX = o->animFrame << 2;
        o->state++;
        o->timer = D_80135064[func_80020958() & 7];
        o->wac = 1;
        o->anim = D_80137478[0];
        func_80020DE4(o);
        break;
    case 1:
        if (o->animFrame) o->a.p.whole--;
        else o->a.p.whole++;
        WOBBLE();
        func_80020E38(o);
        if (--o->timer == -1) {
            o->timer = 0x3c;
            o->wac = 4;
            o->state++;
            o->anim = D_80137484[0];
            func_80020DE4(o);
        } else if (chk(o)) {
            o->wb0 = 1;
            o->ba5 = o->state;
            o->state = 4;
            o->substep = 0;
        }
        break;
    case 2:
        if (o->animFrame) o->a.raw += -0x8000;
        else o->a.raw += 0x8000;
        WOBBLE();
        if (chk(o)) goto hit;
        if (o->timer) o->timer--;
        if (func_80020E38(o) && o->timer == 0) {
            o->timer = 0x3c;
            o->state++;
            o->ba6 = (o->ba6 + 1) & 1;
        }
        break;
    case 3:
        if (o->animFrame) o->a.raw += -0x8000;
        else o->a.raw += 0x8000;
        WOBBLE();
        if (func_80020E38(o)) {
            if (*(signed char *)&o->ba6) {
                if ((func_80020958() & 3) == 1) {
                    o->step++;
                    o->state = 0;
                    break;
                }
            } else if (D_8013505C[func_80020958() & 7] == 1) {
                o->step = 3;
                o->state = 0;
                o->velH = 0x80;
                break;
            }
            if (o->timer == 0) {
                o->state = 0;
                break;
            }
        }
        if (chk(o)) {
        hit:
            o->wb0 = 4;
            o->ba5 = o->state;
            o->state = 4;
            o->substep = 0;
            break;
        }
        if (o->timer) o->timer--;
        break;
    case 4:
        switch (o->substep) {
        case 0:
            o->substep++;
            o->wac = 6;
            o->anim = D_8013748C[0];
            func_80020DE4(o);
            break;
        case 1:
            if (func_80020E38(o)) {
                unsigned short w;
                int f;
                unsigned char st;
                f = 1 - o->animFrame;
                st = o->ba5;
                w = o->wb0;
                o->animFrame = f;
                o->velX = f << 2;
                o->wac = w;
                o->state = st;
                o->anim = D_80137474[(short)w];
                func_80020DE4(o);
            }
            break;
        }
        WOBBLE();
        break;
    }
}
#else
#include "TOBJ.H"

extern unsigned short D_80135E68[];
extern unsigned char D_80135E60[];
extern void *D_8013A69C[], *D_8013A6A8[], *D_8013A6B0[];
extern void *WINGED_FORK_PIG_ANIMS[];
extern int Rand(void);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int wingedForkPigCheckWall(TObj *);

#define WOBBLE() \
    o->ba7 += 2; \
    o->y.raw += (short)(((unsigned short *)NEG_SIN_TABLE)[o->ba7] << 2);

static __inline__ short chk2(TObj *o)
{
    short r;
    if (o->animFrame & 1) r = o->a.p.whole < o->wb6;
    else if (o->y.p.whole < -0x46a) r = o->wb8 - 0x14 < o->a.p.whole;
    else r = o->a.p.whole > o->wb8;
    if (r) return 1;
    return 0;
}

#define chk(o) (wingedForkPigCheckWall(o) || chk2(o))

void wingedForkPigWander(TObj *o)
{
    switch (o->state) {
    case 0:
        o->velX = o->animFrame << 2;
        o->state++;
        o->timer = D_80135E68[Rand() & 7];
        o->wac = 1;
        o->anim = D_8013A69C[0];
        AnimLoadDuration(o);
        break;
    case 1:
        if (o->animFrame) o->a.p.whole--;
        else o->a.p.whole++;
        WOBBLE();
        AnimAdvance(o);
        if (--o->timer == -1) {
            o->timer = 0x3c;
            o->wac = 4;
            o->state++;
            o->anim = D_8013A6A8[0];
            AnimLoadDuration(o);
        } else if (chk(o)) {
            o->wb0 = 1;
            o->ba5 = o->state;
            o->state = 4;
            o->substep = 0;
        }
        break;
    case 2:
        if (o->animFrame) o->a.raw += -0x8000;
        else o->a.raw += 0x8000;
        WOBBLE();
        if (chk(o)) goto hit;
        if (o->timer) o->timer--;
        if (AnimAdvance(o) && o->timer == 0) {
            o->timer = 0x3c;
            o->state++;
            o->ba6 = (o->ba6 + 1) & 1;
        }
        break;
    case 3:
        if (o->animFrame) o->a.raw += -0x8000;
        else o->a.raw += 0x8000;
        WOBBLE();
        if (AnimAdvance(o)) {
            if (*(signed char *)&o->ba6) {
                if ((Rand() & 3) == 1) {
                    o->step++;
                    o->state = 0;
                    break;
                }
            } else if (D_80135E60[Rand() & 7] == 1) {
                o->step = 3;
                o->state = 0;
                o->velH = 0x80;
                break;
            }
            if (o->timer == 0) {
                o->state = 0;
                break;
            }
        }
        if (chk(o)) {
        hit:
            o->wb0 = 4;
            o->ba5 = o->state;
            o->state = 4;
            o->substep = 0;
            break;
        }
        if (o->timer) o->timer--;
        break;
    case 4:
        switch (o->substep) {
        case 0:
            o->substep++;
            o->wac = 6;
            o->anim = D_8013A6B0[0];
            AnimLoadDuration(o);
            break;
        case 1:
            if (AnimAdvance(o)) {
                unsigned short w;
                int f;
                unsigned char st;
                f = 1 - o->animFrame;
                st = o->ba5;
                w = o->wb0;
                o->animFrame = f;
                o->velX = f << 2;
                o->wac = w;
                o->state = st;
                o->anim = WINGED_FORK_PIG_ANIMS[(short)w];
                AnimLoadDuration(o);
            }
            break;
        }
        WOBBLE();
        break;
    }
}
#endif
