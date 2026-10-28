// FUNC 801163b8 516 X006
// MATCHING 801163b8 516
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"
#include "gametask.h"
typedef struct { char pad[0x2c]; unsigned short w2c; } PL;
typedef struct { char pad[0x4c]; short w4c; short w4e; } SC;
extern void scrollCameraDepthStep(TObj *);
extern void setupCameraMatrixFromObj(TObj *);
extern void spawnRaceMessage(int, int, int);
extern void playSFX(int);

void a06SectionScrollStep(TObj *o)
{
    PL *p = (*(PL **)&GVARS.p4c);

    switch (o->step) {
    case 0:
        o->step++;
        scrollCameraDepthStep(o);
        break;
    case 1:
        if (GVARS.playerLanded != 0) {
            o->step++;
        }
    call:
        scrollCameraDepthStep(o);
        break;
    case 2:
        if (p->w2c >= 0xb0) {
            o->step++;
        }
        goto call;
    case 3:
        if ((*(int *)&o->velX += 0x100) >= 0) {
            *(int *)&o->velX = 0;
            o->step++;
        }
        goto call;
    case 4:
        if (p->w2c >= 0xc1) {
            spawnRaceMessage(5, 100, 0x46);
            playSFX(0x103);
            o->step++;
            GVARS.b3f = 1;
            GVARS.b3e = 1;
            GVARS.b58 = 3;
        }
        goto call;
    case 5:
        if (p->w2c >= 0xc8) {
            GVARS.b58 = 0;
            o->w4a = 0x6e;
            o->step++;
        }
        goto call;
    case 6:
        if (--o->w4a == -1) {
            o->step++;
            (*(short*)&GAME.nextArea) = 6;
            (*(short*)&GAME.nextSection) = 1;
            (*(short*)&GAME.nextSpawnPoint) = 0;
            GVARS.fadeEffect = 0;
            GAME.fadeScreenControl = 3;
        }
        goto call;
    case 7:
        if (GAME.fadeScreenControl == 1) {
            TASK_C->mode = 7;
            TASK_C->step = 0;
        }
        break;
    default:
        return;
    }
    setupCameraMatrixFromObj(o);
}
