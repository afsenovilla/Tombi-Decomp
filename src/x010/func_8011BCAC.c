// FUNC 8011bcac 176 X010
// MATCHING 8011bcac 176
#include "TOBJ.H"
extern TObj *GRABBED_OBJECT;
extern int rsin(int);
extern int rcos(int);

void func_8011BCAC(TObj *o)
{
    int a = (GRABBED_OBJECT->d8c + 0x400) & 0xfff;
    int x = rcos(a) * o->wba >> 12;
    int y = rsin(a) * o->wba;
    o->h->p.whole = o->wb8 + (GRABBED_OBJECT->h->p.whole + x) + GRABBED_OBJECT->d30;
    o->y.p.whole = GRABBED_OBJECT->y.p.whole + (y >> 12);
}
