// FUNC 8003be14 364 MAIN0
#include "TOBJ.H"
extern unsigned char DAT_8007a138[];
extern char DAT_80077d0c[], DAT_80077cdc[];
extern void SfxPlay(int);
extern void FUN_800e9f74(int, int, int, int);
extern void FUN_8001fa88(TObj *, unsigned short);

void FUN_8003be14(TObj *o)
{
    unsigned short u;
    short s;
    switch (o->state) {
    case 0:
        SfxPlay(DAT_8007a138[o->subtype]);
        FUN_800e9f74(500, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        o->ba5 = 0;
        if (o->subtype != 0xd) {
            o->b0b = 1;
            o->b0f = 4;
        }
        o->velV = -0x400;
        o->movetab = DAT_80077d0c;
        if (o->animFrame & 2)
            o->movetab = DAT_80077cdc;
        o->state = o->state + 1;
        break;
    case 1:
        u = o->animFrame;
        if (u & 2)
            u = u & 1;
        else
            u = 1 - u;
        FUN_8001fa88(o, u);
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
