// VERSION BETA_980203 x00 8013ABC4 824
#if defined(VERSION_BETA_980203)
// Beta version: USA C of src/charlesTalkCheck.c: no GAME_PLAYER_MODE test, talk mask at 0x3c8 (D_1F8003C8), event-control test alone.
#include "plx.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
#include "gamestate.h"
extern unsigned char D_8009CE4A;
extern short *PLAYER_H;
extern signed char GAME_PLAYER_MODE;
extern unsigned short D_1F8003C8; /* the talk button mask (PAL BUTTON_MASK_A at 0x3c4) */
extern unsigned short JOYPAD_STATE;
extern unsigned char D_800A60F8;
extern unsigned char D_8009D0A7;
extern unsigned char D_8009CEBD;
extern unsigned char D_8009D00D;
extern unsigned char D_8009CFFB;

short charlesTalkCheck(TObj *o)
{
    short n = 0;
    unsigned char **p = (unsigned char **)((char *)o + 0x1c);

    if (GAME.selectedSection == 0) {
        if (D_8009CE4A != 0xff) {
            if (PLAYER.obj.y.p.whole < -0x122 && PLAYER.obj.b69 != 0 && (unsigned short)((*(unsigned short **)&PLAYER.obj.h)[1] - 0x148) < 0x10 &&
                (JOYPAD_STATE & D_1F8003C8) != 0 && GVARS.useItemPending == 0) {
                D_800A60F8 = 1;
                ((unsigned char*)&GVARS.b42)[0] = 0;
                *(short *)&o->anim = 1;
                n = 1;
            }
            if (D_8009D0A7 != 0) {
                if (D_8009CEBD == 0) {
                    if (PLAYER.obj.y.p.whole < -0x110) {
                        if ((*(unsigned char*)&PLAYER.obj.wac) == 3)
                            goto l6ac;
                        if (GVARS.useItemPending == 0)
                            return n;
                        if (GVARS.useItemId == 3) {
                            ((unsigned char*)&GVARS.b42)[0] = 1;
                            n++;
                            D_800A60F8 = 1;
                            *(short *)&o->anim = 0;
                        }
                    }
                } else {
                    if (PLAYER.obj.y.p.whole < -0x110 && PLAYER_H[1] < 0x17c && PLAYER.obj.b69 != 0) {
                        ((unsigned char*)&GVARS.b42)[0] = 0;
                        D_800A60F8 = 0;
                        *(short *)&o->anim = 0;
                        n++;
                        if (GVARS.useItemPending != 0 && GVARS.useItemId == 3) {
                            ((unsigned char*)&GVARS.b42)[0] = 1;
                            D_800A60F8 = 1;
                            *(short *)&o->anim = 0;
                        }
                    }
                }
            } else {
                if (D_8009CEBD == 0) {
                    if (PLAYER.obj.y.p.whole < -0x110 && (*(unsigned char*)&PLAYER.obj.wac) == 3) {
                    l6ac:
                        if ((*(unsigned char **)&PLX.p611c)[2] == 0x15) {
                            n++;
                            ((unsigned char*)&GVARS.b42)[0] = 0;
                            D_800A60F8 = 0;
                            *p = (*(unsigned char **)&PLX.p611c);
                            *(short *)&o->anim = 0;
                        }
                    }
                } else {
                    if (PLAYER.obj.y.p.whole < -0x110 && PLAYER_H[1] < 0x17c && PLAYER.obj.b69 != 0) {
                        n++;
                        ((unsigned char*)&GVARS.b42)[0] = 0;
                        D_800A60F8 = 0;
                        *(short *)&o->anim = 0;
                    }
                }
            }
        } else {
            if (D_8009D00D != 0)
                n = 1;
            if ((GAME.area00_eventControl & 0x40) == 0)
                n++;
            if (n == 0)
                o->b04 = 2;
        }
    }
    return n;
}
#endif
