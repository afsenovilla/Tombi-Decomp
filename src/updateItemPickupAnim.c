// FUNC 8003be14 364 MAIN0
// MATCHING 8003be14 364
// VERSION SLPM_80165 slpm80165 80039ACC 352
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 8003BE14 (updateItemPickupAnim.c): no subtype 13 exception.
#include "TOBJ.H"
extern unsigned char DAT_8007a138[];
extern char MOVE_TABLE_2[], MOVE_TABLE_0[];
extern void SfxPlay(int);
extern void awardAPByArea(int, int, int, int);
extern void applyAnimVelocityX(TObj *, unsigned short);

void updateItemPickupAnim(TObj *o)
{
    unsigned short u;
    short s;
    switch (o->state) {
    case 0:
        SfxPlay(DAT_8007a138[o->subtype]);
        awardAPByArea(500, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        o->b0b = 1;
        o->b0f = 4;
        o->velV = -0x400;
        o->ba5 = 0;
        o->movetab = MOVE_TABLE_2;
        if (o->animFrame & 2)
            o->movetab = MOVE_TABLE_0;
        o->state = o->state + 1;
        break;
    case 1:
        u = o->animFrame;
        if (u & 2)
            applyAnimVelocityX(o, u & 1);
        else
            applyAnimVelocityX(o, 1 - u);
        s = o->velV + 0x40;
        o->velV = s;
        if (s > 0x400)
            o->velV = 0x400;
        o->y.raw = o->y.raw + o->velV * 0x100;
        break;
    }
    if (o->animFrame & 1)
        o->d8c = (o->d8c + 0x18) & 0xff;
    else
        o->d8c = (o->d8c - 0x18) & 0xff;
}
#else
#include "TOBJ.H"
extern unsigned char DAT_8007a138[];
extern char MOVE_TABLE_2[], MOVE_TABLE_0[];
extern void SfxPlay(int);
extern void awardAPByArea(int, int, int, int);
extern void applyAnimVelocityX(TObj *, unsigned short);

void updateItemPickupAnim(TObj *o)
{
    unsigned short u;
    short s;
    switch (o->state) {
    case 0:
        SfxPlay(DAT_8007a138[o->subtype]);
        awardAPByArea(500, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        o->ba5 = 0;
        if (o->subtype != 0xd) {
            o->b0b = 1;
            o->b0f = 4;
        }
        o->velV = -0x400;
        o->movetab = MOVE_TABLE_2;
        if (o->animFrame & 2)
            o->movetab = MOVE_TABLE_0;
        o->state = o->state + 1;
        break;
    case 1:
        u = o->animFrame;
        if (u & 2)
            applyAnimVelocityX(o, u & 1);
        else
            applyAnimVelocityX(o, 1 - u);
        s = o->velV + 0x40;
        o->velV = s;
        if (s > 0x400)
            o->velV = 0x400;
        o->y.raw = o->y.raw + o->velV * 0x100;
        break;
    }
    if (o->animFrame & 1)
        o->d8c = (o->d8c + 0x18) & 0xff;
    else
        o->d8c = (o->d8c - 0x18) & 0xff;
}
#endif
