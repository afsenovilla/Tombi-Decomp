// FUNC 80123310 584 X004
// MATCHING 80123310 584
#include "spad.h"
#include "TOBJ.H"
typedef struct { short w0; unsigned short w2; } SB;
extern char D_80077CF4[];
extern unsigned short *D_80133C10[], *D_80133C64[];
extern char D_801342CC[];
extern char D_801342F0[];
extern char D_80134304[];
extern char D_80133E84[];
extern char D_80133E98[];
extern char D_801341A8[];
extern char D_801341BC[];
extern char D_80134574[];
extern char D_80134328[];
extern char D_8013434C[];
extern char D_8013445C[];
extern char D_801341D0[];
extern char D_80133D58[];
extern char D_80133D60[];
extern char D_8013456C[];
extern char D_80134370[];
static unsigned *D_80133C1C[18] = {
    (unsigned *)D_801342CC, (unsigned *)D_801342F0, (unsigned *)D_80134304, (unsigned *)D_80133E84,
    (unsigned *)D_80133E98, (unsigned *)D_801341A8, (unsigned *)D_801341BC, (unsigned *)D_80134574,
    (unsigned *)D_801341BC, (unsigned *)D_80134328, (unsigned *)D_8013434C, (unsigned *)D_8013434C,
    (unsigned *)D_8013445C, (unsigned *)D_801341D0, (unsigned *)D_80133D58, (unsigned *)D_80133D60,
    (unsigned *)D_8013456C, (unsigned *)D_80134370,
};
extern unsigned char D_80130FD4[];
extern int AnimAdvanceWithBox(TObj *);
extern void applyFrameVelocityY(TObj *);
extern int komaPigCheckLanded(TObj *);

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
        applyFrameVelocityY(o);
        if (komaPigCheckLanded(o)) {
            o->d8c = s->w2;
            o->timer = 0;
            o->state++;
        }
        break;
    case 2:
        AnimAdvanceWithBox(o);
        dx = o->h->p.whole - (*(unsigned short*)((char*)&SPAD.i168 + 2)) + 0xa0;
        dz = o->d->p.whole - (*(unsigned short*)((char*)&SPAD.i170 + 2)) + 0x2d;
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
