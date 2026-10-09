// FUNC 8011e594 484 X004
// MATCHING 8011e594 484
#include "TOBJ.H"
extern unsigned char D_8009C940;
extern unsigned char D_8009C941;
extern unsigned char D_8009CD99, D_8009CD9D;
extern unsigned char D_800B146D, D_800B1475, D_800B1471;
extern void *D_80134D84;
extern int D_1F8002D4[];
extern void FUN_8001fe6c(TObj *);
extern int FUN_800202b4(TObj *);
extern void FUN_8001fec0(TObj *);
extern int rcos(int);
extern void FUN_8005a9a4(int, int);
extern void FUN_8004d620(int, int);
extern void FUN_800188e0(TObj *);
static __inline__ void setbox(TObj *q, short a, short b, short c, short d)
{
    q->box0 = a;
    q->box1 = b;
    q->box2 = c;
    q->box3 = d;
}

void func_8011E594(TObj *o)
{
    unsigned char *p;

    switch (o->b04) {
    case 0:
        p = &D_8009C940;
        if (*p != 0) {
            if (D_8009C941 != 0x7e) break;
            *p = 0;
            setbox(o, 8, 0x10, 8, 0x10);
            o->active = 1;
            o->b69 = 0;
            o->b0a = 0;
            o->w1e = 9;
            o->b0d = 1;
            o->w08 = 0x7a0e;
            o->anim = D_80134D84;
            o->d3c = D_1F8002D4[0];
            o->d30 = o->a.p.whole;
            o->w22 = 0;
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
