// FUNC 8011d4fc 264 X014
// MATCHING 8011d4fc 264
#include "TOBJ.H"

extern void spawnPoolType32At(int, int, int, int);

void rotatedChildPartEnemyCollision(TObj *o, TObj *p)
{
    if (o->type == 0x47 && o->subtype >= 3) return;
    if ((unsigned short)(o->d->p.whole - p->d->p.whole + 0x2d) >= 0x5b) return;
    if ((unsigned short)(o->h->p.whole - p->h->p.whole + (p->box0 + o->box0)) > p->box1 + o->box1) return;
    if ((unsigned short)(o->y.p.whole - p->y.p.whole + (o->box2 + p->box2)) > o->box3 + p->box3) return;
    o->active = 2;
    o->b04 = 2;
    o->step = 0;
    o->state = 0;
    spawnPoolType32At(0, o->a.p.whole, o->y.p.whole, o->b.p.whole);
}
