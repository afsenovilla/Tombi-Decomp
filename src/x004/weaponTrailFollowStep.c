// FUNC 800ebd40 552 X004
// MATCHING 800ebd40 552
#include "TOBJ.H"
typedef struct T12 { void **p; int a, b; } T12;
extern T12 WEAPON_TRAIL_DEFS_ANIMS[];
extern unsigned char PLAYER_VISIBLE[];
extern unsigned char PLAYER_DRAW_DEPTH;
extern volatile unsigned short PLAYER_ANIM_FRAME[];
extern unsigned short DAT_800a6066s;
extern Fix16 *PLAYER_H;
extern Fix16 *PLAYER_D;
extern unsigned short PLAYER_Y[];
extern void FUN_8001fe6c(TObj *);
extern void FUN_8001fec0(TObj *);

void weaponTrailFollowStep(TObj *o)
{
    int dx;
    short dy;
    int w;
    Fix16 *hp;
    TObj *q;
    switch (o->state) {
    case 0:
        o->anim = *WEAPON_TRAIL_DEFS_ANIMS[o->subtype].p;
        FUN_8001fe6c(o);
        o->visible = PLAYER_VISIBLE[0];
        o->b0f = PLAYER_DRAW_DEPTH - 1;
        o->state++;
    case 1:
        q = (TObj *)o->d90;
        switch (DAT_800a6066s) {
        case 0:
        case 1:
        case 2:
        case 3:
            dx = -0x16;
            dy = -4;
            break;
        case 4:
        case 5:
            dx = -0x18;
            dy = 0;
            break;
        case 6:
        case 7:
            dx = -0x16;
            dy = 6;
            break;
        }
        FUN_8001fec0(o);
        o->animFrame = PLAYER_ANIM_FRAME[0] & 1;
        w = PLAYER_H->p.whole;
        hp = o->h;
        if (PLAYER_ANIM_FRAME[0] & 1) hp->p.whole = w - (dx << 16 >> 16); else hp->p.whole = w + (dx << 16 >> 16);
        o->y.p.whole = PLAYER_Y[0] + dy;
        o->d->p.whole = PLAYER_D->p.whole;
        switch (o->subtype) {
        case 0:
            if (q->state == 7)
                break;
            if (q->state == 8) {
                o->animTimer = 1;
                break;
            }
            o->b04 = 2;
            break;
        case 1 ... 2:
            if (q->state == 6)
                break;
            if (q->state == 7) {
                o->animTimer = 1;
                break;
            }
            o->b04 = 2;
            break;
        }
        if (q->step >= 2)
            o->b04 = 2;
        if (q->b04 != 1)
            o->b04 = 2;
    }
}
