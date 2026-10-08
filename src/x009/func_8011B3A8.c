// FUNC 8011b3a8 352 X009
// MATCHING 8011b3a8 352
/* debt: volatile timer store (game reloads it for the first division; the second divides by a short 20) */
#include "TOBJ.H"
extern TObj D_800A6038;
extern void *D_8012E024;

void func_8011B3A8(TObj *o)
{
    int vy;
    int vx;
    short n;
    char pad[16];

    if (o->state == 0) {
        o->visible = D_800A6038.visible;
        o->animFrame = D_800A6038.animFrame & 1;
        o->anim = D_8012E024;
        o->h->p.whole = D_800A6038.h->p.whole;
        o->y.p.whole = D_800A6038.y.p.whole;
        o->d->p.whole = D_800A6038.d->p.whole;
        *(volatile short *)&o->timer = 20;
        vy = (-0x6c00 - (o->y.p.whole << 8)) / o->timer;
        o->d8c = 0;
        o->d30 = 0x20;
        o->d34 = -0x3c0;
        n = 20;
        vx = (0x79b00 - (o->h->p.whole << 8)) / n;
        o->velY = vy;
        o->velX = vx;
        o->category |= 0x80;
        o->b0f = D_800A6038.b0f - 1;
        o->state++;
    }
}
