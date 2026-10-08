// FUNC 80133254 544 X000
// MATCHING 80133254 544
#include "TOBJ.H"
extern unsigned short DAT_8009c962;
extern unsigned short DAT_800a6066;
extern void *DAT_8013b17c;
extern void *DAT_8013b184[];
extern void *DAT_8013b188[];
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);
extern short FUN_80040278(TObj *, int, int);

void FUN_80133254(TObj *o)
{
    short f;
    switch (o->state) {
    case 0:
        o->box0 = 8;
        o->box1 = 0x10;
        o->box2 = 8;
        o->box3 = 0x10;
        o->active = 2;
        {
            short x = o->h->p.whole;
            o->velY = -0x400;
            o->velX = x;
        }
        o->anim = DAT_8013b184[0];
        FUN_8001fe6c(o);
        {
            unsigned short c = DAT_8009c962;
            unsigned char s = o->state;
            o->w78 = 1;
            o->b68 = c;
            o->state = s + 1;
        }
    case 1:
        FUN_8001fec0(o);
        f = ((unsigned short *)o->anim)[2];
        o->animFrame = f;
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (o->velY > 0)
            o->state = 2;
        break;
    case 2:
        FUN_8001fec0(o);
        f = ((unsigned short *)o->anim)[2];
        o->animFrame = f;
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (FUN_80040278(o, o->h->p.whole, (short)(o->y.p.whole + 0x20))) {
            o->animFrame = DAT_800a6066 ^ 1;
            o->anim = DAT_8013b188[0];
            FUN_8001fe6c(o);
            o->state = 3;
        }
        break;
    case 3:
        FUN_8001fec0(o);
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (FUN_80040278(o, o->h->p.whole, (short)(o->y.p.whole + 0x10))) {
            o->active = 1;
            o->state = 4;
        }
        break;
    case 4:
        if (FUN_8001fec0(o)) {
            o->anim = DAT_8013b17c;
            FUN_8001fe6c(o);
            o->step = 1;
            o->state = 0;
        }
        break;
    }
}
