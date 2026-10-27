// FUNC 800e8d98 316 I0NFO2
// MATCHING 800e8d98 316
#include "TOBJ.H"
extern void func_800270A0(TObj *o, int a, int b);
extern void func_80027810(TObj *o);
extern void func_8002A4D0(TObj *o);
extern void (*D_80079AD0[])(TObj *o);
extern unsigned char GVARS_PLAYER_LANDED;
extern short PLAYER_X;
extern unsigned short PLAYER_Y;

void topdownSectionStepC(TObj *o)
{
    switch (o->step) {
    case 0:
        if (GVARS_PLAYER_LANDED != 0) o->step++;
        func_80027810(o);
        break;
    case 1:
        if (PLAYER_X > 0x22B) {
            if ((unsigned)(PLAYER_Y - 0x169) < 0x64) func_800270A0(o, 1, 0);
        } else if ((short)PLAYER_Y < 0x4F) {
            if ((unsigned short)(PLAYER_X - 0xC7) < 0x41) func_800270A0(o, 1, 1);
        }
        D_80079AD0[o->subtype](o);
        break;
    case 2:
        func_8002A4D0(o);
        break;
    }
}
