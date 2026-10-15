// FUNC 8011e6b0 1080 X003
// MATCHING 8011e6b0 1080
#include "gvars.h"
#include "TOBJ.H"

extern TObj *D_8009C330;
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void SfxPlay2(int, int);
extern void playSFX(int);
extern void playerReleaseBittenAirborne(TObj *);
extern void spawnImpactEffect(int, int, int, int);
extern int AnimAdvance(TObj *);
extern void playerSetFallLean(TObj *);
extern int ObjCheckHeadCollision(TObj *);
extern void objCollideSideWalls(TObj *);
extern short ObjTileCollide(TObj *, int, int);
extern void playerHurtLandRecover(TObj *);

static __inline__ void land(TObj *o, short n)
{
    *(unsigned char *)&D_8009C330->w08 = 0;
    o->b9c = n;
    *(unsigned char *)&o->wac = 1;
    o->b04 = 1;
    playerSetFallLean(o);
    o->step = n;
    o->state = 3;
    o->substep = 0;
}

void playerStepLaunched(TObj *o)
{
    short v;

    switch (o->state) {
    case 0:
        v = 0x200;
        if (o->animFrame & 1) {
            v = -0x200;
        }
        o->b9c = 1;
        o->velX = v;
        o->velY = 0;
        o->timer = 0x14;
        o->state++;
    case 1:
        o->visible = 1;
        PlayerSetAnimIfChanged(o, 0x39);
        SfxPlay2(0x23, 0x24);
        playSFX(0x1f);
        playerReleaseBittenAirborne(o);
        o->animFrame ^= 1;
        *(unsigned char *)&D_8009C330->w08 = ((unsigned char*)&GVARS.b38)[0];
        spawnImpactEffect(3, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        break;
    case 2:
        o->h->raw += ((short*)&GVARS.s44)[0] << 8;
        o->y.raw += ((short*)&GVARS.s46)[0] << 8;
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        if (AnimAdvance(o)) {
            PlayerSetAnimIfChanged(o, 0x10);
            o->state = 3;
        }
        break;
    case 3:
        o->h->raw += ((short*)&GVARS.s44)[0] << 8;
        o->y.raw += ((short*)&GVARS.s46)[0] << 8;
        {
            int d = o->d8c;
            o->d8c = ((o->animFrame & 1) ? d - 0x10 : d + 0x10) & 0xff;
        }
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        if (--o->timer <= 0) {
            o->state++;
        }
        if (*(unsigned char *)&D_8009C330->w08 == 0) {
            if (o->velY > 0) {
                land(o, 2);
            }
            if (ObjCheckHeadCollision(o)) {
                land(o, 2);
            }
        }
        break;
    case 4:
        o->h->raw += ((short*)&GVARS.s44)[0] << 8;
        o->y.raw += ((short*)&GVARS.s46)[0] << 8;
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        if (o->velY > 0x680) {
            o->velY = 0x680;
        }
        objCollideSideWalls(o);
        if (o->b69 || ObjTileCollide(o, 0, 0)) {
            o->b9c = 0;
            o->substep = 0;
            o->animFrame ^= 1;
            o->state++;
        }
        if (*(unsigned char *)&D_8009C330->w08 == 0) {
            if (ObjCheckHeadCollision(o)) {
                land(o, 2);
            }
            if (o->velY > 0) {
                land(o, 2);
            }
        }
        break;
    case 5:
        playerHurtLandRecover(o);
        break;
    }
}
