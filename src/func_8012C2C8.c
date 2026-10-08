// FUNC 8012c2c8 720 X000
// MATCHING 8012c2c8 720
#include "TOBJ.H"
extern char D_80077CF4[];
extern unsigned short *D_8013A214[];
extern unsigned short *D_8013A220[];
extern unsigned short *D_8013A268[];
extern unsigned short *D_8013A254[];
extern unsigned short *D_8013A18C[];
extern unsigned char D_80138FD8[];
extern unsigned short DAT_1f80016a;
extern unsigned short DAT_1f800172;
extern int AnimAdvanceWithBox(TObj *);
extern void FUN_8001fb20(TObj *);
extern int FUN_801274cc(TObj *);

static __inline__ void SetBox(TObj *o)
{
    unsigned char *p;
    p = &D_80138FD8[((unsigned short *)o->anim)[1] * 4];
    o->box0 = *p++;
    o->box1 = *p++;
    o->box2 = *p++;
    o->box3 = *p;
    o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
}

void func_8012C2C8(TObj *o)
{
    unsigned short *w = (unsigned short *)&o->wb4;
    unsigned short dx, dz;
    switch (o->state) {
    case 0:
        o->movetab = D_80077CF4;
        o->b69 = 0;
        o->wac = 0x2d;
        o->state++;
        o->anim = D_8013A214[0];
        SetBox(o);
        break;
    case 1:
        AnimAdvanceWithBox(o);
        FUN_8001fb20(o);
        if (FUN_801274cc(o)) {
            o->d8c = w[1];
            o->timer = 0;
            o->state++;
        }
        break;
    case 2:
        AnimAdvanceWithBox(o);
        dx = o->h->p.whole - DAT_1f80016a + 0xa0;
        dz = o->d->p.whole - DAT_1f800172 + 0x2d;
        if (dx > 0x140) {
            o->timer = 0;
            break;
        }
        if (o->timer++ < 0x3c) break;
        if (dz < 0x5a) {
            o->state = 3;
        } else {
            o->state = 5;
        }
        break;
    case 3:
        o->timer = 0xb4;
        o->wac = 0x30;
        o->state++;
        o->anim = D_8013A220[0];
        SetBox(o);
        break;
    case 4:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->state = 8;
            o->wac = 0x42;
            o->anim = D_8013A268[0];
            SetBox(o);
        }
        break;
    case 5:
        o->wac = 0x3d;
        o->state++;
        o->anim = D_8013A254[0];
        SetBox(o);
        break;
    case 6:
        if (AnimAdvanceWithBox(o)) {
            o->timer = 0xb4;
            o->wac = 0xb;
            o->state++;
            o->anim = D_8013A18C[0];
            SetBox(o);
        }
        break;
    case 7:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) goto next;
        break;
    case 8:
        if (AnimAdvanceWithBox(o)) {
        next:
            o->step++;
            o->state = 0;
        }
        break;
    }
}
