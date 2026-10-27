// FUNC 8002f384 348 MAIN0
// MATCHING 8002f384 348
#include "TOBJ.H"
extern void **PLAYER_EFFECT_4E_ANIM_TABLE;
extern unsigned char DAT_800a6039, DAT_800a6047[];
extern unsigned short DAT_800a6066, PLAYER_Y[];
extern short *PLAYER_H, *PLAYER_D[];
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);

void playerEffect4EStepB(TObj *o)
{
    unsigned char t;
    switch (o->state) {
    case 0:
        o->anim = *PLAYER_EFFECT_4E_ANIM_TABLE;
        AnimLoadDuration(o);
        o->timer = 0x3c;
        o->state++;
    case 1:
        break;
    default:
        return;
    }
    AnimAdvance(o);
    o->timer = o->timer - 1;
    o->visible = DAT_800a6039;
    if (o->timer > 0x20) {
        if (o->b6b > 0x7e) t = o->b6b;
        else t = o->b6b + 8;
        o->b6b = t;
    } else if (o->timer < 0) {
        o->b6b = 0;
        o->b04 = 2;
    } else {
        if (o->b6b == 0) t = 0;
        else t = o->b6b - 8;
        o->b6b = t;
    }
    {
        int x = PLAYER_H[1];
        o->h->p.whole = (DAT_800a6066 & 1) ? x + 4 : x - 4;
    }
    o->y.p.whole = PLAYER_Y[0] - 8;
    o->d->p.whole = PLAYER_D[0][1];
    o->b0f = DAT_800a6047[0] - 1;
}
