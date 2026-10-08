// FUNC 80123c60 584 X003
// MATCHING 80123c60 584
#include "TOBJ.H"
typedef struct { short w0; unsigned short w2; } SB;
extern char D_80077CF4[];
extern unsigned short *D_8013878C[], *D_80138790[], *D_801387A4[];
extern unsigned char D_80135B30[];
extern unsigned short D_1F80016A, D_1F800172;
extern int AnimAdvanceWithBox(TObj *);
extern void FUN_8001fb20(TObj *);
extern int func_801206F0(TObj *);

void func_80123C60(TObj *o)
{
    SB *s = (SB *)&o->wb4;
    unsigned short *a;
    unsigned short dx, dz;

    switch (o->state) {
    case 0:
        o->movetab = D_80077CF4;
        o->b69 = 0;
        o->wac = 0x25;
        o->state++;
        a = D_8013878C[0];
        goto common;
    case 1:
        AnimAdvanceWithBox(o);
        FUN_8001fb20(o);
        if (func_801206F0(o)) {
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
        o->wac = 0x26;
        o->state++;
        a = D_80138790[0];
        goto common;
    case 4:
        AnimAdvanceWithBox(o);
        if (--o->timer != -1) break;
        o->wac = 0x2b;
        o->state++;
        a = D_801387A4[0];
    common:
        o->anim = a;
        {
            unsigned char *pp = &D_80135B30[a[1] * 4];
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
