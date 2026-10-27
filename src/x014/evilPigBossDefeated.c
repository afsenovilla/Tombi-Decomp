// FUNC 8011ee50 968 X014
// MATCHING 8011ee50 968
#include "camera.h"
#include "gvars.h"
/* The `if (r)` block is written in both arms of case 1 (cross-jumped later): a2/a3 set twice lose sched1's
   birthing-insn boost, so the arg loads come before the state lbu. */
#include "TOBJ.H"
extern int CAMERA_TARGET_H_FIX, CAMERA_TARGET_Y_FIX;
extern unsigned char D_800A4553[];
extern char D_80077D6C[], MOVE_TABLE_2[];
extern void AnimLoadDuration(TObj *);
extern void applyFrameVelocityX(TObj *);
extern int evilPigBossCheckLanded(TObj *);
extern int evilPigBossDefeatedCheckWall(TObj *);
extern void evilPigBossCheckWall(TObj *);
extern void spawnImpactEffect(int, short, short, short);
extern void playSFX(int);
extern void applyAnimVelocityX(TObj *, unsigned short);
extern void spawnEvilPigHitStars(TObj *);

#define setbox(o, q) { unsigned char *p_; p_ = (unsigned char *)o->d90; p_ += (q); (o)->box0 = *p_++; (o)->box1 = *p_++; (o)->box2 = *p_; (o)->box3 = p_[1]; }

typedef struct { char pad[0xa8]; void **tbl; } TB;

void evilPigBossDefeated(TObj *o)
{
    unsigned char *p;
    int r;

    switch (o->state) {
    case 0:
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        ((unsigned char*)&GVARS.b3e)[0] = 1;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        D_800A4553[0] = 3;
        CAMERA_SCROLL.velY = 0;
        CAMERA_TARGET_Y_FIX = o->y.raw;
        CAMERA_TARGET_H_FIX = o->h->raw;
        o->b9c = 0;
        o->b6a = 1;
        o->state++;
        setbox(o, 0x3c);
        o->wac = 0xf;
        o->anim = ((TB *)o)->tbl[15];
        AnimLoadDuration(o);
        o->movetab = D_80077D6C;
        break;
    case 1:
        if (o->animFrame & 2) {
            o->y.raw += 0x48000;
            r = evilPigBossCheckLanded(o);
            if (r) {
                o->b6a = 0;
                o->state++;
                spawnImpactEffect(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
                playSFX(6);
            }
        } else {
            applyFrameVelocityX(o);
            r = evilPigBossDefeatedCheckWall(o);
            if (r) {
                o->b6a = 0;
                o->state++;
                spawnImpactEffect(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
                playSFX(6);
            }
        }
        if (o->animFrame & 1) o->d8c = (unsigned char)(o->d8c + 0x14);
        else o->d8c = (unsigned char)(o->d8c - 0x14);
        CAMERA_TARGET_Y_FIX = o->y.raw;
        CAMERA_TARGET_H_FIX = o->h->raw;
        break;
    case 2:
        if (o->animFrame & 2) o->w7a = o->animFrame & 1;
        else o->w7a = ~o->animFrame & 1;
        playSFX(7);
        o->velV = -0x400;
        o->b9c = 1;
        o->b6a = 0;
        o->movetab = MOVE_TABLE_2;
        o->state++;
    case 3:
        if (o->w7a) o->d8c = (unsigned char)(o->d8c + 0x14);
        else o->d8c = (unsigned char)(o->d8c - 0x14);
        applyAnimVelocityX(o, o->w7a);
        evilPigBossCheckWall(o);
        o->velV += 0x40;
        if (o->velV > 0x400) o->velV = 0x400;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b9c = 2;
            if (evilPigBossCheckLanded(o)) {
                o->b9c = 0;
                o->d8c = 0;
                o->state++;
                o->animFrame &= 1;
            }
        }
        CAMERA_TARGET_Y_FIX = o->y.raw;
        CAMERA_TARGET_H_FIX = o->h->raw;
        break;
    case 4:
        setbox(o, 0x34);
        o->wac = 0xd;
        o->anim = ((TB *)o)->tbl[13];
        AnimLoadDuration(o);
        o->timer = 0x3c;
        o->state++;
        CAMERA_TARGET_H_FIX = o->h->raw;
        CAMERA_TARGET_Y_FIX = o->y.raw;
        D_800A4553[0] = 6;
        spawnEvilPigHitStars(o);
        break;
    case 5:
        if (--o->timer == -1) {
            o->d8c = 0;
            o->step = 2;
            o->state = 0;
            o->substep = 0;
        }
        break;
    }
}
