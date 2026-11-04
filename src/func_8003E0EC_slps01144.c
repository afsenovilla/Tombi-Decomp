// VERSION SLPS_01144 slps01144 8003E0EC 144
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
extern unsigned short GAME;
extern unsigned char D_8009EC23;
extern void addItemToInventory(int, int, int);
extern void setEventStarted(int, int, int);
extern void func_80021A40(int);
void func_8003E0EC_slps01144(TObj *o)
{
    addItemToInventory(8, 1, 1);
    setEventStarted(169, 0, 0);
    if (GAME == 0) D_8009EC23 = 2;
    if (!(o->b0c & 0x80)) func_80021A40(o->b6b);
    o->b04++;
}
#endif
