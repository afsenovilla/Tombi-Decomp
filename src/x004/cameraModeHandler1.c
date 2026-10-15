// FUNC 80116934 428 X004
// MATCHING 80116934 428
#include "gvars.h"
#include "gamestate.h"
typedef struct { char p[6]; unsigned char b6, b7; } E8;
typedef struct O {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x24 - 8];
    void *anim;
    char p28[0x3f - 0x28];
    unsigned char b3f;
    char p40[0x68 - 0x40];
    E8 *p68;
    char p6c[0x7c - 0x6c];
    int d7c;
} O;

extern short D_1F8001C8;
extern void updateCameraFollow(O *);
extern unsigned int stepScrollReturnY(O *);
extern unsigned int FUN_80028a80(O *);
extern int cameraModeHandler1Step(O *);
extern void setPlayerAxisPointers(void);

void cameraModeHandler1(O *o)
{
    if (GVARS.b38) {
        updateCameraFollow(o);
        return;
    }
    switch (o->state) {
    case 0:
        if (stepScrollReturnY(o) & FUN_80028a80(o)) {
            o->substep = 0;
            o->state++;
        }
        break;
    case 1:
        o->state++;
        GAME.disableSelectMenu = 1;
        break;
    case 2:
        if (cameraModeHandler1Step(o)) o->state++;
        break;
    case 3:
        GAME.disableSelectMenu = 0;
        if (o->b3f == 0) D_1F8001C8 = (D_1F8001C8 + 1) & 1;
        else D_1F8001C8 = (D_1F8001C8 - 1) & 1;
        setPlayerAxisPointers();
        o->d7c = D_1F8001C8 * 0x5a00;
        o->anim = 0;
        o->subtype = 0;
        o->state = 0;
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((unsigned short*)&GAME.selectedSection)[0] = o->p68->b6;
        ((unsigned short*)&GAME.selectedSpawnPoint)[0] = o->p68->b7;
        break;
    }
}
