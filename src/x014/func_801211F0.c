// FUNC 801211f0 516 X014
// MATCHING 801211f0 516
#include "spad.h"
#include "TOBJ.H"
#include "gamestate.h"

extern unsigned short D_80126590[];
static short D_801265A0[8] = {
    0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xa0, 0xe0, 0xc0,
};
extern short DAT_8007a1f0[];
extern short DAT_8007a5f0[];
extern int Rand(void);
extern void FUN_8001e4f0(int);
extern int func_80120DA0(TObj *);

void func_801211F0(TObj *o)
{
    switch (o->step) {
    case 0:
        o->h->p.whole = *(unsigned short *)0x1F800176 + D_80126590[Rand() & 7];
        o->y.p.whole = ((unsigned short*)&SPAD.s17a)[0] - 0xf0;
        o->d38 = D_801265A0[Rand() & 7];
        o->velH = 0;
        o->step++;
        if (GAME.selectedSection == 7)
            FUN_8001e4f0(0xf0);
        else
            FUN_8001e4f0(0xcf);
        break;
    case 1:
        {
            int i = o->d38;
            int dx = (o->velH * DAT_8007a5f0[i]) << 4 >> 16;
            int dy = (o->velH * DAT_8007a1f0[i]) << 4 >> 16;
            o->h->raw += dx << 8;
            o->y.raw += dy << 8;
        }
        o->velH += 0x20;
        if (o->velH > 0x480)
            o->velH = 0x480;
        if (((o->d38 - 0x40) & 0xff) > 0x80)
            o->d8c = (o->d8c + 0x40) & 0xfff;
        else
            o->d8c = (o->d8c - 0x40) & 0xfff;
        if (func_80120DA0(o))
            o->step++;
        break;
    case 2:
        o->b04++;
        break;
    }
}
