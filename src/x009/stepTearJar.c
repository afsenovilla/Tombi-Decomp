// FUNC 8011b508 436 X009
// MATCHING 8011b508 436
/* debt: volatile store of timer (game re-reads it with lh), char pad[16] for the 0x28 frame */
#include "TOBJ.H"

extern TObj PLAYER;
extern void *D_8012E024;
extern void tearJarHeld(TObj *);

static __inline__ int vdiv(int a, int b)
{
    return a / b;
}

static __inline__ void start(TObj *o)
{
    int vy, vx;

    o->visible = PLAYER.visible;
    o->animFrame = PLAYER.animFrame & 1;
    o->anim = D_8012E024;
    o->h->p.whole = PLAYER.h->p.whole;
    o->y.p.whole = PLAYER.y.p.whole;
    o->d->p.whole = PLAYER.d->p.whole;
    *(volatile short *)&o->timer = 20;
    vy = vdiv(-0x6c00 - (o->y.p.whole << 8), o->timer);
    o->d8c = 0;
    o->d30 = 0x20;
    o->d34 = -0x3c0;
    vx = vdiv(0x79b00 - (o->h->p.whole << 8), 20);
    o->category |= 0x80;
    o->velY = vy;
    o->velX = vx;
    o->b0f = PLAYER.b0f - 1;
    o->state++;
}

void stepTearJar(TObj *o)
{
    char pad[16];

    if (o->visible == 0)
        return;
    switch (o->step) {
    case 0:
        break;
    case 1:
        tearJarHeld(o);
        break;
    case 2:
        if (o->state != 0)
            break;
        start(o);
        break;
    }
}
