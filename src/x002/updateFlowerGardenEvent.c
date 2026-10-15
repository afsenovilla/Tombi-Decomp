// FUNC 80119914 264 X002
// MATCHING 80119914 264
#include "gcd.h"
#include "TOBJ.H"
#include "gamestate.h"
typedef struct { short xf, x, yf, y, zf, z; } V6;
extern unsigned int D_8009C98C;
extern TObj *ObjSpawnType0(int, int, V6 *);
extern void flowerGardenCompleteScene(TObj *);
extern void PoolFree_1F800210(TObj *);

void updateFlowerGardenEvent(TObj *o)
{
    V6 v;

    switch (o->b04) {
    case 0:
        if (GAME.playerAP >= D_8009C98C && GCD.bcdcb != 0 && GAME.goldenFlowerState == 0) {
            v.x = 0xb0;
            v.y = -0x38;
            v.z = 0;
            ObjSpawnType0(0x5f, 0, &v);
        }
        o->b04++;
        break;
    case 1:
        flowerGardenCompleteScene(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
