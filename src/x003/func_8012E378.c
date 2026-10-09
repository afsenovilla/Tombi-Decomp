// FUNC 8012e378 824 X003
// MATCHING 8012e378 824
#include "TOBJ.H"

extern void *D_8013A698[];
extern unsigned short D_8007A3F0[];
extern short D_8007A5F0[];
extern unsigned short D_1F80016E;
extern short isObjectBelowGround(TObj *o);
extern int AnimAdvance(TObj *o);
extern void AnimLoadDuration(TObj *o);
extern int func_8012D518(TObj *o);

static __inline__ int side(TObj *o)
{
    unsigned short d = D_1F80016E;
    if ((unsigned short)(d - o->y.p.whole + 0x60) < 0xc0) return 0;
    return (short)d > o->y.p.whole ? 6 : 2;
}

void func_8012E378(TObj *o)
{
    int r;
    int i;
    short sp;
    int c, s;
    short t;

    switch (o->state) {
    case 0:
        o->velX = o->animFrame << 2;
        t = isObjectBelowGround(o);
        if (o->animFrame == t) {
            o->state = 2;
            o->wac = 3;
        } else {
            o->wac = 6;
            o->state++;
        }
        o->anim = D_8013A698[o->wac];
        AnimLoadDuration(o);
        break;
    case 1:
        o->ba7 += 2;
        o->y.raw += (short)(D_8007A3F0[o->ba7] << 2);
        if (AnimAdvance(o)) {
            o->wac = 3;
            o->state++;
            o->animFrame = 1 - o->animFrame;
            o->velX = o->animFrame << 2;
            o->anim = D_8013A698[3];
            AnimLoadDuration(o);
        }
        break;
    case 2:
        o->ba7 += 2;
        o->y.raw += (short)(D_8007A3F0[o->ba7] << 2);
        if (AnimAdvance(o)) o->state++;
        break;
    case 3:
        r = side(o);
        if (r == 0) goto done;
        o->timer = 100;
        o->velH = 0;
        o->state++;
        if (r & 4) {
            o->wac = 2;
            if (o->animFrame) o->velX = 5;
            else o->velX = 7;
        } else {
            o->wac = 0;
            if (o->animFrame) o->velX = 3;
            else o->velX = 1;
        }
        o->anim = D_8013A698[o->wac];
        AnimLoadDuration(o);
        break;
    case 4:
        AnimAdvance(o);
        i = (o->velX << 5) & 0xe0;
        sp = o->velH;
        c = sp * D_8007A5F0[i];
        s = sp * (short)D_8007A3F0[i];
        o->a.raw += (short)(c >> 12) << 8;
        o->y.raw += (short)(s >> 12) << 8;
        if (func_8012D518(o)) o->timer = 0;
        o->velH += 0x20;
        if (o->velH > 0x180) o->velH = 0x180;
        if (--o->timer == -1) {
        done:
            o->step = 1;
            o->state = 0;
            o->substep = 0;
        }
        break;
    }
}
