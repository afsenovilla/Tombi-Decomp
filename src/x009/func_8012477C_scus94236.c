// VERSION SCUS_94236 x09 8012477C 1536
// FLAGS.SCUS_94236 -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SCUS_94236)
// USA version: JP C of src/x009/func_801246FC_slps01144.c (func_801246FC_slps01144) (1.000 masked) [-O2 -G0 -fno-strength-reduce] (tools/demo_sweep.py)
#include "spad.h"
#include "TOBJ.H"

extern void *D_8012E230[], *D_8012E240[], *D_8012E22C[];
extern char D_8007722C[], D_80077220[];
extern short D_8007DB88[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void applyAnimVelocityX(TObj *, unsigned short);
extern void playSFX(int);
extern short probeWallAt(TObj *, short, short, short);
extern short TileCollideAt(TObj *, short, short);

static __inline__ int wallchk(TObj *o)
{
    unsigned short f = o->animFrame;
    short g, d;
    if ((o->b9d & 2) && f == (o->b9d & 1))
        return 1;
    g = f;
    if (g)
        d = -0x10;
    else
        d = 0x10;
    return probeWallAt(o, o->h->p.whole + d, o->y.p.whole + 0x30, g);
}

#define PAR(o) (*(TObj **)&(o)->wa8)

void func_8012477C_scus94236(TObj *o)
{
    unsigned char t;
    unsigned short af;
    int r;
    short f, d;
    TObj *p;

    switch (o->state) {
    case 0:
        o->movetab = D_8007722C;
        o->velV = 0x180;
        o->velY = -0x18;
        o->wac = 0;
        o->state++;
        o->anim = D_8012E230[0];
        AnimLoadDuration(o);
        o->d30 = PAR(o)->h->p.whole;
        o->d34 = PAR(o)->y.p.whole;
        break;
    case 1:
        o->state++;
        o->timer = 0;
        if ((short)((*(unsigned short*)((char*)&SPAD.i168 + 2)) - o->h->p.whole) >= 0)
            o->animFrame = 1;
        else
            o->animFrame = 0;
    case 2:
        AnimAdvance(o);
        if ((unsigned short)(o->h->p.whole - (*(unsigned short*)((char*)&SPAD.i174 + 2)) - 0x30) < 0x110) {
            applyAnimVelocityX(o, o->animFrame);
            {
                int q;
                q = wallchk(o);
                if (q) o->animFrame = 1 - o->animFrame;
            }
            o->y.raw += o->velV << 8;
            if (o->velV >= -0x17f) o->velV += o->velY;
            TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x30);
        } else {
            o->wac = 4;
            o->state++;
            o->anim = D_8012E240[0];
            AnimLoadDuration(o);
            o->movetab = D_80077220;
        }
        break;
    case 3:
        applyAnimVelocityX(o, o->animFrame);
        o->y.raw += o->velV << 8;
        r = wallchk(o);
        TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x30);
        if (AnimAdvance(o)) o->state++;
        break;
    case 4:
        o->state++;
        o->wac = 0;
        o->anim = D_8012E22C[0];
        AnimLoadDuration(o);
        o->velV = 0x100;
        o->d88 = 0;
        o->animFrame = 1 - o->animFrame;
        playSFX(0x97);
        break;
    case 5:
        AnimAdvance(o);
        o->y.raw += (D_8007DB88[o->d88] * o->velV) >> 4;
        TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x30);
        o->d88 = (o->d88 + 2) & 0xff;
        break;
    }
    {
        TObj *q = PAR(o);
        if (*(unsigned short *)&q->b04 == 0x102) {
            r = -1;
        } else if (*(unsigned short *)&q->b04 == 0x202) {
            if ((short)(q->h->p.whole - o->h->p.whole) < 0) {
                if (q->animFrame & 1) {
                m2:
                    r = -2;
                } else {
                    r = 1;
                }
            } else {
                if (!(q->animFrame & 1)) goto m2;
                r = 1;
            }
        } else {
            r = 0;
        }
    }
    switch (r) {
    case -2:
        o->step = 0;
        o->state = 0;
        break;
    case -1:
        {
            short dy, dz, dx, ex;
            p = PAR(o);
            dz = p->y.p.whole - o->d34;
            dy = o->y.p.whole - p->y.p.whole;
            dx = p->h->p.whole - o->d30;
            ex = o->h->p.whole - p->h->p.whole;
            if (ex > 0 && dx > 0)
                o->h->p.whole += dx >> 1;
            else if (ex < 0 && dx < 0)
                o->h->p.whole += dx >> 1;
            if (dy >= -0x4f)
                o->y.p.whole -= dz >> 1;
            else
                o->y.p.whole += dz >> 1;
            r = wallchk(o);
            TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x30);
            o->d30 = PAR(o)->h->p.whole;
            o->d34 = PAR(o)->y.p.whole;
        }
        break;
    case 0:
        o->step = 0;
        o->state = 0;
        break;
    case 1:
        o->step = 4;
        o->state = 0;
        break;
    }
}
#endif
