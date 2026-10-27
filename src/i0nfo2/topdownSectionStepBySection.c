// FUNC 800e9174 396 I0NFO2
// MATCHING 800e9174 396
#include "TOBJ.H"
extern void func_8002A4D0(TObj *o);
extern void func_80027810(TObj *o);
extern void func_800270A0(TObj *o, int a, int b);
extern void (*D_80079AD0[])(TObj *o);
extern unsigned char D_8009C93A;
extern unsigned short GAME_SELECTED_SECTION;
extern short PLAYER_X;
extern short PLAYER_Y;

void topdownSectionStepBySection(TObj *o)
{
    unsigned char s = o->step;
    switch (s) {
    case 0:
        if (D_8009C93A != 0) {
            o->step = s + 1;
        }
        func_80027810(o);
        break;
    case 1:
        switch (GAME_SELECTED_SECTION) {
        case 0:
            if (PLAYER_X < -0x70) {
                if ((unsigned int)((unsigned short)PLAYER_Y - 0x138) < 0x21) {
                    func_800270A0(o, 1, 0);
                }
            } else if (PLAYER_X >= 0x119 && (unsigned int)((unsigned short)PLAYER_Y - 0x100) < 0x77) {
                func_800270A0(o, 1, 1);
            }
            break;
        case 2:
            if (PLAYER_Y >= 0x1C8 && (unsigned int)((unsigned short)PLAYER_X - 0x2A) < 0x10) {
                func_800270A0(o, 1, 0);
            }
            break;
        }
        D_80079AD0[o->subtype](o);
        break;
    case 2:
        func_8002A4D0(o);
        break;
    }
}
