// FUNC 800ef388 216 X011
// MATCHING 800ef388 216
#include "TOBJ.H"
extern void func_8001e5f4(int a, int b);
extern short func_8003fd78(TObj *o, int a, int b);
extern unsigned char *PLAYER_EXT;
extern unsigned char *GRABBED_OBJECT;

void playerResetOnLanding(TObj *o)
{
    if (o->b69 != 0) {
        func_8001e5f4(0x1c, 0x7f);
        PLAYER_EXT[8] = 0;
        o->ba7 = 0;
        *(char *)&o->wac = 0;
        o->b9c = 0;
        o->b9e = 0;
        *(char *)&o->waa = 0;
        o->step = 0;
        o->state = 0;
    }
    if (GRABBED_OBJECT != 0 && GRABBED_OBJECT[2] == 0x14 && func_8003fd78(o, 0, 0) != 0) {
        func_8001e5f4(0x1c, 0x7f);
        PLAYER_EXT[8] = 0;
        o->ba7 = 0;
        *(char *)&o->wac = 0;
        o->b9c = 0;
        o->b9e = 0;
        *(char *)&o->waa = 0;
        o->step = 0;
        o->state = 0;
    }
}
