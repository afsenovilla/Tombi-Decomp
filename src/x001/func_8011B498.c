// FUNC 8011b498 396 X001
// MATCHING 8011b498 396
#include "TOBJ.H"

typedef struct { short x, y, z, pad; } SV;
#define V(o) ((SV *)((char *)(o) + 0xb4))
extern void *D_8013E5BC;
extern unsigned short D_1F8001C8;

void func_8011B498(TObj *o)
{
    switch (o->state) {
    case 0:
        o->state++;
        o->anim = D_8013E5BC;
    {
        TObj *e = (TObj *)o->d90;
        short dy = e->y.p.whole - o->y.p.whole;
        short dx = e->h->p.whole - o->h->p.whole;
        if (D_1F8001C8 & 1) {
            V(o)[0].y = 0;
            V(o)[0].x = 0;
            V(o)[1].y = 0;
            V(o)[1].x = 0;
            V(o)[2].y = dy;
            V(o)[2].x = 0;
            V(o)[3].y = dy;
            V(o)[3].x = 0;
            V(o)[0].z = dx - o->box0;
            V(o)[1].z = dx + o->box1;
            V(o)[2].z = dx - o->box0;
            V(o)[3].z = dx + o->box1;
        } else {
            V(o)[0].y = 0;
            V(o)[0].z = 0;
            V(o)[1].y = 0;
            V(o)[1].z = 0;
            V(o)[2].y = dy;
            V(o)[2].z = 0;
            V(o)[3].y = dy;
            V(o)[3].z = 0;
            V(o)[0].x = dx - o->box0;
            V(o)[1].x = dx + o->box1;
            V(o)[2].x = dx - o->box0;
            V(o)[3].x = dx + o->box1;
        }
    }
        break;
    case 1:
    {
        TObj *e = (TObj *)o->d90;
        short dy = e->y.p.whole - o->y.p.whole;
        short dx = e->h->p.whole - o->h->p.whole;
        if (D_1F8001C8 & 1) {
            V(o)[2].y = dy;
            V(o)[3].y = dy;
            V(o)[2].z = dx - o->box0;
            V(o)[3].z = dx + o->box1;
        } else {
            V(o)[2].y = dy;
            V(o)[3].y = dy;
            V(o)[2].x = dx - o->box0;
            V(o)[3].x = dx + o->box1;
        }
    }
        break;
    case 2:
        break;
    }
}
