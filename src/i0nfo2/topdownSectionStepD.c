// FUNC 800e8ed4 364 I0NFO2
// MATCHING 800e8ed4 364
#include "TOBJ.H"
extern void func_800270A0(TObj *o, int a, int b);
extern void func_80027810(TObj *o);
extern void func_8002A4D0(TObj *o);
extern void (*D_80079AD0[])(TObj *o);
extern unsigned char D_8009C93A;
extern short PLAYER_X;
extern unsigned short PLAYER_Y;

void topdownSectionStepD(TObj *o)
{
    switch (o->step) {
    case 0:
        if (D_8009C93A != 0) o->step++;
        func_80027810(o);
        break;
    case 1:
        if (PLAYER_X < -0x74) {
            if ((unsigned)(PLAYER_Y - 0x208) < 0xA) func_800270A0(o, 1, 0);
        } else if (PLAYER_X > 0x93) {
            if ((short)PLAYER_Y < 0x4A) func_800270A0(o, 1, 2);
        } else if ((unsigned short)(PLAYER_X - 0x6B) < 8) {
            if ((unsigned)(PLAYER_Y - 0x13F) < 2) func_800270A0(o, 1, 1);
        }
        D_80079AD0[o->subtype](o);
        break;
    case 2:
        func_8002A4D0(o);
        break;
    }
}
