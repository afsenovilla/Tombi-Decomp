// FUNC 80115fb4 640 X013
// MATCHING 80115fb4 640
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
#include "gamestate.h"
#include "gametask.h"
typedef void (*Fn)(TObj *);

extern Fn D_80079A98[];
extern short D_1F800172;
extern int D_1F8000F0;
extern unsigned int stepScrollReturnY(TObj *);
extern unsigned int stepScrollClearX(TObj *);
extern void FUN_80027c74(TObj *);
extern void func_8002795C(TObj *);
extern void syncObjectTargetY(TObj *);
extern void func_80029078(TObj *);
extern int stepPlayerXToward(int);

void func_80115FB4(TObj *o)
{
    switch (o->step) {
    case 0:
        if (((unsigned char*)&GAME.disableSelectMenu)[0] != 0) {
            if (stepScrollReturnY(o) & stepScrollClearX(o)) ((unsigned char*)&GAME.disableSelectMenu)[0] = 0;
            break;
        }
        o->subtype = 0;
        o->step++;
        break;
    case 1:
        if (GVARS.b3a != 0) o->step++;
        FUN_80027c74(o);
        func_8002795C(o);
        syncObjectTargetY(o);
        func_80029078(o);
        GAME.selectedSpawnPoint = 0;
        break;
    case 2:
        D_80079A98[o->subtype](o);
        if ((*(unsigned char*)((char*)&PLAYER.obj.da0 + 2)) == 2) {
            stepPlayerXToward(0xa0);
            if (D_1F800172 >= 0x50) {
                PLAYER.obj.b04 = 5;
                PLAYER.obj.visible = 0;
                PLAYER.obj.step = 0x41;
                PLAYER.obj.state = 0;
                o->step++;
            }
        }
        break;
    case 3:
        o->w48 = 0x2d;
        o->step++;
        PLAYER.obj.velY = 0;
        break;
    case 4:
        if (--o->w48 == -1) {
            (*(short*)&GAME.nextArea) = 1;
            (*(short*)&GAME.nextSection) = 4;
            (*(short*)&GAME.nextSpawnPoint) = 4;
            GVARS.b3c = 0;
            GAME.fadeScreenControl = 3;
            o->step++;
        }
    case 5:
        {
            short *k = &PLAYER.obj.velY;
            *k += 0x20;
            if (*k > 0x500) *k = 0x500;
            { int *g = &D_1F8000F0; *g += *k << 8; }
        }
        if (GAME.fadeScreenControl == 1) {
            TASK_C->mode = 7;
            TASK_C->step = 0;
        }
        break;
    }
}
