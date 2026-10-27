// FUNC 80135db0 736 X000
// MATCHING 80135db0 736
#include "TOBJ.H"
extern short COS_TABLE[];
extern char D_80077CE8[];
extern void *A00_ROAMING_FLYER_BOB_ANIMS;
extern void *A00_ROAMING_FLYER_BOB_ANIM;
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void applyAnimVelocityX(TObj *, int);

void a00RoamingFlyerBob(TObj *o)
{
    switch (o->step) {
    case 0:
        o->b6b = 0;
        o->movetab = D_80077CE8;
        o->step++;
        o->velV = 0x80;
        o->d88 = 0;
        o->anim = A00_ROAMING_FLYER_BOB_ANIMS;
        AnimLoadDuration(o);
        break;
    case 1:
    case 4:
        if (o->step == 4 && o->b6b == 0) {
            if (AnimAdvance(o)) {
                o->anim = A00_ROAMING_FLYER_BOB_ANIMS;
                AnimLoadDuration(o);
                o->b6b = 1;
            }
        } else {
            AnimAdvance(o);
        }
        applyAnimVelocityX(o, o->animFrame);
        o->d88 = (o->d88 + 1) & 0xff;
        o->y.raw -= (COS_TABLE[o->d88] * o->velV) >> 4;
        if (o->d88 > 0x20) {
            o->timer = 0;
            o->w22 = 0;
            o->step++;
        }
        break;
    case 2:
    case 5:
        AnimAdvance(o);
        applyAnimVelocityX(o, o->animFrame);
        if (*(unsigned short *)0x1F8001F8 & 1) {
            o->d88--;
        } else {
            o->d88++;
        }
        o->d88 = *(unsigned char *)&o->d88;
        o->y.raw -= (COS_TABLE[o->d88] * o->velV) >> 4;
        if (++o->timer >= 0x18) {
            o->step++;
        }
        break;
    case 3:
    case 6:
        AnimAdvance(o);
        applyAnimVelocityX(o, o->animFrame);
        o->d88 = (o->d88 + 1) & 0xff;
        o->y.raw -= (COS_TABLE[o->d88] * o->velV) >> 4;
        if (o->d88 >= 0x80) {
            *(int *)((char *)o + 0x88) = 0;
            *(short *)((char *)o + 0x20) = 0x10;
            *(short *)((char *)o + 0x16) = o->d34;
            o->anim = A00_ROAMING_FLYER_BOB_ANIM;
            AnimLoadDuration(o);
            o->step++;
        }
        break;
    case 7:
        AnimAdvance(o);
        if (--o->timer == -1) {
            o->step++;
        }
        break;
    case 8:
        o->step = 0;
        o->animFrame = 1 - o->animFrame;
        break;
    }
}
