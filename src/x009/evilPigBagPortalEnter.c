// FUNC 8011cad4 688 X009
// MATCHING 8011cad4 688
#include "camera.h"
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"
#include "gametask.h"

typedef struct { char pad[0x4c]; short w4c; short w4e; } SC;
extern TObj PLAYER;
extern unsigned char D_800A4553[];
extern int CAMERA_TARGET_Y_FIX, CAMERA_TARGET_H_FIX;
extern unsigned char PLAYER_STATE[], PLAYER_SUBSTATE[];
extern unsigned char DISPLAY_BUFFER_0_BG_R[], DISPLAY_BUFFER_0_BG_G[], DISPLAY_BUFFER_0_BG_B[];
extern unsigned char DISPLAY_BUFFER_1_BG_R[], DISPLAY_BUFFER_1_BG_G[], DISPLAY_BUFFER_1_BG_B[];
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
        CAMERA_TARGET_Y_FIX = o->y.raw;
        CAMERA_TARGET_H_FIX = o->h->raw;
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
            ((short*)&GAME.nextSection)[0] = 5;
            ((short*)&GAME.nextSpawnPoint)[0] = 0;
        }
        break;
    case 2:
        t = o->timer;
        if (t == 0) {
            ((unsigned char*)&GAME.fadeScreenControl)[0] = 3;
            ((unsigned char*)&GVARS.fadeEffect)[0] = 1;
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
            DISPLAY_BUFFER_0_BG_R[0] = 0xff;
            DISPLAY_BUFFER_0_BG_G[0] = 0xff;
            DISPLAY_BUFFER_0_BG_B[0] = 0xff;
            DISPLAY_BUFFER_1_BG_R[0] = 0xff;
            DISPLAY_BUFFER_1_BG_G[0] = 0xff;
            DISPLAY_BUFFER_1_BG_B[0] = 0xff;
        }
        break;
    }
}
