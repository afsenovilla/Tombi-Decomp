// FUNC 80118b8c 224 X017
// MATCHING 80118b8c 224
#include "gcd.h"
#include "TOBJ.H"
extern TObj PLAYER;
extern TObj *PLAYER_EXT;
extern void spawnNpcSet12(TObj *);
extern void FUN_80018980(TObj *);

void updateNpcSet12(TObj *o)
{
    switch (o->b04) {
    case 0:
        spawnNpcSet12(o);
        o->b04++;
        break;
    case 1:
        if ((unsigned short)(PLAYER.h->p.whole - 0x100) < 0x90 && GCD.bcfd6 == 0)
            *((unsigned char *)PLAYER_EXT + 0x1e) = 0;
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        FUN_80018980(o);
        break;
    }
}
