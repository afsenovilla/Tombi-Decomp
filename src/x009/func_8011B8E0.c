// FUNC 8011b8e0 556 X009
// MATCHING 8011b8e0 556
#include "TOBJ.H"
extern int D_1F8002D4;
extern void *D_8012DB6C[];
extern void *D_8012DB74[];
extern short FUN_8005e420(int, int);
extern int FUN_8001fe0c(int, int);
extern int FUN_8001fe3c(int, int);
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);
extern int FUN_800202b4(TObj *);
extern void FUN_80018838(TObj *);

void func_8011B8E0(TObj *o)
{
    TObj *e;
    int r;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->step = 0;
        o->state = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->w1e = 6;
        o->b0a = 2;
        o->b0d = 1;
        o->b69 = 0;
        o->w08 = FUN_8005e420(0x80, 0x1ff);
        switch (o->subtype & 1) {
        case 0:
            o->anim = D_8012DB6C[o->b0c];
            o->active = 2;
            break;
        case 1:
            o->anim = D_8012DB74[o->b0c];
            o->box0 = 0x20;
            o->box1 = 0x40;
            o->box2 = 0;
            o->box3 = 0x20;
            o->active = 1;
            break;
        }
        o->d3c = D_1F8002D4;
        FUN_8001fe6c(o);
        break;
    case 1:
        switch (o->subtype & 1) {
        case 0:
            o->velX += 2;
            o->velH = FUN_8001fe0c(o->velX & 0xff, 0x1000);
            o->d8c = (o->velH >> 8) & 0xfff;
            break;
        case 1:
            e = (TObj *)o->d90;
            o->h->p.whole = e->h->p.whole - FUN_8001fe0c(*(unsigned char *)&e->d8c, 0x4c);
            o->y.p.whole = e->y.p.whole + FUN_8001fe3c(*(unsigned char *)&e->d8c, 0x4c);
            o->d8c = e->d8c;
            break;
        }
        FUN_8001fec0(o);
        FUN_800202b4(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}
