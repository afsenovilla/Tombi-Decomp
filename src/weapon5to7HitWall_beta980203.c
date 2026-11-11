// VERSION BETA_980203 beta980203 80035E14 312
#if defined(VERSION_BETA_980203)
// Beta version: PAL weapon5to7HitWall.c without the wall-debris spawn.
#include "TOBJ.H"
extern short MulCos(int, int);
extern short MulNegSinScaled(int, int);
extern void ObjApplyVelocity(TObj *);
extern short probeCollisionAtDepthB(TObj *, int, int);
extern void spawnImpactEffect(int, int, int, int);
extern void spawnWeaponHitDebris(int, int, int, int);
extern void SfxPlay(int);
extern void playerSelectGroundAnim(TObj *);
extern unsigned short COLLISION_HIT_FLAGS;
extern int DAT_800a60c4[];
extern unsigned char DAT_800a60d5[];
extern unsigned char *PLAYER_EXT;
extern short DAT_8007a074[];

static __inline__ void f(TObj *q)
{
    q->waa = DAT_8007a074[(short)(q->animFrame + q->animFrame * 2)];
    DAT_800a60d5[0] = 0;
    *PLAYER_EXT = 0;
    playerSelectGroundAnim(q);
}

int weapon5to7HitWall(TObj *o)
{
    int r;
    unsigned char *p;
    short t;
    unsigned short u;
    int m;
    r = 0;
    o->d84 = 0x200;
    t = o->wa8 - 0x200;
    o->d88 = o->d88 + 0x100 & 0xfff;
    o->wa8 = t;
    o->velH = MulCos((unsigned char)o->waa, t);
    o->velV = MulNegSinScaled((unsigned char)o->waa, o->wa8);
    ObjApplyVelocity(o);
    if (o->active == 1 && probeCollisionAtDepthB(o, o->h->p.whole, o->y.p.whole) != 0) {
        spawnImpactEffect(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        SfxPlay(5);
        o->wa8 = 0x4ff;
    }
    DAT_800a60c4[0] = 0;
    if (o->wa8 < 0x500) {
        f(o);
        r = 1;
    }
    return r;
}
#endif
