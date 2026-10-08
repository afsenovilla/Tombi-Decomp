// FUNC 80133254 544 X000
// wip: score 34 (antes 85). DAT_8013b188 como array y orden de stores del caso 0 por permutacion; falta: lw h tras los sh de 8, y en casos 1-2 el sh animFrame va al final (a2) con lh+lhu de velY.
#include "TOBJ.H"
extern unsigned short DAT_8009c962;
extern unsigned short DAT_800a6066;
extern void *DAT_8013b17c;
extern void *DAT_8013b184;
extern void *DAT_8013b188[];
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);
extern short FUN_80040278(TObj *, int, int);

void FUN_80133254(TObj *o)
{
    switch (o->state) {
    case 0:
        o->box0 = 8;
        o->box1 = 0x10;
        o->box2 = 8;
        o->box3 = 0x10;
        o->velY = -0x400;
        o->velX = o->h->p.whole;
        o->active = 2;
        o->anim = DAT_8013b184;
        FUN_8001fe6c(o);
        o->w78 = 1;
        o->b68 = DAT_8009c962;
        o->state++;
    case 1:
        FUN_8001fec0(o);
        o->animFrame = ((unsigned short *)o->anim)[2];
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (o->velY > 0)
            o->state = 2;
        break;
    case 2:
        FUN_8001fec0(o);
        o->animFrame = ((unsigned short *)o->anim)[2];
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
