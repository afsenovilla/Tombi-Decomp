// FUNC 80102f40 700 X008
// MATCHING 80102f40 700
#include "TOBJ.H"
extern TObj *BITTEN_OBJECT;
extern unsigned short PAD_PRESSED;
extern unsigned char DAT_8009d2b0;
extern unsigned short DAT_1f8001fc;
extern unsigned short DAT_1f8003c6;
extern void FUN_800ee680(TObj *);
extern void FUN_800eeb5c(TObj *, int);
extern short FUN_8003fd78(TObj *, int, int);
extern void playerSetSlopeTilt(TObj *, int);

void playerBiteHoldType03(TObj *o)
{
    short flag;
    volatile unsigned short *pad;

    switch (o->state) {
    case 1:
        o->timer++;
        if (BITTEN_OBJECT->type == 3)
            BITTEN_OBJECT->d8c = 0;
        else if (BITTEN_OBJECT->type == 0x1f)
            BITTEN_OBJECT->d8c = 0;
        FUN_800ee680(o);
        flag = 0;
        if (o->y.p.whole + BITTEN_OBJECT->box2 >= BITTEN_OBJECT->y.p.whole) {
            flag = 1;
            BITTEN_OBJECT->y.p.whole = o->y.p.whole + o->wba;
        } else {
            o->y.p.whole += 8;
        }
        if (o->velX < 0) {
            if (o->h->p.whole > BITTEN_OBJECT->h->p.whole) {
                o->h->p.whole -= 8;
            } else {
                o->h->p.whole = BITTEN_OBJECT->h->p.whole;
                if (flag)
                    FUN_800eeb5c(o, 0x23);
            }
        } else {
            if (o->h->p.whole >= BITTEN_OBJECT->h->p.whole) {
                o->h->p.whole = BITTEN_OBJECT->h->p.whole;
                if (flag)
                    FUN_800eeb5c(o, 0x23);
            } else {
                o->h->p.whole += 8;
            }
        }
        if (BITTEN_OBJECT->b69 || o->b69 || FUN_8003fd78(o, 0, 1) || o->timer > 0x14) {
            o->timer = 0;
            FUN_800eeb5c(o, 0x23);
            *(unsigned char *)&o->wac = 3;
            o->b9c = 0;
            o->velX = 0;
            o->velY = 0;
            BITTEN_OBJECT->animFrame = o->animFrame & 1;
            o->h->p.whole = BITTEN_OBJECT->h->p.whole;
            BITTEN_OBJECT->y.p.whole = o->y.p.whole + o->wba;
            playerSetSlopeTilt(o, 0);
            o->state = 2;
        }
        break;
    case 2:
        pad = &PAD_PRESSED;
        if (*pad & 0x80)
            o->animFrame = 1;
        if (*pad & 0x20)
            o->animFrame = 0;
        o->h->p.whole = BITTEN_OBJECT->h->p.whole;
        BITTEN_OBJECT->y.p.whole = o->y.p.whole + o->wba;
        if (DAT_1f8001fc & DAT_1f8003c6) {
            DAT_8009d2b0 = 0;
            o->b9c = 1;
            o->b69 = 0;
            o->wb2 = 0;
            o->step = 0xf;
            o->state = 0;
        }
        break;
    }
}
