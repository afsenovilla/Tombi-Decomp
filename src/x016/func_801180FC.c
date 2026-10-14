// FUNC 801180fc 860 X016
// MATCHING 801180fc 860
#include "gcd.h"
#include "TOBJ.H"
typedef struct { char c[12]; } B12;
typedef struct { void **p; int x, y; } AT3;
extern TObj PLAYER;
extern unsigned char DAT_8009c942[], DAT_8009d2b0;
extern unsigned char D_8009D12F;
extern short D_1F8001C6;
extern AT3 D_80118D34[];
extern TObj *showMessageBoxDirect(int, int, B12 *);
extern void AnimJump(TObj *, int);
extern void AnimAdvance(TObj *);
extern void setEventStarted(int, int, int);

void func_801180FC(TObj *o)
{
    B12 v;

    switch (o->state) {
    case 0:
        if (o->b68 == 0)
            break;
        v = *(B12 *)&o->a;
        DAT_8009d2b0 = 2;
        DAT_8009c942[0] = 1;
        PLAYER.b04 = 5;
        PLAYER.step = 0;
        PLAYER.state = 0;
        o->anim = D_80118D34[o->subtype].p[2];
        AnimJump(o, 0);
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        if (((unsigned char*)&GCD.bcdac)[0] != 0xff) {
            if (D_8009D12F != 0)
                o->d90 = (int)showMessageBoxDirect(2, 0x14, &v);
            else if (GCD.bd076 == 0) {
                o->d90 = (int)showMessageBoxDirect(2, 0xd, &v);
                GCD.bd076 = 1;
            } else
                o->d90 = (int)showMessageBoxDirect(2, 0xe, &v);
            o->state = 1;
        } else {
            if (GCD.bd077 == 0) {
                o->d90 = (int)showMessageBoxDirect(2, 10, &v);
                GCD.bd077 = 1;
                o->state = 1;
            } else {
                o->d90 = (int)showMessageBoxDirect(2, 0xb, &v);
                o->state = 3;
            }
        }
        break;
    case 1:
        AnimAdvance(o);
        if (((TObj *)o->d90)->b04 != 2)
            break;
        ((TObj *)o->d90)->b04 = 3;
        DAT_8009c942[0] = 0;
        PLAYER.wb2 = 0;
        o->timer = 4;
        D_1F8001C6 = 0;
        o->state = 2;
        break;
    case 2:
        AnimAdvance(o);
        if (--o->timer != 0)
            o->state = 9;
        break;
    case 3:
        AnimAdvance(o);
        if (((TObj *)o->d90)->b04 != 2)
            break;
        ((TObj *)o->d90)->b04 = 3;
        DAT_8009c942[0] = 0;
        PLAYER.wb2 = 0;
        o->timer = 4;
        D_1F8001C6 = 0;
        if (((unsigned char*)&GCD.bcded)[0] == 0) {
            setEventStarted(0x49, 1, 1);
            o->timer = 200;
        }
        o->state = 2;
        break;
    case 9:
        D_1F8001C6 = 0;
        DAT_8009c942[0] = 0;
        PLAYER.wb2 = 0;
        o->anim = D_80118D34[o->subtype].p[0];
        AnimJump(o, 0);
        o->animFrame = o->wbc;
        PLAYER.b04 = 1;
        PLAYER.step = 0;
        PLAYER.state = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
