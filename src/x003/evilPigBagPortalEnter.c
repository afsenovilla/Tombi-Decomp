// FUNC 8011d59c 688 X003
// MATCHING 8011d59c 688
#include "camera.h"
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"
#include "gametask.h"
extern unsigned char PLAYER[];
extern unsigned char D_800A6039[];
extern unsigned char D_800A603C[];
extern unsigned char D_800A603D[];
extern unsigned char D_800A4553[];
extern int D_1F800190;
extern int D_1F80018C;
extern unsigned char D_8009E375[], D_8009E376[], D_8009E377[];
extern unsigned char D_8009F085[], D_8009F086[], D_8009F087[];
extern int evilPigBagPortalCheckPlayer(TObj *);

void evilPigBagPortalEnter(TObj *o)
{
    switch (o->state) {
    case 0:
        if (o->visible && evilPigBagPortalCheckPlayer(o)) {
            o->state++;
            PLAYER[0] = 2;
            ((unsigned char*)&GVARS.b3f)[0] = 1;
            ((unsigned char*)&GVARS.b3e)[0] = 1;
            ((unsigned char*)&GVARS.b42)[0] = 1;
            D_800A4553[0] = 3;
            CAMERA_SCROLL.velY = 0;
            D_1F800190 = o->y.raw;
            D_1F80018C = o->h->raw;
            o->timer = 10;
        }
        break;
    case 1:
        if (--o->timer == -1) {
            PLAYER[0] = 5;
            D_800A603C[0] = 5;
            ((unsigned char*)&GVARS.b3e)[0] = 0;
            D_800A6039[0] = 0;
            D_800A603D[0] = 0x40;
            o->timer = 0x14;
            o->state++;
            (*(short*)&GAME.nextArea) = 0xe;
            (*(short*)&GAME.nextSection) = 6;
            (*(short*)&GAME.nextSpawnPoint) = 0;
        }
        break;
    case 2:
        if (o->timer == 0) {
            ((unsigned char*)&GAME.fadeScreenControl)[0] = 3;
            GVARS.b3c = 1;
            SPAD.b3d1 = 1;
            o->state++;
        } else {
            o->timer--;
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
