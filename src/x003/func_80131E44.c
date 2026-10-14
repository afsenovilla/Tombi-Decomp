// FUNC 80131e44 424 X003
// MATCHING 80131e44 424
#include "gvars.h"
#include "TOBJ.H"

extern TObj PLAYER;
extern short *D_800A6078;
extern int SCRIPT_OBJECTS;
extern int D_8009F2DC;
extern unsigned char D_8009D2B0[];
extern unsigned char D_8009CEEF;
void func_801289C8(int a, int b, int c);

void func_80131E44(TObj *o)
{
    int a = SCRIPT_OBJECTS;
    int b = D_8009F2DC;

    switch (o->state) {
    case 0:
        if (GVARS.b3a != 0 && D_800A6078[1] >= 0x1b9) {
            GVARS.b3f = 1;
            GVARS.b42 = 1;
            PLAYER.active = 1;
            D_8009D2B0[0] = 0;
            D_800A6078[1] = 0x1b8;
            PLAYER.animFrame = 0;
            PLAYER.b04 = 5;
            PLAYER.step = 0;
            PLAYER.state = 0;
            o->state++;
        }
        break;
    case 1:
        if (PLAYER.b69) {
            D_8009CEEF = 1;
            func_801289C8(a, 0, 0);
            func_801289C8(b, 0, 0);
            o->state++;
        }
        break;
    case 2:
        if (D_8009CEEF == 3) {
            o->w08 = 0x3c;
            o->state++;
        }
        break;
    case 3:
        if (--o->w08 <= 0) {
            D_8009CEEF = 4;
            o->step = 1;
            o->state = 0;
        }
        break;
    }
}
