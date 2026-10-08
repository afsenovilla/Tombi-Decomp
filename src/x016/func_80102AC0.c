// FUNC 80102ac0 1012 X016
// MATCHING 80102ac0 1012
#include "TOBJ.H"
extern TObj *D_8009D2E8;
extern unsigned short D_8009D670;
extern unsigned short D_1F8003C6, D_1F8001FC;
extern unsigned char D_8009D2B0;
extern void ObjMotionStep(TObj *);
extern void FUN_8001f96c(int, int, int, int);
extern void PlayerSetAnimIfChanged(TObj *, int);
extern short ObjTileCollide(TObj *, int, int);

void func_80102AC0(TObj *o)
{
    short f;
    volatile unsigned short *k;


    switch (o->state) {
    case 1:
        o->timer++;
        ObjMotionStep(o);
        D_8009D2E8->animFrame = o->animFrame & 1;
        f = 0;
        o->y.p.whole += 8;
        if (D_8009D2E8->y.p.whole < o->y.p.whole + 8) {
            o->y.p.whole = D_8009D2E8->y.p.whole - 8;
            f = 1;
        }
        if (o->velX < 0) {
            o->h->p.whole -= 8;
            if (o->h->p.whole <= D_8009D2E8->h->p.whole) {
                o->h->p.whole = D_8009D2E8->h->p.whole;
                if (f) {
                    o->timer = 0;
                    PlayerSetAnimIfChanged(o, 0xd);
                    FUN_8001f96c(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
                    if (D_8009D2E8->type == 2) D_8009D2E8->state = 2;
                }
            }
        } else {
            o->h->p.whole += 8;
            if (o->h->p.whole >= D_8009D2E8->h->p.whole) {
                o->h->p.whole = D_8009D2E8->h->p.whole;
                if (f) {
                    o->timer = 0;
                    PlayerSetAnimIfChanged(o, 0xd);
                    FUN_8001f96c(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
                    if (D_8009D2E8->type == 2) D_8009D2E8->state = 2;
                }
            }
        }
        if (D_8009D2E8->type == 0x21) {
            D_8009D2E8->h->p.whole = o->h->p.whole;
            D_8009D2E8->y.p.whole = o->y.p.whole + 8;
            PlayerSetAnimIfChanged(o, 0xd);
            FUN_8001f96c(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            o->state = 2;
        } else if (o->b69 != 0 || ObjTileCollide(o, 0, 1)) {
            D_8009D2E8->h->p.whole = o->h->p.whole;
            D_8009D2E8->y.p.whole = o->y.p.whole + 8;
            PlayerSetAnimIfChanged(o, 0xd);
            FUN_8001f96c(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            if (D_8009D2E8->type == 2) D_8009D2E8->state = 2;
            o->state = 2;
        } else if (o->timer >= 11) {
            D_8009D2E8->h->p.whole = o->h->p.whole;
            D_8009D2E8->y.p.whole = o->y.p.whole + 8;
            o->timer = 0;
            PlayerSetAnimIfChanged(o, 0xd);
            FUN_8001f96c(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            if (D_8009D2E8->type == 2) D_8009D2E8->state = 2;
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
        k = &D_8009D670;
        if (*k & 0x80) o->animFrame = 1;
        if (*k & 0x20) o->animFrame = 0;
        D_8009D2E8->animFrame = o->animFrame & 1;
        o->h->p.whole = D_8009D2E8->h->p.whole;
        o->y.p.whole = D_8009D2E8->y.p.whole - D_8009D2E8->box2;
        o->d8c = D_8009D2E8->d8c;
        if (D_1F8001FC & D_1F8003C6) {
            D_8009D2B0 = 0;
            o->b9c = 1;
            o->b69 = 0;
            *(unsigned char *)&o->wac = 3;
            o->wb2 = 0;
            if (D_8009D2E8->type == 2) {
                D_8009D2E8->active = 2;
                D_8009D2E8->state = 6;
            }
            o->step = 0xf;
            o->state = 0;
        }
        break;
    }
}
