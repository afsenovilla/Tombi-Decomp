// FUNC 8011f4e0 880 X014
// MATCHING 8011f4e0 880
#include "gvars.h"
#include "TOBJ.H"
extern void FUN_8001f8e4(TObj *);
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);
extern int FUN_800202b4(TObj *);
extern void freeObjectLayer2(TObj *);
extern void spawnEvilPigHitStars(TObj *);
extern void initEvilPigBoss(TObj *);
extern void evilPigBossIntroTalk(TObj *);
extern void evilPigBossTeleport(TObj *);
extern void evilPigBossAttack(TObj *);
extern void evilPigBossWait(TObj *);
extern void evilPigBossHit(TObj *);
extern void evilPigBossStunned(TObj *);
extern void evilPigBossDefeated(TObj *);

#define WC2(o) (*(unsigned short *)((char *)(o) + 0xc2))

static __inline__ void setanim(TObj *o, short k)
{
    unsigned char *p;
    p = (unsigned char *)o->d90;
    p += k * 4;
    o->box0 = *p++;
    o->box1 = *p++;
    o->box2 = *p;
    o->box3 = p[1];
    o->wac = k;
    o->anim = ((void **)*(int *)&o->wa8)[k];
    FUN_8001fe6c(o);
}

void updateEvilPigBoss(TObj *o)
{
    unsigned char *p;

    switch (o->b04) {
    case 0:
        switch (o->step) {
        case 0:
            FUN_8001f8e4(o);
            initEvilPigBoss(o);
            if (o->subtype == 0) {
                setanim(o, 0);
            } else {
                setanim(o, 2);
            }
            (*(TObj **)&GVARS.p48) = o;
            o->step++;
            break;
        case 1:
            if (GVARS.b3a) evilPigBossIntroTalk(o);
            break;
        }
        break;
    case 1:
        FUN_800202b4(o);
        switch (o->step) {
        case 0:
            evilPigBossTeleport(o);
            break;
        case 1:
            evilPigBossAttack(o);
            break;
        case 2:
            evilPigBossWait(o);
            break;
        }
        o->wb4++;
        break;
    case 2:
        if (WC2(o) == 1) {
            GVARS.b3e = 0;
            GVARS.b42 = 0;
            WC2(o) = 0;
        }
        FUN_800202b4(o);
        switch (o->step) {
        case 0:
            evilPigBossHit(o);
            break;
        case 1:
            evilPigBossStunned(o);
            break;
        case 2:
            switch (o->state) {
            case 0:
                spawnEvilPigHitStars(o);
                o->d8c = 0;
                o->state++;
                setanim(o, 0xe);
                break;
            case 1:
                if (FUN_8001fec0(o)) {
                    o->b68 = 0;
                    o->active = 1;
                    o->b04 = 1;
                    o->step = 0;
                    o->state = 0;
                    o->substep = 0;
                }
                break;
            }
            break;
        case 3:
            evilPigBossDefeated(o);
            break;
        case 4:
            if (o->state == 1) o->visible = 0;
            break;
        }
        o->wb4++;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
