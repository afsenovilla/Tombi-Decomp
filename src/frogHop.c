// FUNC 80132f80 724 X000
// MATCHING 80132f80 724
#include "player.h"
#include "TOBJ.H"
extern void *D_8013B18C, *D_8013B180, *FROG_ANIMS;
extern char MOVE_TABLE_1[];
extern int Rand(void);
extern void AnimJump(TObj *, int);
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);
extern short probeWallAt(TObj *, short, short, int);
extern void applyFrameVelocityX(TObj *);
extern short TileCollideAt(TObj *, short, short);
void frogHop(TObj *o)
{
    unsigned short px, hx;
    short near;
    Fix16 *pp = (*(Fix16 **)&PLAYER.obj.h);
    int x, t;
    int s2;
    px = pp->p.whole;
    hx = o->h->p.whole;
    near = (unsigned short)(px - hx + 0x40) < 0x80;
    switch (o->state) {
    case 0:
        o->animFrame = (short)hx < (short)px;
        if (Rand() & 1) {
            o->anim = D_8013B18C;
            AnimJump(o, 1);
            o->movetab = MOVE_TABLE_1;
            o->velY = -0x200;
            o->state = 2;
        } else {
            o->anim = D_8013B180;
            AnimLoadDuration(o);
            o->timer = 0x40 >> (near * 5);
            o->state = 1;
        }
        break;
    case 1:
        AnimAdvance(o);
        t = near;
        if (--o->timer == 0) {
            o->state = 0;
        } else if (t) {
            o->state = 0;
        }
        break;
    case 2:
        AnimAdvance(o);
        t = o->h->p.whole;
        s2 = (o->animFrame & 1) ? t - 8 : t + 8;
        if (probeWallAt(o, s2, o->y.p.whole, 0) == 0) {
            applyFrameVelocityX(o);
        }
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (o->velY > 0) {
            o->anim = D_8013B18C;
            AnimJump(o, 4);
            o->state = 3;
        }
        goto common;
    case 3:
        t = (short)hx;
        if (o->animFrame & 1) x = t - 8;
        else x = t + 8;
        if (probeWallAt(o, x, o->y.p.whole, 0) == 0) {
            applyFrameVelocityX(o);
        }
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 8) != 0) {
            o->anim = FROG_ANIMS;
            AnimLoadDuration(o);
            o->state = 0;
        }
    common:
        if (o->b68 == 0 && o->h->p.whole >= 0x49f) {
            o->b04 = 3;
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
