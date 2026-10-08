// FUNC 80123310 584 X004
// MATCHING 80123310 584
#include "TOBJ.H"
typedef struct { short w0; unsigned short w2; } SB;
extern char D_80077CF4[];
extern unsigned short *D_80133C10[], *D_80133C1C[], *D_80133C64[];
extern unsigned char D_80130FD4[];
extern unsigned short D_1F80016A, D_1F800172;
extern int AnimAdvanceWithBox(TObj *);
extern void FUN_8001fb20(TObj *);
extern int func_8011FDD4(TObj *);

void func_80123310(TObj *o)
{
    SB *s = (SB *)&o->wb4;
    unsigned short *a;
    unsigned short dx, dz;

    switch (o->state) {
    case 0:
        o->movetab = D_80077CF4;
        o->b69 = 0;
        o->wac = 0x2d;
        o->state++;
        a = D_80133C10[0];
        goto common;
    case 1:
        AnimAdvanceWithBox(o);
        FUN_8001fb20(o);
        if (func_8011FDD4(o)) {
            o->d8c = s->w2;
            o->timer = 0;
            o->state++;
        }
        break;
    case 2:
        AnimAdvanceWithBox(o);
        dx = o->h->p.whole - D_1F80016A + 0xa0;
        dz = o->d->p.whole - D_1F800172 + 0x2d;
        if (dx >= 0x141) {
            o->timer = 0;
            break;
        }
        if (o->timer++ < 0x3c) break;
        if (dz >= 0x5a) break;
        o->state++;
        break;
    case 3:
        o->timer = 0xb4;
        o->wac = 0x30;
        o->state++;
        a = D_80133C1C[0];
        goto common;
    case 4:
        AnimAdvanceWithBox(o);
        if (--o->timer != -1) break;
        o->wac = 0x42;
        o->state++;
        a = D_80133C64[0];
    common:
        o->anim = a;
        {
            unsigned char *pp = &D_80130FD4[a[1] * 4];
            o->box0 = *pp++;
            o->box1 = *pp++;
            o->box2 = *pp++;
            o->box3 = *pp++;
        }
        o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
        break;
    case 5:
        if (AnimAdvanceWithBox(o)) {
            o->state = 0;
            o->step++;
        }
        break;
    }
}
