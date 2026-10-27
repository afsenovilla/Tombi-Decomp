// FUNC 8011b47c 948 X014
// MATCHING 8011b47c 948
#include "gvars.h"
#include "TOBJ.H"
#include "raw7.h"
extern TObj *BITTEN_OBJECT;
extern TObj *PLAYER_EXT;
extern volatile unsigned short PAD_PRESSED[];
extern unsigned short JOYPAD_STATE, D_1F8003C6;
extern void playerBiteHoldAttach(TObj *);
extern void playerResetSlopeState(TObj *);
extern void FUN_8003facc(TObj *);
extern void ObjSetAnimFromTable(TObj *);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void SfxPlay2(int, int);
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void playerPlaceBittenObject(TObj *);

void playerStep48Area14(TObj *o)
{
    TObj *q;
    volatile unsigned short *k;
    short f;
    short y;

    switch (o->state) {
    case 1:
        o->timer++;
        o->y.p.whole += 8;
        y = o->y.p.whole;
        f = 0;
        {
            TObj *p = BITTEN_OBJECT;
            if (y + p->box2 > p->y.p.whole) {
                o->y.p.whole = p->y.p.whole - p->box2;
                f = 1;
            }
        }
        if (o->velX < 0) {
            o->h->p.whole -= 8;
            if (o->h->p.whole <= BITTEN_OBJECT->h->p.whole) {
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
        o->h->raw += ((short*)&GVARS.s44)[0] << 8;
        o->y.raw += ((short*)&GVARS.s46)[0] << 8;
        FUN_8003facc(o);
        k = PAD_PRESSED;
        if (*k & 0x80)
            o->animFrame = 1;
        if (*k & 0x20)
            o->animFrame = 0;
        {
            TObj *p = BITTEN_OBJECT;
            p->animFrame = o->animFrame & 1;
            p->h->p.whole = o->h->p.whole;
            p->y.p.whole = o->y.p.whole + p->box2;
        }
        if (JOYPAD_STATE & D_1F8003C6) {
            o->b69 = 0;
            o->wb2 = 0;
            o->velX = 0;
            o->velY = 0;
            if (*k & 0x40)
                PLAYER_EXT->animTimer = 0xf;
            else
                PLAYER_EXT->animTimer = 0xe;
            if (PLAYER_EXT->animFrame != PLAYER_EXT->animTimer) {
                BITTEN_OBJECT->state = 7;
                ObjSetAnimFromTable(o);
                AnimLoadDuration(o);
                PLAYER_EXT->animFrame = PLAYER_EXT->animTimer;
            }
            o->b9c = 0;
            o->velY = 0;
            o->state = 3;
        }
        break;
    case 3:
        if (AnimAdvance(o)) {
            SfxPlay2(0x22, 0x23);
            o->velY = 0;
            PlayerSetAnimIfChanged(o, 0x1d);
            o->d8c = 0x200;
            o->state = 4;
        } else
            playerPlaceBittenObject(o);
        break;
    case 4:
        BITTEN_OBJECT->b04 = 2;
        BITTEN_OBJECT->step = 3;
        BITTEN_OBJECT->state = 0;
        q = BITTEN_OBJECT;
        y = o->animFrame & 1;
        q->animFrame = y;
        if (PAD_PRESSED[0] & 0x40)
            q->animFrame = y | 2;
        o->b9c = 0;
        (*(unsigned char*)&o->wac) = 0;
        o->step = 0x3e;
        o->state = 0;
        break;
    }
}
