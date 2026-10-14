// FUNC 80115fb4 640 X013
// MATCHING 80115fb4 640
#include "TOBJ.H"
#include "gamestate.h"
typedef void (*Fn)(TObj *);

extern Fn D_80079A98[];
extern unsigned char D_8009C93A, D_800A60DA, D_8009C93C;
extern unsigned char D_800A603C, D_800A6039, D_800A603D, D_800A603E;
extern short D_800A60B6;
extern short D_1F800172;
extern int D_1F8000F0;
extern TObj *TASK_C;
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
        if (D_8009C93A != 0) o->step++;
        FUN_80027c74(o);
        func_8002795C(o);
        syncObjectTargetY(o);
        func_80029078(o);
        GAME.selectedSpawnPoint = 0;
        break;
    case 2:
        D_80079A98[o->subtype](o);
        if (D_800A60DA == 2) {
            stepPlayerXToward(0xa0);
            if (D_1F800172 >= 0x50) {
                D_800A603C = 5;
                D_800A6039 = 0;
                D_800A603D = 0x41;
                D_800A603E = 0;
                o->step++;
            }
        }
        break;
    case 3:
        o->w48 = 0x2d;
        o->step++;
        D_800A60B6 = 0;
        break;
    case 4:
        if (--o->w48 == -1) {
            (*(short*)&GAME.nextArea) = 1;
            (*(short*)&GAME.nextSection) = 4;
            (*(short*)&GAME.nextSpawnPoint) = 4;
            D_8009C93C = 0;
            GAME.fadeScreenControl = 3;
            o->step++;
        }
    case 5:
        {
            short *k = &D_800A60B6;
            *k += 0x20;
            if (*k > 0x500) *k = 0x500;
            { int *g = &D_1F8000F0; *g += *k << 8; }
        }
        if (GAME.fadeScreenControl == 1) {
            TASK_C->w4c = 7;
            TASK_C->w4e = 0;
        }
        break;
    }
}
