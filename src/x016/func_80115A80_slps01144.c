// VERSION SLPS_01144 x16 80115A80 680
// JP near-twin of PAL x002 80117644 (evilPigBagPortalEnter.c): same code shape, without the `SPAD.b3d1 = 1` store of
// case 2 (8 bytes shorter) and with the JP addresses (GVARS 0x8009E4D8, GAME 0x8009E508, PLAYER 0x800A7B78).
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"
#include "gametask.h"
#include "player.h"

typedef struct { char pad[0x18]; int velY; } CamScroll;   /* D_800A60B0: the JP CAMERA_SCROLL.velY */
extern CamScroll D_800A6098;
extern unsigned char D_800A609B[];                        /* PAL D_800A4553 */
extern int CAMERA_TARGET_Y_FIX, CAMERA_TARGET_H_FIX;      /* scratchpad 0x1F800190 / 0x1F80018C */
extern unsigned char PLAYER_STATE[], PLAYER_SUBSTATE[];   /* D_800A7B7C / D_800A7B7D */
extern unsigned char DISPLAY_BUFFER_0_BG_R[], DISPLAY_BUFFER_0_BG_G[], DISPLAY_BUFFER_0_BG_B[];  /* D_8009FF05..07 */
extern unsigned char DISPLAY_BUFFER_1_BG_R[], DISPLAY_BUFFER_1_BG_G[], DISPLAY_BUFFER_1_BG_B[];  /* D_800A0C15..17 */
extern int func_801157DC_slps01144(TObj *);               /* the JP evilPigBagPortalCheckPlayer */

void func_80115A80_slps01144(TObj *o)
{
    short t;

    switch (o->state) {
    case 0:
        if (o->visible == 0) break;
        if (func_801157DC_slps01144(o) == 0) break;
        o->state++;
        PLAYER.obj.active = 2;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        ((unsigned char*)&GVARS.b3e)[0] = 1;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        D_800A609B[0] = 3;
        D_800A6098.velY = 0;
        CAMERA_TARGET_Y_FIX = o->y.raw;
        CAMERA_TARGET_H_FIX = o->h->raw;
        o->timer = 10;
        break;
    case 1:
        if (--o->timer == -1) {
            PLAYER.obj.active = 5;
            PLAYER_STATE[0] = 5;
            ((unsigned char*)&GVARS.b3e)[0] = 0;
            PLAYER.obj.visible = 0;
            PLAYER_SUBSTATE[0] = 0x40;
            o->timer = 0x14;
            o->state++;
            ((short*)&GAME.nextArea)[0] = 0xe;
            ((short*)&GAME.nextSection)[0] = 2;
            ((short*)&GAME.nextSpawnPoint)[0] = 0;
        }
        break;
    case 2:
        t = o->timer;
        if (t == 0) {
            ((unsigned char*)&GAME.fadeScreenControl)[0] = 3;
            ((unsigned char*)&GVARS.fadeEffect)[0] = 1;
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
#endif
