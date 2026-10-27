// FUNC 80102ac0 1012 X000
// MATCHING 80102ac0 1012
// VERSION SLPM_80165 x00 800FB38C 1284
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x000 80102AC0 (func_80102AC0.c): d-pad bits 0x1000-0x8000 (PAL 0x10-0x80). player animation change inline (PAL calls PlayerSetAnimIfChanged). Also moves by velX and runs two more step helpers; fixed pad bit 0x40.
#include "gv6.h"
#include "TOBJ.H"
typedef struct { char p_[0x2c]; unsigned short animTimer, animFrame; } AnimP_;
extern AnimP_ *D_8009C330_anim;
extern void func_800EDA04(void *);
extern void func_80020554(void *);
#define PlayerSetAnimIfChanged(o, n) {                                                   \
    AnimP_ *p_ = D_8009C330_anim;                                     \
    p_->animTimer = (n);                                              \
    if (p_->animFrame != (n)) {                                       \
        func_800EDA04(o);                                             \
        func_80020554(o);                                             \
        D_8009C330_anim->animFrame = D_8009C330_anim->animTimer;      \
    }                                                                 \
}
extern TObj *BITTEN_OBJECT;
extern unsigned short D_1F8003C6, D_1F8001FC;
extern unsigned char GAME_PLAYER_MODE;
extern void ObjMotionStep(TObj *);
extern void FUN_8001f96c(int, int, int, int);
extern short ObjTileCollide(TObj *, int, int);

extern void func_8010A91C(TObj *), func_8002047C(TObj *);
void func_80102AC0(TObj *o)
{
    short f;
    volatile unsigned short *k;


    switch (o->state) {
    case 1:
        o->timer++;
        ObjMotionStep(o);
        o->h->raw += o->velX << 8;
        func_8010A91C(o);
        func_8002047C(o);
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
                    FUN_8001f96c(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
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
                    FUN_8001f96c(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
                    if (BITTEN_OBJECT->type == 2) BITTEN_OBJECT->state = 2;
                }
            }
        }
        if (BITTEN_OBJECT->type == 0x21) {
            BITTEN_OBJECT->h->p.whole = o->h->p.whole;
            BITTEN_OBJECT->y.p.whole = o->y.p.whole + 8;
            PlayerSetAnimIfChanged(o, 0xd);
            FUN_8001f96c(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            o->state = 2;
        } else if (o->b69 != 0 || ObjTileCollide(o, 0, 1)) {
            BITTEN_OBJECT->h->p.whole = o->h->p.whole;
            BITTEN_OBJECT->y.p.whole = o->y.p.whole + 8;
            PlayerSetAnimIfChanged(o, 0xd);
            FUN_8001f96c(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            if (BITTEN_OBJECT->type == 2) BITTEN_OBJECT->state = 2;
            o->state = 2;
        } else if (o->timer >= 11) {
            BITTEN_OBJECT->h->p.whole = o->h->p.whole;
            BITTEN_OBJECT->y.p.whole = o->y.p.whole + 8;
            o->timer = 0;
            PlayerSetAnimIfChanged(o, 0xd);
            FUN_8001f96c(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
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
        if (*k & 0x8000) o->animFrame = 1;
        if (*k & 0x2000) o->animFrame = 0;
        BITTEN_OBJECT->animFrame = o->animFrame & 1;
        o->h->p.whole = BITTEN_OBJECT->h->p.whole;
        o->y.p.whole = BITTEN_OBJECT->y.p.whole - BITTEN_OBJECT->box2;
        o->d8c = BITTEN_OBJECT->d8c;
        if (D_1F8001FC & 0x40) {
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
#else
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
#endif
