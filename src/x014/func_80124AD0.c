// FUNC 80124ad0 1112 X014
// MATCHING 80124ad0 1112
#include "spad.h"
#include "TOBJ.H"
#include "gamestate.h"

static short D_80126718[8] = {
    0x100, -0x380, 0x180, -0x580, 0x200, -0x480, 0x80, -0x300,
};
extern short D_1F80027E;
extern int func_8012491C(TObj *o);
extern int func_801249D0(TObj *o);
extern short TileCollideAt(TObj *o, short x, short y);
extern short probeWallAt(TObj *o, short x, short y, int k);

static __inline__ int land(TObj *o)
{
    if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 10)) {
        o->wae = SPAD.s284;
        o->wb2 = D_1F80027E;
        return 1;
    }
    if (probeWallAt(o, o->h->p.whole - 8, o->y.p.whole, 1)) return 0;
    probeWallAt(o, o->h->p.whole + 8, o->y.p.whole, 0);
    return 0;
}

void func_80124AD0(TObj *o)
{
    short *e;
    short v;

    switch (o->step) {
    case 0:
        { int ix;
        o->step++;
        ix = *(unsigned short *)&o->wb4 << 2;
        ix += (int)D_80126718;
        o->velH = *(short *)ix;
        ix += 2;
        o->velV = *(short *)ix;
        }
        if (o->animFrame) o->velH = -o->velH;
        break;
    case 1:
        o->a.raw += o->velH << 8;
        if (func_8012491C(o)) o->velH = 0;
        o->velV += 0x20;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b69 = 0;
            o->b9c = 2;
            o->step++;
        }
        break;
    case 4:
        v = o->velH;
        if (v < 0) {
            o->velH = v + 0x30;
            if (o->velH >= 0) o->velH = 0;
        } else {
            o->velH = v - 0x30;
            if (o->velH <= 0) o->velH = 0;
        }
    case 2:
        o->a.raw += o->velH << 8;
        if (func_8012491C(o)) o->velH = 0;
        o->velV += 0x20;
        if (o->velV > 0x500) o->velV = 0x500;
        o->wb2 = 0;
        o->y.raw += o->velV << 8;
        if (land(o)) {
            if (o->wae == 1 || func_801249D0(o)) {
                o->active = 2;
                o->b04 = 2;
                o->step = 0;
                o->state = 0;
            } else if (GAME.selectedSection != 7) {
                o->step = 3;
                o->velH = 0;
                o->velV = 0;
            } else {
                o->active = 2;
                o->b04 = 2;
                o->step = 0;
                o->state = 0;
            }
        }
        break;
    case 3:
        o->y.p.whole += 3;
        o->wb2 = 0;
        if (land(o)) {
            if (o->wb2) {
                if (o->wb2 > 0) {
                    o->velH += 0x38;
                    if (o->velH > 0x400) o->velH = 0x400;
                } else {
                    o->velH -= 0x38;
                    if (o->velH < -0x400) o->velH = -0x400;
                }
            }
            o->a.raw += o->velH << 8;
            if (func_8012491C(o)) o->velH = 0;
        } else {
            o->step = 4;
            o->y.p.whole -= 3;
        }
        break;
    }
    if (o->velH < 0) o->d8c += 0x10;
    else o->d8c -= 0x10;
    o->d8c = *(unsigned char *)&o->d8c;
}
