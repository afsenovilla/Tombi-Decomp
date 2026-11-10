// VERSION BETA_980203 beta980203 8003F9E8 144
#if defined(VERSION_BETA_980203)
// Beta version: JP C of src/func_8003E0EC_slps01144.c with setEventStarted(169, 0, 1) (JP 0).
#include "TOBJ.H"
extern unsigned short GAME;
extern unsigned char D_8009EC23;
extern void addItemToInventory(int, int, int);
extern void setEventStarted(int, int, int);
extern void func_80021A40(int);
void func_8003F9E8(TObj *o)
{
    addItemToInventory(8, 1, 1);
    setEventStarted(169, 0, 1);
    if (GAME == 0) D_8009EC23 = 2;
    if (!(o->b0c & 0x80)) func_80021A40(o->b6b);
    o->b04++;
}
#endif
