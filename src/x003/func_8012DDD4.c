// FUNC 8012ddd4 1444 X003
// MATCHING 8012ddd4 1444
#include "TOBJ.H"

extern unsigned short D_8007A3F0[];
extern unsigned short D_80135E68[];
extern unsigned char D_80135E60[];
extern void *D_8013A69C[], *D_8013A6A8[], *D_8013A6B0[];
extern void *D_8013A698[];
extern int Rand(void);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int func_8012D66C(TObj *);

#define WOBBLE() \
    o->ba7 += 2; \
    o->y.raw += (short)(D_8007A3F0[o->ba7] << 2);

static __inline__ short chk2(TObj *o)
{
    short r;
    if (o->animFrame & 1) r = o->a.p.whole < o->wb6;
    else if (o->y.p.whole < -0x46a) r = o->wb8 - 0x14 < o->a.p.whole;
    else r = o->a.p.whole > o->wb8;
    if (r) return 1;
    return 0;
}

#define chk(o) (func_8012D66C(o) || chk2(o))

void func_8012DDD4(TObj *o)
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
                o->anim = D_8013A698[(short)w];
                AnimLoadDuration(o);
            }
            break;
        }
        WOBBLE();
        break;
    }
}
