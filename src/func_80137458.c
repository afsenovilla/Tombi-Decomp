// FUNC 80137458 864 X000
// MATCHING 80137458 864
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
#include "gamestate.h"
extern unsigned char D_8009CE4A;
extern short *D_800A6078s;
extern signed char D_8009D2B0;
extern unsigned short D_1F8003C4;
extern unsigned short D_1F8001FC;
extern unsigned char D_800A60F8;
extern unsigned char D_8009D0A7;
extern unsigned char D_8009CEBD;
extern unsigned char *D_800A611C;
extern unsigned char D_8009D00D;
extern unsigned char D_8009CFFB;

short func_80137458(TObj *o)
{
    short n = 0;
    unsigned char **p = (unsigned char **)((char *)o + 0x1c);

    if (GAME.selectedSection == 0) {
        if (D_8009CE4A != 0xff) {
            if (PLAYER.obj.y.p.whole < -0x122 && PLAYER.obj.b69 != 0 && (unsigned short)((*(unsigned short **)&PLAYER.obj.h)[1] - 0x148) < 0x10 &&
                D_8009D2B0 == 1 && (D_1F8001FC & D_1F8003C4) != 0 && GVARS.b40 == 0) {
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
                        if (GVARS.b40 == 0)
                            return n;
                        if (GVARS.b41 == 3) {
                            ((unsigned char*)&GVARS.b42)[0] = 1;
                            n++;
                            D_800A60F8 = 1;
                            *(short *)&o->anim = 0;
                        }
                    }
                } else {
                    if (PLAYER.obj.y.p.whole < -0x110 && D_800A6078s[1] < 0x17c && PLAYER.obj.b69 != 0) {
                        ((unsigned char*)&GVARS.b42)[0] = 0;
                        D_800A60F8 = 0;
                        *(short *)&o->anim = 0;
                        n++;
                        if (GVARS.b40 != 0 && GVARS.b41 == 3) {
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
                        if (D_800A611C[2] == 0x15) {
                            n++;
                            ((unsigned char*)&GVARS.b42)[0] = 0;
                            D_800A60F8 = 0;
                            *p = D_800A611C;
                            *(short *)&o->anim = 0;
                        }
                    }
                } else {
                    if (PLAYER.obj.y.p.whole < -0x110 && D_800A6078s[1] < 0x17c && PLAYER.obj.b69 != 0) {
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
            if ((GAME.area00_eventControl & 0x40) == 0 || D_8009CFFB != 0)
                n++;
            if (n == 0)
                o->b04 = 2;
        }
    }
    return n;
}
