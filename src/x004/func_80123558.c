// FUNC 80123558 504 X004
// MATCHING 80123558 504
#include "TOBJ.H"

extern char D_80077CF4[];
extern unsigned char D_80130FD4[];
extern unsigned short *D_80133B98[];
extern unsigned short D_1F80016A;
extern unsigned short D_1F800172;
extern int AnimAdvanceWithBox(TObj *);
extern void FUN_8001fb20(TObj *);
extern int func_8011FDD4(TObj *);

void func_80123558(TObj *o)
{
    unsigned short *a;
    unsigned char *p;
    unsigned short dx;

    switch (o->state) {
    case 0:
        o->movetab = D_80077CF4;
        o->b69 = 0;
        o->wac = 0xf;
        o->state++;
        a = D_80133B98[0];
        o->anim = a;
        p = &D_80130FD4[a[1] * 4];
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p;
        o->box3 = p[1];
        o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
        break;
    case 1:
        AnimAdvanceWithBox(o);
        FUN_8001fb20(o);
        if (func_8011FDD4(o)) {
            o->timer = 0;
            o->state++;
        }
        break;
    case 2:
        AnimAdvanceWithBox(o);
        dx = o->h->p.whole - D_1F80016A + 0x60;
        if ((unsigned short)(o->d->p.whole - D_1F800172 + 0x2d) >= 0x5b ||
            dx >= 0xc1) {
            o->timer = 0;
        } else if (o->timer++ >= 0x46) {
            o->timer = 10;
            o->state++;
        }
        break;
    case 3:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->step = 1;
            o->state = 0;
            o->substep = 6;
            o->b69 = 0;
            o->b68 = 0;
        }
        break;
    }
}
