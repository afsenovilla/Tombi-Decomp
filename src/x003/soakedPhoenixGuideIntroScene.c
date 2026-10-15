// FUNC 80131e44 424 X003
// MATCHING 80131e44 424
#include "gcd.h"
#include "gv2.h"
#include "gvars.h"
#include "TOBJ.H"

extern TObj PLAYER;
extern short *D_800A6078;
extern int SCRIPT_OBJECTS;
extern int D_8009F2DC;
void npcSetAnim(int a, int b, int c);

void soakedPhoenixGuideIntroScene(TObj *o)
{
    int a = SCRIPT_OBJECTS;
    int b = D_8009F2DC;

    switch (o->state) {
    case 0:
        if (GVARS.b3a != 0 && D_800A6078[1] >= 0x1b9) {
            GVARS.b3f = 1;
            GVARS.b42 = 1;
            PLAYER.active = 1;
            ((unsigned char*)&GV2.bb0)[0] = 0;
            D_800A6078[1] = 0x1b8;
            PLAYER.animFrame = 0;
            PLAYER.b04 = 5;
            PLAYER.step = 0;
            PLAYER.state = 0;
            o->state++;
        }
        break;
    case 1:
        if (PLAYER.b69) {
            GCD.bceef = 1;
            npcSetAnim(a, 0, 0);
            npcSetAnim(b, 0, 0);
            o->state++;
        }
        break;
    case 2:
        if (GCD.bceef == 3) {
            o->w08 = 0x3c;
            o->state++;
        }
        break;
    case 3:
        if (--o->w08 <= 0) {
            GCD.bceef = 4;
            o->step = 1;
            o->state = 0;
        }
        break;
    }
}
