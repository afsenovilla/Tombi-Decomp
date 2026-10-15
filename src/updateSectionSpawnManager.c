// FUNC 8004c1ec 556 MAIN0
// MATCHING 8004c1ec 556
#include "spad.h"
#include "gcd.h"
#include "gamestate.h"
typedef struct {
    unsigned char b0, b1, b2, b3, b4, b5, b6, b7;
    short w08;
    short w0a;
    unsigned short w0c;
    unsigned char b0e;
    signed char b0f;
} S;
extern unsigned short D_8009C962s;
extern unsigned short D_80078792;
extern int APD_DATA[];
extern void spawnSectionObjects(S *);
extern void spawnScrollBackObjects(S *);
extern void despawnOtherSectionObjects(S *);
extern void spawnSectionObjectsWithPrevious(S *);
extern void PoolFree_1F800210(S *);

void updateSectionSpawnManager(S *o)
{
    unsigned char t = o->b4;

    switch (t) {
    case 0:
        o->w0c = ((unsigned short*)&GAME.selectedArea)[0];
        o->b0e = ((unsigned short*)&GAME.selectedSection)[0];
        o->b4++;
        o->b5 = 0;
        if (((unsigned short*)&GAME.selectedArea)[0] == 1 && !(GAME.purifiedAreas & 1) && GCD.bcdb5 == 0 && D_8009C962s < 2) {
            APD_DATA[0] = SPAD.i370;
            SPAD.i35c = SPAD.i374;
            APD_DATA[D_80078792] = SPAD.i378;
        }
        spawnSectionObjects(o);
        spawnScrollBackObjects(o);
        break;
    case 1:
        switch (o->b5) {
        case 0:
            o->b0f = o->b0e - *(unsigned char *)((unsigned short*)&GAME.selectedSection);
            if (o->b0f == 0) break;
            if (o->b0f < 0) o->b0f = 0;
            o->b0e = ((unsigned short*)&GAME.selectedSection)[0];
            o->w08 = 2;
            o->b5++;
            despawnOtherSectionObjects(o);
            break;
        case 1:
            if (--o->w08 == 0) {
                o->b5--;
                if (o->b0f == 0) {
                    spawnSectionObjects(o);
                } else {
                    spawnSectionObjectsWithPrevious(o);
                }
            }
            break;
        }
        break;
    case 2:
        o->b4 = t - 1;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
