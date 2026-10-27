// FUNC 80102ac0 1012 X005
// MATCHING 80102ac0 1012
#include "gv6.h"
#include "TOBJ.H"
extern TObj *BITTEN_OBJECT;
extern unsigned short D_1F8003C6, D_1F8001FC;
extern unsigned char GAME_PLAYER_MODE;
extern void ObjMotionStep(TObj *);
extern void spawnImpactEffect(int, int, int, int);
extern void PlayerSetAnimIfChanged(TObj *, int);
extern short ObjTileCollide(TObj *, int, int);

void playerBiteHoldType02(TObj *o)
{
    short f;
    volatile unsigned short *k;


    switch (o->state) {
    case 1:
        o->timer++;
        ObjMotionStep(o);
        BITTEN_OBJECT->animFrame = o->animFrame & 1;
        f = 0;
        o->y.p.whole += 8;
        if (BITTEN_OBJECT->y.p.whole < o->y.p.whole + 8) {
            o->y.p.whole = BITTEN_OBJECT->y.p.whole - 8;
            f = 1;
        }
        if (o->velX < 0) {
            o->h->p.whole -= 8;
            if (o->h->p.whole <= BITTEN_OBJECT->h->p.whole) {
                o->h->p.whole = BITTEN_OBJECT->h->p.whole;
                if (f) {
                    o->timer = 0;
                    PlayerSetAnimIfChanged(o, 0xd);
                    spawnImpactEffect(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
                    if (BITTEN_OBJECT->type == 2) BITTEN_OBJECT->state = 2;
                }
            }
        } else {
            o->h->p.whole += 8;
            if (o->h->p.whole >= BITTEN_OBJECT->h->p.whole) {
                o->h->p.whole = BITTEN_OBJECT->h->p.whole;
                if (f) {
                    o->timer = 0;
                    PlayerSetAnimIfChanged(o, 0xd);
                    spawnImpactEffect(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
                    if (BITTEN_OBJECT->type == 2) BITTEN_OBJECT->state = 2;
                }
            }
        }
        if (BITTEN_OBJECT->type == 0x21) {
            BITTEN_OBJECT->h->p.whole = o->h->p.whole;
            BITTEN_OBJECT->y.p.whole = o->y.p.whole + 8;
            PlayerSetAnimIfChanged(o, 0xd);
            spawnImpactEffect(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            o->state = 2;
        } else if (o->b69 != 0 || ObjTileCollide(o, 0, 1)) {
            BITTEN_OBJECT->h->p.whole = o->h->p.whole;
            BITTEN_OBJECT->y.p.whole = o->y.p.whole + 8;
            PlayerSetAnimIfChanged(o, 0xd);
            spawnImpactEffect(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            if (BITTEN_OBJECT->type == 2) BITTEN_OBJECT->state = 2;
            o->state = 2;
        } else if (o->timer >= 11) {
            BITTEN_OBJECT->h->p.whole = o->h->p.whole;
            BITTEN_OBJECT->y.p.whole = o->y.p.whole + 8;
            o->timer = 0;
            PlayerSetAnimIfChanged(o, 0xd);
            spawnImpactEffect(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            if (BITTEN_OBJECT->type == 2) BITTEN_OBJECT->state = 2;
            o->state = 2;
        }
        break;
    case 2:
        PlayerSetAnimIfChanged(o, 0xd);
        o->b9c = 0;
        o->velX = 0;
        o->velY = 0;
        o->state++;
    case 3:
        k = &GV6.s70;
        if (*k & 0x80) o->animFrame = 1;
        if (*k & 0x20) o->animFrame = 0;
        BITTEN_OBJECT->animFrame = o->animFrame & 1;
        o->h->p.whole = BITTEN_OBJECT->h->p.whole;
        o->y.p.whole = BITTEN_OBJECT->y.p.whole - BITTEN_OBJECT->box2;
        o->d8c = BITTEN_OBJECT->d8c;
        if (D_1F8001FC & D_1F8003C6) {
            GAME_PLAYER_MODE = 0;
            o->b9c = 1;
            o->b69 = 0;
            *(unsigned char *)&o->wac = 3;
            o->wb2 = 0;
            if (BITTEN_OBJECT->type == 2) {
                BITTEN_OBJECT->active = 2;
                BITTEN_OBJECT->state = 6;
            }
            o->step = 0xf;
            o->state = 0;
        }
        break;
    }
}
