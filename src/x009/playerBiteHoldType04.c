// FUNC 801040d0 804 X009
// MATCHING 801040d0 804
#include "gv6.h"
#include "TOBJ.H"
#define B(o, n) (((unsigned char *)(o))[n])
extern TObj *BITTEN_OBJECT;
extern unsigned short DAT_1f8003c6;
extern unsigned short DAT_1f8001fc;
extern void playerAirAccelerateX(TObj *);
extern void ObjAddVelY7E(TObj *);
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void spawnImpactEffect(int, int, int, int);

#define LAND(o)                                                         \
    do {                                                                \
        o->timer = 0;                                                   \
        PlayerSetAnimIfChanged(o, 0xd);                                 \
        spawnImpactEffect(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);      \
        B(o, 0xac) = 3;                                                 \
        o->b9c = 0;                                                     \
        o->velX = 0;                                                    \
        o->velY = 0;                                                    \
        BITTEN_OBJECT->h->p.whole = o->h->p.whole;                         \
        BITTEN_OBJECT->y.p.whole = o->y.p.whole + BITTEN_OBJECT->box2;        \
        o->state = 2;                                                   \
    } while (0)

void playerBiteHoldType04(TObj *o)
{
    short hit;

    switch (o->state) {
    case 1:
        o->timer++;
        playerAirAccelerateX(o);
        o->h->raw += o->velX << 8;
        o->velY += 8;
        if (o->velY > 0x680)
            o->velY = 0x680;
        if (o->velY < -0x680)
            o->velY = -0x680;
        ObjAddVelY7E(o);
        hit = 0;
        o->y.p.whole += 8;
        if (o->y.p.whole + BITTEN_OBJECT->box2 >= BITTEN_OBJECT->y.p.whole) {
            o->y.p.whole = BITTEN_OBJECT->y.p.whole - BITTEN_OBJECT->box2;
            hit = 1;
        }
        if (o->velX < 0) {
            o->h->p.whole -= 8;
            if (BITTEN_OBJECT->h->p.whole >= o->h->p.whole) {
                o->h->p.whole = BITTEN_OBJECT->h->p.whole;
                if (hit)
                    LAND(o);
            }
        } else {
            o->h->p.whole += 8;
            if (o->h->p.whole >= BITTEN_OBJECT->h->p.whole) {
                o->h->p.whole = BITTEN_OBJECT->h->p.whole;
                if (hit)
                    LAND(o);
            }
        }
        if (o->timer >= 0xb)
            LAND(o);
        break;
    case 2:
        if (((unsigned short*)&GV6.s70)[0] & 0x80)
            o->animFrame = 1;
        if (((unsigned short*)&GV6.s70)[0] & 0x20)
            o->animFrame = 0;
        BITTEN_OBJECT->h->p.whole = o->h->p.whole;
        BITTEN_OBJECT->y.p.whole = o->y.p.whole + BITTEN_OBJECT->box2;
        if (DAT_1f8001fc & DAT_1f8003c6) {
            o->b9c = 1;
            o->b69 = 0;
            o->wb2 = 0;
            BITTEN_OBJECT->state = 4;
            o->step = 0xf;
            o->state = 0;
        }
        break;
    }
}
