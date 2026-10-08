// FUNC 8011f3cc 80 X004
// MATCHING 8011f3cc 80
#include "TOBJ.H"

static __inline__ void push(Fix16 *h, short w, short x, short bw)
{
    if (h->p.whole + w > x - bw) h->p.whole = x - bw - w;
}

void func_8011F3CC(TObj *a, TObj *b)
{
    push(a->h, a->box0, b->h->p.whole, b->box0);
}
