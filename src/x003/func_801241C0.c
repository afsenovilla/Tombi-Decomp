// FUNC 801241c0 504 X003
// MATCHING 801241c0 504
#include "TOBJ.H"
extern char D_80077CF4[];
extern unsigned short *D_80138794[];
extern unsigned char D_80135B30[];
extern unsigned short D_1F80016A, D_1F800172;
extern int AnimAdvanceWithBox(TObj *);
extern void FUN_8001fb20(TObj *);
extern int func_801206F0(TObj *);

void func_801241C0(TObj *o)
{
    unsigned short *a;
    unsigned short dx, dz;

    switch (o->state) {
    case 0:
        o->movetab = D_80077CF4;
        o->b69 = 0;
        o->wac = 0x27;
        o->state++;
        a = D_80138794[0];
        o->anim = a;
        {
            unsigned char *pp = &D_80135B30[a[1] * 4];
            o->box0 = *pp++;
            o->box1 = *pp++;
            o->box2 = *pp;
            o->box3 = pp[1];
        }
        o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
        break;
    case 1:
        AnimAdvanceWithBox(o);
        FUN_8001fb20(o);
        if (func_801206F0(o)) {
            o->timer = 0;
            o->state++;
        }
        break;
    case 2:
        AnimAdvanceWithBox(o);
        dx = o->h->p.whole - D_1F80016A + 0x60;
        dz = o->d->p.whole - D_1F800172 + 0x2d;
        if (dz >= 0x5b || dx >= 0xc1) {
            o->timer = 0;
            break;
        }
        if (o->timer++ < 0x46)
            break;
        o->timer = 10;
        o->state++;
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
