// FUNC 801072fc 196 X018
// MATCHING 801072fc 196
#include "TOBJ.H"
extern unsigned char *D_8009C330;
extern char D_80077D0C[];
void func_801072FC(TObj *o)
{
    ((unsigned char *)o)[0xc7] = 1;
    o->b9d = 0;
    ((unsigned char *)o)[0xc6] = 0;
    ((unsigned char *)o)[0xe3] = 0;
    D_8009C330[0] = 0;
    o->movetab = D_80077D0C;
    *(unsigned char *)&o->wac = 0;
    o->b6b = 0;
    o->ba4 = 0;
    o->d84 = 0;
    o->d88 = 0;
    o->d8c = 0;
    D_8009C330[8] = o->bbe & 1;
    if (D_8009C330[8] == 0) {
        if (o->wb2 < -0x144) o->wb2 = -0x144;
    } else {
        if (o->wb2 > 0x144) o->wb2 = 0x144;
    }
    o->state = 1;
    o->animFrame = o->bbe & 1;
}
