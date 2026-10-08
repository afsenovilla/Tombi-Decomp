// FUNC 8011e594 484 X004
/* score 10: whole function (covers csv pieces 8011E688/8011E6B0). Left: in case 0 the constant 8 lands in v0 (game v1)
   and lh 0x12 (o->a.p.whole) is scheduled after the anim store (game before). The game keeps that lh after the
   w08 store: reproduced only with an empty do{}while(0) barrier (debt) - without it sched1 hoists the load (32).
   Tried: [0] arrays for D_80134D84/D_1F8002D4, raw/volatile reads of a.p.whole, temp for anim, if/else layouts. */
#include "TOBJ.H"
extern unsigned char D_8009C940;
extern unsigned char D_8009C941;
extern unsigned char D_8009CD99, D_8009CD9D;
extern unsigned char D_800B146D, D_800B1475, D_800B1471;
extern void *D_80134D84[0];
extern int D_1F8002D4;
extern void FUN_8001fe6c(TObj *);
extern int FUN_800202b4(TObj *);
extern void FUN_8001fec0(TObj *);
extern int rcos(int);
extern void FUN_8005a9a4(int, int);
extern void FUN_8004d620(int, int);
extern void FUN_800188e0(TObj *);

void func_8011E594(TObj *o)
{
    unsigned char *p;

    switch (o->b04) {
    case 0:
        p = &D_8009C940;
        if (*p != 0) {
            if (D_8009C941 != 0x7e) break;
            *p = 0;
            o->box2 = 8;
            o->active = 1;
            o->box0 = 8;
            o->b69 = 0;
            o->b0a = 0;
            o->b0d = 1;
            o->anim = D_80134D84[0];
            o->box1 = 0x10;
            o->box3 = 0x10;
            o->w1e = 9;
            o->w08 = 0x7a0e;
            do {} while (0);
            o->d30 = o->a.p.whole;
            o->w22 = 0;
            o->d3c = D_1F8002D4;
            FUN_8001fe6c(o);
            o->b04++;
        } else {
            o->active = 2;
        }
        break;
    case 1:
        if (FUN_800202b4(o)) FUN_8001fec0(o);
        o->a.p.whole = o->d30 + (rcos(o->w22 & 0xfff) >> 7);
        if ((o->w22 & 0xfff) > 0x800) o->animFrame = 0;
        else o->animFrame = 1;
        o->w22 += 0x10;
        break;
    case 2:
        FUN_8005a9a4(0x77, 0);
        FUN_8004d620(0x32, 2);
        D_8009CD99 = 9;
        D_8009CD9D = 0x3c;
        D_800B146D = 0x3c;
        D_800B1475 = 1;
        D_800B1471 = 0x1e;
        o->b04++;
        break;
    case 3:
        FUN_800188e0(o);
        break;
    }
}
