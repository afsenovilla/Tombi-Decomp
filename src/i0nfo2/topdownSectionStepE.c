// FUNC 800e9040 308 I0NFO2
// MATCHING 800e9040 308
#include "TOBJ.H"
extern void func_800270A0(TObj *o, int a, int b);
extern void func_80027810(TObj *o);
extern void func_8002A4D0(TObj *o);
extern void (*D_80079AD0[])(TObj *o);
extern unsigned char GVARS_PLAYER_LANDED;
extern unsigned short PLAYER_X;
extern short PLAYER_Y;

void topdownSectionStepE(TObj *o)
{
    switch (o->step) {
    case 0:
        if (GVARS_PLAYER_LANDED != 0) o->step++;
        func_80027810(o);
        break;
    case 1:
        if (PLAYER_Y > 0x23A) {
            if ((unsigned)(PLAYER_X - 0xA7) < 0x71) func_800270A0(o, 1, 0);
        } else if (PLAYER_Y < 0x50) {
            if ((unsigned)(PLAYER_X - 0xB0) < 0x60) func_800270A0(o, 1, 1);
        }
        D_80079AD0[o->subtype](o);
        break;
    case 2:
        func_8002A4D0(o);
        break;
    }
}
