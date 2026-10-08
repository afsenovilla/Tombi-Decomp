// FUNC 8011a5dc 336 X001
// MATCHING 8011a5dc 336
#include "TOBJ.H"
typedef struct { short x, y, z, pad; } SV;
typedef struct { char p[0xb4]; SV v[4]; } TX;

extern void *D_8013E5BC;
extern unsigned short D_1F8001C8;

void func_8011A5DC(TObj *o)
{
    TObj *p;
    short dx, dy;
    SV *v;

    if (o->state != 0) return;
    o->y.raw = o->d34;
    o->h->raw = o->d30;
    o->velH = 0;
    o->velV = 0;
    o->velX = 0;
    o->velY = 0;
    o->d84 = 0;
    o->d88 = 0;
    o->w74 = 0;
    if (o->subtype) {
        o->anim = D_8013E5BC;
        p = (TObj *)o->d90;
        dy = p->y.p.whole - o->y.p.whole;
        dx = p->h->p.whole - o->h->p.whole;
        v = ((TX *)o)->v;
        if (D_1F8001C8 & 1) {
            v[0].y = 0;
            v[0].x = 0;
            v[1].y = 0;
            v[1].x = 0;
            v[2].y = dy;
            v[2].x = 0;
            v[3].y = dy;
            v[3].x = 0;
            v[0].z = dx - o->box0;
            v[1].z = dx + o->box1;
            v[2].z = dx - o->box0;
            v[3].z = dx + o->box1;
        } else {
            v[0].y = 0;
            v[0].z = 0;
            v[1].y = 0;
            v[1].z = 0;
            v[2].y = dy;
            v[2].z = 0;
            v[3].y = dy;
            v[3].z = 0;
            v[0].x = dx - o->box0;
            v[1].x = dx + o->box1;
            v[2].x = dx - o->box0;
            v[3].x = dx + o->box1;
        }
    }
    o->state++;
}
