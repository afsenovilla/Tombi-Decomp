// VERSION SCUS_94236 x09 80125504 676
// FLAGS.SCUS_94236 -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SCUS_94236)
// USA version: JP C of src/x009/func_80125484_slps01144.c (func_80125484_slps01144) (1.000 masked) [-O2 -G0 -fno-strength-reduce] (tools/demo_sweep.py)
#include "TOBJ.H"

typedef struct { void *p; } PP;
extern PP D_8012E230;
extern PP D_8012E22C;
extern char D_80077220[];
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern void applyAnimVelocityX(TObj *, int);
extern int TileCollideAt(TObj *, int, int);
extern short probeWallAt(TObj *, short, short, short);

static __inline__ void chk(TObj *o, unsigned short f)
{
    short d;
    short g = f;

    if ((o->b9d & 2) && f == (o->b9d & 1))
        return;
    if (g)
        d = -16;
    else
        d = 16;
    probeWallAt(o, o->h->p.whole + d, o->y.p.whole + 0x30, g);
}

void func_80125504_scus94236(TObj *o)
{
    switch (o->state) {
    case 0:
        o->state++;
        o->wac = 0;
        o->anim = D_8012E230.p;
        AnimLoadDuration(o);
        if ((*(TObj **)&o->wa8)->y.p.whole > o->y.p.whole)
            o->w74 = 0;
        else
            o->w74 = 1;
        o->velV = 0x200;
        o->velY = -8;
        break;
    case 1:
        AnimAdvance(o);
        if (o->w74 == 0)
            o->y.raw -= o->velV << 8;
        else
            o->y.raw += o->velV << 8;
        TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x30));
        o->velV += o->velY;
        if (o->velV <= 0) {
            o->timer = 24;
            o->wac = 0;
            o->anim = D_8012E22C.p;
            AnimLoadDuration(o);
            o->movetab = D_80077220;
            o->state++;
        }
        break;
    case 2:
        AnimAdvance(o);
        applyAnimVelocityX(o, o->animFrame);
        if (o->w74 == 0)
            o->y.raw -= o->velV << 8;
        else
            o->y.raw += o->velV << 8;
        if (o->velV >= -0x200)
            o->velV += o->velY;
        chk(o, o->animFrame);
        TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x30));
        if (--o->timer == -1)
            o->state++;
        break;
    case 3:
        o->step = 0;
        o->state = 0;
        break;
    }
}
#endif
