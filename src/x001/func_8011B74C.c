// FUNC 8011b74c 524 X001
// MATCHING 8011b74c 524
#include "TOBJ.H"

typedef struct { short x, y, z, pad; } V8011B74C;

extern unsigned short D_1F8001C8;
extern void *D_8013E5BC[];

void func_8011B74C(TObj *o)
{
    V8011B74C *a = (V8011B74C *)&o->wb4;
    V8011B74C *b = (V8011B74C *)&o->wbc;
    V8011B74C *c = (V8011B74C *)((char *)o + 0xc4);
    V8011B74C *d = (V8011B74C *)((char *)o + 0xcc);
    TObj *p;
    short dx, dy;

    switch (o->state) {
    case 0:
        {TObj *q;
        q = (TObj *)o->d90;
        o->wac = 3;
        o->state++;
        dy = q->y.p.whole - o->y.p.whole;
        dx = q->h->p.whole - o->h->p.whole;}
        if (D_1F8001C8 & 1) {
            a->y = 0;
            a->x = 0;
            b->y = 0;
            b->x = 0;
            c->y = dy;
            c->x = 0;
            d->y = dy;
            d->x = 0;
            a->z = dx - o->box0;
            b->z = dx + o->box1;
            c->z = dx - o->box0;
            d->z = dx + o->box1;
        } else {
            a->y = 0;
            a->z = 0;
            b->y = 0;
            b->z = 0;
            c->y = dy;
            c->z = 0;
            d->y = dy;
            d->z = 0;
            a->x = dx - o->box0;
            b->x = dx + o->box1;
            c->x = dx - o->box0;
            d->x = dx + o->box1;
        }
        break;
    case 1:
        p = (TObj *)o->d90;
        o->wac = 3;
        dy = p->y.p.whole - o->y.p.whole;
        dx = p->h->p.whole - o->h->p.whole;
        if (D_1F8001C8 & 1) {
            c->y = dy;
            d->y = dy;
            c->z = dx - o->box0;
            d->z = dx + o->box1;
        } else {
            c->y = dy;
            d->y = dy;
            c->x = dx - o->box0;
            d->x = dx + o->box1;
        }
        break;
    case 2:
        p = (TObj *)o->d90;
        o->wac = 0;
        dy = p->y.p.whole - o->y.p.whole;
        dx = p->h->p.whole - o->h->p.whole;
        if (D_1F8001C8 & 1) {
            c->y = dy;
            d->y = dy;
            c->z = dx - o->box0;
            d->z = dx + o->box1;
        } else {
            c->y = dy;
            d->y = dy;
            c->x = dx - o->box0;
            d->x = dx + o->box1;
        }
        break;
    }
    o->anim = D_8013E5BC[o->wac];
}
