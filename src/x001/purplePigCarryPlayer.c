// FUNC 8012a290 828 X001
// MATCHING 8012a290 828
#include "TOBJ.H"

extern TObj PLAYER;
extern unsigned short D_800A6066;
extern short D_800A60B6;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern void *D_8013FC88[], *D_8013FC8C[], *D_8013FCCC[];
extern unsigned char D_8013C7C8[];
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern int Rand(void);
extern void applyFrameVelocityX(TObj *);
extern int probeCollisionAtDepthA(TObj *, int, int);
extern void decreaseObjectTimer(TObj *, int);

void purplePigCarryPlayer(TObj *o)
{
    switch (o->state) {
    case 0:
        o->b9c = 1;
        o->state++;
    case 1:
        if (AnimAdvance(o)) {
            o->velV = -0x400;
            o->wac = 6;
            o->state++;
            o->anim = D_8013FC88[0];
            AnimLoadDuration(o);
        }
        PLAYER.b69 = 1;
        PLAYER.h->p.whole = o->h->p.whole;
        {
            short t = PLAYER.y.p.whole - 0x18;
            o->velY = o->y.p.whole - t;
        }
        if (o->velY != 0) {
            if (o->velY < 0) PLAYER.y.p.whole--;
            else PLAYER.y.p.whole++;
        }
        break;
    case 2:
        AnimAdvance(o);
        o->velV += 0x10;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) o->state++;
        else if ((short)probeCollisionAtDepthA(o, o->h->p.whole, (short)(o->y.p.whole - 0x10))) o->velV = 0;
        PLAYER.b69 = 1;
        PLAYER.h->p.whole = o->h->p.whole;
        PLAYER.y.p.whole = o->y.p.whole + (PLAYER.box3 - PLAYER.box2);
        break;
    case 3: {
        TObj *p = &PLAYER;
        *(signed char *)&o->b0f = -9;
        o->b6a = 0;
        D_800A6066 = 2;
        D_800A60B6 = 0;
        p->active = 2;
        D_800A603C = 2;
        D_800A603D = 0;
        D_800A603E = 1;
        decreaseObjectTimer(p, 1);
        D_800A60B6 = 0x200;
        o->wac = 7;
        o->state++;
        o->anim = D_8013FC8C[0];
        AnimLoadDuration(o);
        break;
    }
    case 4:
        if (AnimAdvance(o)) {
            o->active = 1;
            o->state++;
        }
        break;
    case 5:
        o->state++;
        if (D_8013C7C8[Rand() & 0xf]) o->timer = 0x50;
        else o->timer = 0x32;
        o->wac = 0x17;
        o->anim = D_8013FCCC[0];
        AnimLoadDuration(o);
        break;
    case 6:
        applyFrameVelocityX(o);
        AnimAdvance(o);
        if (--o->timer == -1) {
            o->timer = 0x80;
            o->step = 7;
            o->state = 0;
        }
        break;
    }
}
