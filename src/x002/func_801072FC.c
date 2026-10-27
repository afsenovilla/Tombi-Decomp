// FUNC 801072fc 196 X002
// MATCHING 801072fc 196
#include "TOBJ.H"
extern unsigned char *PLAYER_EXT;
extern char MOVE_TABLE_2[];
void func_801072FC(TObj *o)
{
    ((unsigned char *)o)[0xc7] = 1;
    o->b9d = 0;
    ((unsigned char *)o)[0xc6] = 0;
    ((unsigned char *)o)[0xe3] = 0;
    PLAYER_EXT[0] = 0;
    o->movetab = MOVE_TABLE_2;
    *(unsigned char *)&o->wac = 0;
    o->b6b = 0;
    o->ba4 = 0;
    o->d84 = 0;
    o->d88 = 0;
    o->d8c = 0;
    PLAYER_EXT[8] = o->bbe & 1;
    if (PLAYER_EXT[8] == 0) {
        if (o->wb2 < -0x144) o->wb2 = -0x144;
    } else {
        if (o->wb2 > 0x144) o->wb2 = 0x144;
    }
    o->state = 1;
    o->animFrame = o->bbe & 1;
}
