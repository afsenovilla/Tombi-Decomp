// FUNC 80120780 684 X001
// MATCHING 80120780 684
#include "camera.h"
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"
#include "gametask.h"

typedef struct { char pad[0x4c]; short w4c; short w4e; } SC;
extern TObj PLAYER;
extern unsigned char D_800A4553[];
extern int D_1F800190, D_1F80018C;
extern unsigned char PLAYER_STATE[], PLAYER_SUBSTATE[];
extern unsigned char D_8009E375[], D_8009E376[], D_8009E377[];
extern unsigned char D_8009F085[], D_8009F086[], D_8009F087[];
extern int evilPigBagPortalCheckPlayer(TObj *);

void evilPigBagPortalEnter(TObj *o)
{
    short t;

    switch (o->state) {
    case 0:
        if (o->visible == 0) break;
        if (evilPigBagPortalCheckPlayer(o) == 0) break;
        o->state++;
        PLAYER.active = 2;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        ((unsigned char*)&GVARS.b3e)[0] = 1;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        D_800A4553[0] = 3;
        CAMERA_SCROLL.velY = 0;
        D_1F800190 = o->y.raw;
        D_1F80018C = o->h->raw;
        o->timer = 10;
        break;
    case 1:
        if (--o->timer == -1) {
            PLAYER.active = 5;
            PLAYER_STATE[0] = 5;
            ((unsigned char*)&GVARS.b3e)[0] = 0;
            PLAYER.visible = 0;
            PLAYER_SUBSTATE[0] = 0x40;
            o->timer = 0x14;
            o->state++;
            ((short*)&GAME.nextArea)[0] = 0xe;
            ((short*)&GAME.nextSection)[0] = 0;
            ((short*)&GAME.nextSpawnPoint)[0] = 0;
        }
        break;
    case 2:
        t = o->timer;
        if (t == 0) {
            ((unsigned char*)&GAME.fadeScreenControl)[0] = 3;
            ((unsigned char*)&GVARS.b3c)[0] = 1;
            ((unsigned char*)&SPAD.b3d1)[0] = 1;
            o->state++;
        } else {
            o->timer = t - 1;
        }
        o->w74 += 0x80;
        if (o->w74 > 0x4000) {
            o->w74 = 0x4000;
            o->state++;
        }
        o->w76 = o->w74;
        o->w78 = o->w74;
        break;
    case 3:
        o->w74 += 0x80;
        if (o->w74 > 0x4000) {
            o->w74 = 0x4000;
        }
        o->w76 = o->w74;
        o->w78 = o->w74;
        if (((unsigned char*)&GAME.fadeScreenControl)[0] == 1) {
            TASK_C->mode = 7;
            TASK_C->step = 0;
            D_8009E375[0] = 0xff;
            D_8009E376[0] = 0xff;
            D_8009E377[0] = 0xff;
            D_8009F085[0] = 0xff;
            D_8009F086[0] = 0xff;
            D_8009F087[0] = 0xff;
        }
        break;
    }
}
