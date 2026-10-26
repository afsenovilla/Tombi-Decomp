// FUNC 800edad4 888 I0NFO
// MATCHING 800edad4 888
#include "TOBJ.H"
extern void func_8001FE6C(TObj *o);
extern void func_800202B4(TObj *o);
extern void **D_800EE914[][3];
extern TObj PLAYER;
extern unsigned char D_8009CEAB;
extern unsigned char D_8009CEDB;
extern signed char D_8009D2B0[];

void overheadFollowerStep(TObj *o)
{
    func_800202B4(o);
    if (D_8009CEAB == 5) return;
    switch (PLAYER.animFrame & 7) {
    case 0:
        o->animFrame = 0;
        o->anim = D_800EE914[o->subtype][0][0xD2];
        o->a.p.whole = PLAYER.a.p.whole - 4;
        o->y.p.whole = PLAYER.y.p.whole - 0x10;
        o->b0f = PLAYER.b0f + 1;
        break;
    case 1:
        o->animFrame = 0;
        o->anim = D_800EE914[o->subtype][0][0xD3];
        o->a.p.whole = PLAYER.a.p.whole - 4;
        o->y.p.whole = PLAYER.y.p.whole - 0x10;
        o->b0f = PLAYER.b0f - 2;
        break;
    case 2:
        o->animFrame = 0;
        o->anim = D_800EE914[o->subtype][0][0xD3];
        o->a.p.whole = PLAYER.a.p.whole;
        o->y.p.whole = PLAYER.y.p.whole - 0x10;
        o->b0f = PLAYER.b0f - 2;
        break;
    case 3:
        o->animFrame = 2;
        o->anim = D_800EE914[o->subtype][0][0xD3];
        o->a.p.whole = PLAYER.a.p.whole + 4;
        o->y.p.whole = PLAYER.y.p.whole - 0x10;
        o->b0f = PLAYER.b0f - 2;
        break;
    case 4:
        o->animFrame = 2;
        o->anim = D_800EE914[o->subtype][0][0xD2];
        o->a.p.whole = PLAYER.a.p.whole + 4;
        o->y.p.whole = PLAYER.y.p.whole - 0x10;
        o->b0f = PLAYER.b0f + 1;
        break;
    case 5:
        o->animFrame = 2;
        o->anim = D_800EE914[o->subtype][0][0xD3];
        o->a.p.whole = PLAYER.a.p.whole + 4;
        o->y.p.whole = PLAYER.y.p.whole - 0x10;
        o->b0f = PLAYER.b0f + 1;
        break;
    case 6:
        o->animFrame = 0;
        o->anim = D_800EE914[o->subtype][0][0xD3];
        o->a.p.whole = PLAYER.a.p.whole;
        o->y.p.whole = PLAYER.y.p.whole - 0x10;
        o->b0f = PLAYER.b0f + 1;
        break;
    case 7:
        o->animFrame = 0;
        o->anim = D_800EE914[o->subtype][0][0xD3];
        o->a.p.whole = PLAYER.a.p.whole - 4;
        o->y.p.whole = PLAYER.y.p.whole - 0x10;
        o->b0f = PLAYER.b0f + 1;
        break;
    }
    func_8001FE6C(o);
    o->b.raw = PLAYER.b.raw;
    o->_pad0e[0] = 3;
    if (D_8009D2B0[0] == 3) {
        o->b04 = 3;
        o->step = 0;
        o->state = 0;
        o->substep = 0;
    }
    if (D_8009CEDB == 1) {
        D_8009CEDB = 2;
        o->b04 = 3;
        o->step = 0;
        o->state = 0;
        o->substep = 0;
    }
}
