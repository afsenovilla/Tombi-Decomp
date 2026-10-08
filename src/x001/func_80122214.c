// FUNC 80122214 176 X001
// MATCHING 80122214 176
#include "TOBJ.H"
extern TObj *D_8009F0EC;
extern int rsin(int);
extern int rcos(int);

void func_80122214(TObj *o)
{
    int a = (D_8009F0EC->d8c + 0x400) & 0xfff;
    int x = rcos(a) * o->wba >> 12;
    int y = rsin(a) * o->wba;
    o->h->p.whole = o->wb8 + (D_8009F0EC->h->p.whole + x) + D_8009F0EC->d30;
    o->y.p.whole = D_8009F0EC->y.p.whole + (y >> 12);
}
