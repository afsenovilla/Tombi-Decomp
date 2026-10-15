// FUNC 8011d3ec 660 X009
// MATCHING 8011d3ec 660
#include "spad.h"
#include "TOBJ.H"
extern TObj *D_8009D2E8;
extern void playerBittenCarryAirStep(TObj *);
extern void func_80104F2C(TObj *);
extern void FUN_800ef490(TObj *);
extern void ObjGravityStep(TObj *);
extern void ObjAddVelY7E(TObj *);
extern int AnimAdvance(TObj *);
extern short probeFloorPlaneAt(TObj *, short, short);
extern short ObjTileCollide(TObj *, int, int);
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void SfxPlay2(int, int);
extern TObj *allocObjectLayer1(void);
extern void playerPlaceBittenObject(TObj *);
extern void playerBittenJumpAirMove(TObj *);

void func_8011D3EC(TObj *o)
{
    TObj *p;
    TObj *e;

    switch (o->substep) {
    case 0:
        D_8009D2E8->d8c = o->d88 - 0xc0;
        playerBittenCarryAirStep(o);
        func_80104F2C(o);
        break;
    case 1:
        FUN_800ef490(o);
        p = D_8009D2E8;
        p->h->p.whole = o->h->p.whole;
        p->y.p.whole = o->y.p.whole + p->box2;
        p->d8c = o->d88 - 0xc0;
        ObjGravityStep(o);
        ObjAddVelY7E(o);
        AnimAdvance(o);
        if (o->b69 == 0 && !probeFloorPlaneAt(o, o->h->p.whole, o->y.p.whole + 0x40)
            && !(SPAD.s1fc & SPAD.s3c8) && !(SPAD.s1fc & SPAD.s3c6) && !ObjTileCollide(o, 4, 0)) {
            break;
        }
        o->b9c = 0;
        *(char *)&o->wac = 0;
        o->velY = 0;
        o->substep = 2;
        break;
    case 2:
        PlayerSetAnimIfChanged(o, 0xf);
        if (AnimAdvance(o)) {
            SfxPlay2(0x22, 0x23);
            e = allocObjectLayer1();
            if (e) {
                e->active = 1;
                e->type = 4;
                e->subtype = 6;
                {
                    unsigned char c = D_8009D2E8->b0c;
                    e->animFrame |= 2;
                    e->b0c = c;
                }
                e->a.p.whole = o->a.p.whole;
                e->y.p.whole = o->y.p.whole;
                e->b.p.whole = o->b.p.whole;
                {
                    Fix16 *hp = e->h;
                    int x = hp->p.whole;
                    hp->p.whole = (o->animFrame & 1) ? x - 16 : x + 16;
                }
            }
            o->velY = 0;
            o->substep = 3;
        } else {
            playerPlaceBittenObject(o);
        }
        break;
    case 3:
        D_8009D2E8->b04 = 3;
        PlayerSetAnimIfChanged(o, 0x1d);
        o->d8c = 0x200;
        o->wb2 = 0x100;
        o->b9c = 2;
        *(char *)&o->wac = 1;
        o->velY = 0;
        o->substep = 4;
        break;
    case 4:
        playerBittenJumpAirMove(o);
        break;
    }
}
