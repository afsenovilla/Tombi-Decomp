// FUNC 8011f79c 580 X000
// MATCHING 8011f79c 580
#include "TOBJ.H"
#define SUB(o) ((TObj *)(o)->d90)
extern void *DAT_8013b14c;
extern void *DAT_8013b150;
extern unsigned short DAT_800a603c;
extern unsigned char DAT_800a60a1;
extern unsigned char DAT_800a60a3;
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fdac(int, int);
extern void FUN_8001fec0(TObj *);

void FUN_8011f79c(TObj *o)
{
    int t;
    TObj *p;
    switch (o->state) {
    case 0:
        o->b69 = 0;
        o->anim = DAT_8013b14c;
        FUN_8001fe6c(o);
        p = SUB(o);
        o->d88 = 0x100;
        o->velX = o->y.p.whole;
        o->velY = o->y.p.whole;
        p->h->p.whole = o->h->p.whole - 6;
        o->state++;
    case 1:
        o->d88 += 4;
        o->y.p.whole = o->velX + FUN_8001fdac((unsigned char)o->d88, 4);
        FUN_8001fec0(SUB(o));
        FUN_8001fec0(o);
        if (DAT_800a603c == 5 && DAT_800a60a1 && DAT_800a60a3) {
            o->state = 2;
            o->timer = 0x28;
        }
        break;
    case 2:
        o->d88 += 4;
        o->y.p.whole = o->velX + FUN_8001fdac((unsigned char)o->d88, 4);
        FUN_8001fec0(o);
        FUN_8001fec0(SUB(o));
        if (o->b69) {
            p = SUB(o);
            p->anim = DAT_8013b150;
            FUN_8001fe6c(p);
            DAT_800a60a3 = 0;
            o->step = 1;
            o->state = 0;
        }
        break;
    case 3:
        FUN_8001fec0(o);
        o->d88 += 4;
        if (o->d88 > 0x200)
            o->d88 = 0x200;
        t = FUN_8001fdac((unsigned char)o->d88, 4);
        if ((short)t < 0)
            t = -t;
        o->y.p.whole = o->velX + t;
        o->velX++;
        if (o->velX < o->velY) {
            o->velX = o->velY;
            o->state = 2;
            o->velY = o->y.p.whole + 0x10;
        }
        break;
    }
}
