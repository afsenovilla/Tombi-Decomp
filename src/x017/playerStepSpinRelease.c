// FUNC 800f67bc 1144 X017
// MATCHING 800f67bc 1144
#include "gvars.h"
#include "TOBJ.H"
#include "raw7.h"
extern TObj *PLAYER_EXT;
extern TObj *GRABBED_OBJECT;
extern short PLAYER_SPIN_RELEASE_SPEEDS[];
extern void ObjSetAnimFromTable(TObj *);
extern void AnimJump(TObj *, int);
extern void AnimAdvance(TObj *);
extern void SfxPlay2(int, int);
extern void playerSpinReleaseSpeedKind(TObj *);
extern void applyObjectAltSpeedXY(TObj *);
extern int ObjCheckHeadCollision(TObj *);
extern TObj *playerProbeGrab(TObj *, int);
extern void playerEnterGrabStep(TObj *, int);
extern void playerTryAttack(TObj *, int);

static __inline__ void setanim(TObj *o, unsigned short anim, int j)
{
    TObj *p = PLAYER_EXT;
    p->animTimer = anim;
    if (p->animFrame != anim) {
        p->animTimer = anim;
        ObjSetAnimFromTable(o);
        AnimJump(o, j);
        PLAYER_EXT->animFrame = PLAYER_EXT->animTimer;
    }
}

static __inline__ void land(TObj *o)
{
    int v, w;
    setanim(o, 4, 1);
    (*(unsigned char*)&o->wac) = 1;
    o->b9c = 2;
    o->timer = 10;
    (*(unsigned char*)&PLAYER_EXT->w08) = 1;
    v = 0x10;
    o->velY = 0;
    o->velV = 0;
    o->d84 = 0;
    if (o->animFrame & 1) v = 0xf0;
    w = 0xe8;
    o->d88 = v;
    if (o->animFrame & 1) w = 0x18;
    o->d8c = w;
    o->step = 2;
    o->state = 3;
}

void playerStepSpinRelease(TObj *o)
{
    int t;
    TObj *p;
    short k;
    short v;
    switch (o->state) {
    case 0:
        o->b9e = 0;
        o->b69 = 0;
        o->b9c = 1;
        (*(unsigned char*)&o->wac) = 0;
        p = PLAYER_EXT;
        t = 1;
        o->d8c = 0;
        o->wb0 = 0;
        if (o->wb2 > 8) t = 2;
        p->step = t;
        k = PLAYER_EXT->b0b * 7;
        if (o->wb2 < 4) {
            if (o->animFrame & 1) {
                o->velX = -PLAYER_SPIN_RELEASE_SPEEDS[k + 4] / 3;
            } else {
                o->velX = PLAYER_SPIN_RELEASE_SPEEDS[k + 4] / 3;
            }
            o->velY = PLAYER_SPIN_RELEASE_SPEEDS[k + 10] >> 1;
        } else {
            v = PLAYER_SPIN_RELEASE_SPEEDS[k + o->wb2];
            if (o->animFrame & 1) v = -v;
            o->velX = v;
            o->velY = PLAYER_SPIN_RELEASE_SPEEDS[k + 10];
        }
        PLAYER_EXT->timer = 0xe;
        o->wb2 = 0;
        o->wb6 = 0;
        setanim(o, 8, 0);
        SfxPlay2(2, 4);
        o->state++;
    case 1:
        o->h->raw += GVARS.s44 << 8;
        o->y.raw += ((short*)&GVARS.s46)[0] << 8;
        playerSpinReleaseSpeedKind(o);
        applyObjectAltSpeedXY(o);
        AnimAdvance(o);
        if (ObjCheckHeadCollision(o)) {
            land(o);
        }
        if (o->velY > 0) {
            land(o);
        }
        break;
    }
    GRABBED_OBJECT = playerProbeGrab(o, 0);
    if (o->b9e != 0) {
        (*(unsigned char*)&PLAYER_EXT->w08) = 0;
        PLAYER_EXT->timer = 0;
        (*(unsigned char*)&o->wac) = 0;
        o->b9c = 0;
        o->velX = 0;
        o->velY = 0;
        o->wb2 = 0;
        playerEnterGrabStep(o, GRABBED_OBJECT == (TObj *)1);
    }
    if ((*(unsigned char*)&o->wac) != 2 && o->step == 9 && o->b9e == 0) {
        playerTryAttack(o, 1);
    }
}
