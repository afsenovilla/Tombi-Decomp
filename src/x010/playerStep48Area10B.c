// FUNC 8011d5f4 1116 X010
// MATCHING 8011d5f4 1116
#include "gvars.h"
#include "TOBJ.H"

extern TObj *BITTEN_OBJECT;
extern TObj *PLAYER_EXT;

extern volatile unsigned short PAD_PRESSED[];
extern unsigned short JOYPAD_STATE, BUTTON_MASK_B;
extern int ObjCheckHeadCollision(TObj *);
extern void ObjSetAnimFromTable(TObj *);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern TObj *allocObjectLayer1(void);
extern void SfxPlay2(int, int);
extern void FUN_800eeb5c(TObj *, int);
extern void playerPlaceBittenObject(TObj *);
extern void playerBiteHoldAttach(TObj *);
extern void playerResetSlopeState(TObj *);

void playerStep48Area10B(TObj *o)
{
    TObj *p;
    TObj *q;
    short f;

    switch (o->state) {
    case 1:
        o->timer++;
        o->y.p.whole += 8;
        f = 0;
        if (o->y.p.whole + BITTEN_OBJECT->box2 > BITTEN_OBJECT->y.p.whole) {
            o->y.p.whole = BITTEN_OBJECT->y.p.whole - BITTEN_OBJECT->box2;
            f = 1;
        }


        if (o->velX < 0) {
            o->h->p.whole -= 8;
            if (BITTEN_OBJECT->h->p.whole >= o->h->p.whole) {
                o->h->p.whole = BITTEN_OBJECT->h->p.whole;
                if (f) {
                    playerBiteHoldAttach(o);
                    playerResetSlopeState(o);
                }
            }
        } else {
            o->h->p.whole += 8;
            if (o->h->p.whole >= BITTEN_OBJECT->h->p.whole) {
                o->h->p.whole = BITTEN_OBJECT->h->p.whole;
                if (f) {
                    playerBiteHoldAttach(o);
                    playerResetSlopeState(o);
                }
            }
        }
        if (o->timer >= 0xb) {
            playerBiteHoldAttach(o);
            playerResetSlopeState(o);
        }
        break;
    case 2:
        o->h->raw += ((short*)&GVARS.driftH)[0] << 8;
        o->y.raw += ((short*)&GVARS.driftY)[0] << 8;
        ObjCheckHeadCollision(o);
        if (PAD_PRESSED[0] & 0x80) o->animFrame = 1;
        if (PAD_PRESSED[0] & 0x20) o->animFrame = 0;
        p = BITTEN_OBJECT;
        p->animFrame = o->animFrame & 1;
        p->h->p.whole = o->h->p.whole;
        p->y.p.whole = o->y.p.whole + p->box2;
        if (JOYPAD_STATE & BUTTON_MASK_B) {
            o->b69 = 0;
            o->wb2 = 0;
            o->velX = 0;
            o->velY = 0;
            if (PAD_PRESSED[0] & 0x40) PLAYER_EXT->animTimer = 0xf;
            else PLAYER_EXT->animTimer = 0xe;
            if (PLAYER_EXT->animFrame != PLAYER_EXT->animTimer) {
                ObjSetAnimFromTable(o);
                AnimLoadDuration(o);
                PLAYER_EXT->animFrame = PLAYER_EXT->animTimer;
            }
            o->b9c = 0;
            o->velY = 0;
            o->state = 3;
        }
        if (o->y.p.whole < o->w56 + 0x10) o->y.p.whole = o->w56 + 0x10;
        break;
    case 3:
        if (AnimAdvance(o)) {
            q = allocObjectLayer1();
            if (q) {
                q->active = 1;
                q->type = 1;
                q->subtype = 0xd;
                q->b0c = BITTEN_OBJECT->subtype;
                {
                    unsigned short t = o->animFrame & 1;
                    q->animFrame = t;
                    if (PAD_PRESSED[0] & 0x40) q->animFrame = t | 2;
                }
                q->a.p.whole = o->a.p.whole;
                q->y.p.whole = o->y.p.whole;
                q->b.p.whole = o->b.p.whole;
                {
                    Fix16 *h = q->h;
                    int t = h->p.whole;
                    h->p.whole = (o->animFrame & 1) ? t - 0x10 : t + 0x10;
                }
            }
            o->velY = 0;
            SfxPlay2(0x22, 0x23);
            o->velY = 0;
            FUN_800eeb5c(o, 0x1d);
            o->d8c = 0x200;
            o->state = 4;
        } else {
            playerPlaceBittenObject(o);
        }
        break;
    case 4:
        BITTEN_OBJECT->b04 = 3;
        {
            unsigned short t = o->animFrame & 1;
            BITTEN_OBJECT->animFrame = t;
            if (PAD_PRESSED[0] & 0x40) BITTEN_OBJECT->animFrame = t | 2;
        }
        o->b9c = 0;
        *(unsigned char *)&o->wac = 0;
        o->step = 0x3e;
        o->state = 0;
        break;
    }
}
