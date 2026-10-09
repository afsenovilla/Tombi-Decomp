// FUNC 8011ba34 660 X003
// MATCHING 8011ba34 660
#include "TOBJ.H"

extern unsigned char D_800A6038[], D_800A6039[], D_800A603C[], D_800A603D[];
extern void *D_8013986C[];
extern short D_8007A1F0[], D_8007A5F0[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_8001e4f0(int);
extern void func_8011B588(TObj *, int, int);
extern int func_8011B324(TObj *);

static __inline__ void move(TObj *o)
{
    o->velY = (o->velV * D_8007A1F0[*(unsigned char *)&o->d8c]) >> 12;
    o->velX = (o->velV * D_8007A5F0[*(unsigned char *)&o->d8c]) >> 12;
    if (o->animFrame == 0) {
        o->velX = -o->velX;
        o->velY = -o->velY;
    }
    o->h->raw += o->velX << 8;
    o->y.raw += o->velY << 8;
}

void func_8011BA34(TObj *o)
{
    short v;

    switch (o->state) {
    case 0:
        func_8011B588(o, 0, 0);
        if (func_8011B324(o)) {
            o->state++;
            D_800A6038[0] = 5;
            D_800A603C[0] = 5;
            D_800A603D[0] = 0x40;
            D_800A6039[0] = 0;
            o->wac = 2;
            o->anim = D_8013986C[0];
            AnimLoadDuration(o);
            o->velV = 0xe00;
            move(o);
        }
        break;
    case 1:
        FUN_8001e4f0(0x70);
        o->velV = 0x380;
        o->state++;
    case 2:
        move(o);
        v = o->velV;
        if (v < 0) {
            o->velV = v - 0x50;
            if (o->velV < -0x400) {
                o->velV = -0x400;
                o->state = 0;
                o->step++;
                o->h->p.whole = o->d30;
                o->y.p.whole = o->wb4;
            }
        } else {
            o->velV = v - 0x40;
        }
        AnimAdvance(o);
        break;
    }
}
