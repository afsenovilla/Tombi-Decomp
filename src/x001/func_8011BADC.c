// FUNC 8011badc 400 X001
// MATCHING 8011badc 400
#include "TOBJ.H"

typedef struct {
    char p[0xb4];
    short b4, b6, b8, ba, bc, be, c0, c2, c4, c6, c8, ca, cc, ce, d0;
} E;

extern void *D_8013E5BC;
extern unsigned short D_1F8001C8;

void func_8011BADC(TObj *o)
{

    switch (o->state) {
    case 0: {
        TObj *p;
        short dy, dx;
        o->state++;
        *(short *)((char *)o + 0xac) = 0;
        o->anim = D_8013E5BC;
        p = (TObj *)o->d90;
        dy = p->y.p.whole - o->y.p.whole;
        dx = p->h->p.whole - o->h->p.whole;
        if (D_1F8001C8 & 1) {
            ((E *)o)->b6 = 0;
            ((E *)o)->b4 = 0;
            ((E *)o)->be = 0;
            ((E *)o)->bc = 0;
            ((E *)o)->c6 = dy;
            ((E *)o)->c4 = 0;
            ((E *)o)->ce = dy;
            ((E *)o)->cc = 0;
            ((E *)o)->b8 = dx - o->box0;
            ((E *)o)->c0 = dx + o->box1;
            ((E *)o)->c8 = dx - o->box0;
            ((E *)o)->d0 = dx + o->box1;
        } else {
            ((E *)o)->b6 = 0;
            ((E *)o)->b8 = 0;
            ((E *)o)->be = 0;
            ((E *)o)->c0 = 0;
            ((E *)o)->c6 = dy;
            ((E *)o)->c8 = 0;
            ((E *)o)->ce = dy;
            ((E *)o)->d0 = 0;
            ((E *)o)->b4 = dx - o->box0;
            ((E *)o)->bc = dx + o->box1;
            ((E *)o)->c4 = dx - o->box0;
            ((E *)o)->cc = dx + o->box1;
        }
        break;
    }
    case 1: {
        TObj *p;
        short dy, dx;
        p = (TObj *)o->d90;
        dy = p->y.p.whole - o->y.p.whole;
        dx = p->h->p.whole - o->h->p.whole;
        if (D_1F8001C8 & 1) {
            ((E *)o)->c6 = dy;
            ((E *)o)->ce = dy;
            ((E *)o)->c8 = dx - o->box0;
            ((E *)o)->d0 = dx + o->box1;
        } else {
            ((E *)o)->c6 = dy;
            ((E *)o)->ce = dy;
            ((E *)o)->c4 = dx - o->box0;
            ((E *)o)->cc = dx + o->box1;
        }
        break;
    }
    case 2:
        break;
    }
}
