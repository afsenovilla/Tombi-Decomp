/* score 2: case 0 only: game loads lbu state before lw d90 (ours lw d90 first, longer chain); same leftover as wip func_8011B74C. Tried all orders of p/state++/wac and raw forms of the state/wac stores. */
// FUNC 8011aefc 616 X001
#include "TOBJ.H"

typedef struct { short x, y, z, pad; } V8011AEFC;

extern unsigned short D_1F8001C8;
extern void *D_8013E5BC[];

#define a ((V8011AEFC *)&o->wb4)
#define b ((V8011AEFC *)&o->wbc)
#define c ((V8011AEFC *)((char *)o + 0xc4))
#define d ((V8011AEFC *)((char *)o + 0xcc))

void func_8011AEFC(TObj *o)
{
    TObj *p;
    short dx, dy;
    int v;

    switch (o->state) {
    case 0:
        p = (TObj *)o->d90;
        o->state++;
        o->wac = 0;
        dy = p->y.p.whole - o->y.p.whole;
        dx = p->h->p.whole - o->h->p.whole;
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
        o->d88 = ((TObj *)o->d90)->d88;
        v = (o->d88 >> 8) & 0xff;
        if (v < 0x60) {
            o->box0 = 3;
            o->box1 = 3;
        } else if (v < 0xa0) {
            o->box0 = 4;
            o->box1 = 3;
        } else {
            o->box0 = 4;
            o->box1 = 4;
        }
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
    case 3:
        p = (TObj *)o->d90;
        o->box0 = 4;
        o->box1 = 4;
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
    case 4:
        p = (TObj *)o->d90;
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
    case 5:
        o->wac = 0;
        break;
    }
    o->anim = D_8013E5BC[o->wac];
}
