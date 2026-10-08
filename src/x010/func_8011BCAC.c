// FUNC 8011bcac 176 X010
// MATCHING 8011bcac 176
#include "TOBJ.H"
extern TObj *D_8009F0EC;
extern int rsin(int);
extern int rcos(int);

void func_8011BCAC(TObj *o)
{
    int a = (D_8009F0EC->d8c + 0x400) & 0xfff;
    int x = rcos(a) * o->wba >> 12;
    int y = rsin(a) * o->wba;
    o->h->p.whole = o->wb8 + (D_8009F0EC->h->p.whole + x) + D_8009F0EC->d30;
    o->y.p.whole = D_8009F0EC->y.p.whole + (y >> 12);
}
