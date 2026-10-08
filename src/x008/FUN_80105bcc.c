// FUNC 80105bcc 2308 X008
// MATCHING 80105bcc 2308
#include "TOBJ.H"
#include "raw7.h"
extern TObj *D_8009D2E8;
extern TObj *D_8009C330;
extern TObj *D_8009F0EC;
extern unsigned short D_1F8001FC, D_1F8003C8, D_1F8003C6, D_1F8003C4;
extern unsigned short D_8009D670;
extern int D_8009C984;
extern short D_8009C944;
extern short D_8009C946[];
extern unsigned char D_8009D2B1;
extern unsigned char D_8009CF06;
extern unsigned char D_801152E8[];
extern void FUN_8010eaf8(TObj *o);
extern void FUN_800ef490(TObj *o);
extern void ObjGravityStep(TObj *o);
extern void ObjAddVelY7E(TObj *o);
extern int AnimAdvance(TObj *o);
extern void TileCollideAt(TObj *o, int x, int y);
extern int ObjCheckHeadCollision(TObj *o);
extern void ObjMotionStep(TObj *o);
extern short func_80041EBC(TObj *o, int x, int y);
extern short ObjTileCollide(TObj *o, int a, int b);
extern void ObjSetAnimFromTable(TObj *o);
extern void AnimLoadDuration(TObj *o);
extern void PlayerSetAnimIfChanged(TObj *o, int n);
extern void SfxPlay2(int a, int b);
extern void FUN_80104cd8(TObj *o);
extern void FUN_800ee428(TObj *o);
extern TObj *FUN_8004bbc0(TObj *, int);
extern void FUN_800efc8c(TObj *, int);
extern void func_8010E328(TObj *, int);

#define LAND() \
    o->d8c = D_801152E8[o->wb0]; \
    U8(D_8009C330, 8) = 0; \
    o->b9c = 0; \
    o->ba7 = 0; \
    U8(o, 0xac) = 0; \
    o->d84 = 0; \
    o->wb2 = 0; \
    o->velY = 0; \
    if (D_8009D2E8->type == 0x1c) { \
        switch (D_8009D2B1) { \
        case 1: \
            U8(o, 0xac) = 0; \
            o->step = 0x2a; \
            o->state = 0; \
            U8(o, 0xab) |= 0x80; \
            return; \
        case 2: \
            U8(o, 0xac) = 0; \
            o->step = 0x2b; \
            o->state = 0; \
            U8(o, 0xab) |= 0x80; \
            return; \
        default: \
            U8(o, 0xac) = 0; \
        } \
    } \
    o->step = 0; \
    o->state = 0;

#define TURN() \
    U8(D_8009C330, 8) = 0; \
    U8(o, 0xad) = 0; \
    o->wb2 = 0; \
    D_8009C330->timer = 0xe; \
    if (D_8009D2E8->type == 0x1c && (t = D_8009D2B1) < 3 && t != 0) \
        U8(o, 0xab) |= 0x80; \
    if ((D_8009C984 & 0x40) && (*(volatile unsigned short *)&D_8009D670 & D_1F8003C4)) \
        o->ba7 = 1; \
    FUN_800ee428(o); \
    o->step = 2; \
    o->state = 3;

void FUN_80105bcc(TObj *o)
{
    TObj *p;
    int t;

    switch (o->substep) {
    case 0:
        D_8009D2E8->animFrame = o->animFrame & 1;
        D_8009D2E8->d->p.whole = o->d->p.whole;
        D_8009D2E8->step = 1;
        D_8009D2E8->state = 6;
        o->h->raw += D_8009C944 << 8;
        o->y.raw += D_8009C946[0] << 8;
        FUN_8010eaf8(o);
        if (D_8009D2E8->type != 0x1c)
            o->h->raw += o->velX << 8;
        p = D_8009D2E8;
        p->h->p.whole = o->h->p.whole;
        p->y.p.whole = o->y.p.whole + p->box2;
        FUN_800ef490(o);
        ObjGravityStep(o);
        ObjAddVelY7E(o);
        AnimAdvance(o);
        if (o->velY > 0) {
            o->d84 = 0;
            o->b9c = 2;
            o->velY = 0;
            o->substep = 1;
        }
        TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x10));
        if (ObjCheckHeadCollision(o)) {
            o->d84 = 0;
            o->b9c = 2;
            o->velY = 0;
            o->substep = 1;
        }
        if ((D_1F8001FC & D_1F8003C8) || (D_1F8001FC & D_1F8003C6)) {
            D_8009C330->animTimer = 0xf;
            o->velY = 0;
            o->substep = 2;
        }
        break;
    case 1:
        FUN_800ef490(o);
        p = D_8009D2E8;
        p->animFrame = o->animFrame & 1;
        p->h->p.whole = o->h->p.whole;
        p->y.p.whole = o->y.p.whole + p->box2;
        p->d8c = o->d88 - 0xc0;
        o->h->raw += D_8009C944 << 8;
        o->y.raw += D_8009C946[0] << 8;
        FUN_8010eaf8(o);
        if (D_8009D2E8->type != 0x1c)
            o->h->raw += o->velX << 8;
        ObjGravityStep(o);
        ObjAddVelY7E(o);
        AnimAdvance(o);
        if (o->b69 != 0 || func_80041EBC(o, o->h->p.whole, (short)(o->y.p.whole + 0x30)) != 0 ||
            (D_1F8001FC & D_1F8003C8) || (D_1F8001FC & D_1F8003C6) || ObjTileCollide(o, 4, 0) != 0) {
            o->velY = 0;
            o->substep = 2;
        }
        break;
    case 2:
        PlayerSetAnimIfChanged(o, 0xf);
        if (AnimAdvance(o)) {
            SfxPlay2(0x22, 0x23);
            o->velY = 0;
            D_8009C330->animTimer = 0x1d;
            o->substep = 3;
        } else {
            FUN_80104cd8(o);
        }
        break;
    case 3:
        switch (D_8009D2E8->type) {
        case 0x1f:
            D_8009D2E8->b04 = 2;
            D_8009D2E8->step = 1;
            break;
        case 3:
        case 0x2b:
            D_8009D2E8->b04 = 2;
            D_8009D2E8->step = 2;
            break;
        case 0x12:
            D_8009D2E8->b04 = 2;
            D_8009D2E8->step = 3;
            break;
        case 0x1c:
            U8(o, 0xab) = 0;
            switch (D_8009D2E8->subtype) {
            case 0 ... 2:
                D_8009D2B1 = D_8009D2E8->subtype;
                break;
            case 4:
                D_8009CF06 = 1;
                break;
            }
            D_8009D2E8->step = 3;
            D_8009D2E8->state = 0;
            goto anim;
        default:
            t = (int)D_8009D2E8;
            ((TObj *)t)->b04 = 3;
            D_8009D2E8->step = 0;
            break;
        }
        D_8009D2E8->state = 0;
    anim:
        ObjSetAnimFromTable(o);
        AnimLoadDuration(o);
        D_8009C330->animFrame = D_8009C330->animTimer;
        o->d8c = 0x200;
        o->wb2 = 0x100;
        o->b9c = 2;
        U8(o, 0xac) = 1;
        o->velY = 0;
        o->substep = 4;
        break;
    case 4:
        o->h->raw += D_8009C944 << 8;
        o->y.raw += D_8009C946[0] << 8;
        FUN_800ef490(o);
        ObjMotionStep(o);
        if (U8(o, 0xac) == 2) {
            D_8009D2E8 = (TObj *)S32(o, 0xe4);
            U8(D_8009C330, 8) = 0;
            D_8009C330->timer = 0;
            o->ba7 = 0;
            o->ba5 = 0;
            o->step = 0xe;
            o->state = 0;
            U8(o, 0xab) &= 0x7f;
            break;
        }
        if (o->b69 == 1) {
            LAND();
            break;
        }
        if (ObjTileCollide(o, 0, 0)) {
            LAND();
            break;
        }
        if (o->animFrame & 1) {
            o->d8c += 0x10;
            if (o->d8c >= 0x300) {
                o->d8c = 0x300;
                TURN();
            }
        } else {
            o->d8c -= 0x10;
            if (o->d8c <= 0x100) {
                o->d8c = 0x100;
                TURN();
            }
        }
        if (U8(o, 0xac) < 2) {
            D_8009F0EC = FUN_8004bbc0(o, 0);
            if (D_8009F0EC) {
                U8(D_8009C330, 8) = 0;
                D_8009C330->timer = 0;
                U8(o, 0xac) = 0;
                o->ba7 = 0;
                o->b9c = 0;
                o->wb2 = 0;
                FUN_800efc8c(o, D_8009F0EC == (TObj *)1);
            }
        }
        if (o->step == 0xf && o->b9e == 0)
            func_8010E328(o, 1);
        break;
    }
}
