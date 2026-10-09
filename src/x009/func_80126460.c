// FUNC 80126460 1072 X009
// MATCHING 80126460 1072
#include "TOBJ.H"

extern char D_80077D00[];
extern void *D_8012EE80[];
extern short D_8007A5F0[];
extern short D_1F800176;
extern TObj D_800A6038;
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern void playSFX(int);
extern void FUN_8001fa88(TObj *, int);
extern int func_8004065C(TObj *, int, int, int);
extern short TileCollideAt(TObj *, int, int);

static __inline__ int Blocked(TObj *o)
{
    short d;
    short af = o->animFrame;
    if ((o->b9d & 2) && o->animFrame == (o->b9d & 1)) return 1;
    if (af) d = -0x10;
    else d = 0x10;
    return (short)func_8004065C(o, (short)(o->h->p.whole + d), (short)(o->y.p.whole + 0x30), af);
}

void func_80126460(TObj *o)
{
    short s;
    short c;
    TObj *p;
    int k;
    int b;
    unsigned short a;

    switch (o->state) {
    case 0:
        o->movetab = D_80077D00;
        o->d88 = 0;
        o->velV = 0x100;
        o->wac = 0;
        o->state++;
        o->anim = D_8012EE80[0];
        AnimLoadDuration(o);
        playSFX(0x97);
        break;
    case 1:
        AnimAdvance(o);
        FUN_8001fa88(o, o->animFrame);
        s = Blocked(o);
        o->y.raw -= (D_8007A5F0[*(unsigned char *)&o->d88] * o->velV) >> 4;
        c = TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x30));
        if (s && c) {
            o->timer = 0xc;
            o->state = 6;
            break;
        }
        o->d88 = (o->d88 + 1) & 0xff;
        if (o->d88 >= 0xc0) {
            o->d88 = 0;
            o->state++;
            playSFX(0x97);
        }
        break;
    case 2:
        if (o->visible == 0) {
            o->step = 0;
            o->state = 0;
            break;
        }
        p = *(TObj **)&o->wa8;
        a = o->h->p.whole;
        if (((a - (unsigned short)p->h->p.whole + 0x80) & 0xffff) <= 0x100) {
            b = (short)a - 0xa0;
            k = D_1F800176 - b < 0;
            if (o->animFrame == p->animFrame) {
                if (o->animFrame != k) o->animFrame = 1 - o->animFrame;
                o->state++;
            } else if (o->animFrame == k) {
                o->state++;
            } else {
                o->state--;
            }
        } else if (p->state == 3) {
            o->state = 1;
        } else {
            o->state = 5;
        }
        break;
    case 3:
        o->movetab = D_80077D00;
        o->velV = -0x100;
        o->velY = 2;
        o->state++;
    case 4:
        AnimAdvance(o);
        FUN_8001fa88(o, o->animFrame);
        Blocked(o);
        o->y.raw += o->velV << 8;
        TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x30));
        o->velV += o->velY;
        if (o->velV >= 0x100) {
            o->state = 0;
            o->animFrame = 1 - o->animFrame;
        }
        break;
    case 5:
        if (D_800A6038.active != 1 || *(unsigned char *)&D_800A6038.wac >= 2) {
            o->state = 1;
        } else {
            o->step = 3;
            o->state = 0;
        }
        break;
    case 6:
        AnimAdvance(o);
        if (--o->timer == -1) {
            o->state = 1;
            o->timer = 0x10;
            o->animFrame = 1 - o->animFrame;
        }
        break;
    }
}
