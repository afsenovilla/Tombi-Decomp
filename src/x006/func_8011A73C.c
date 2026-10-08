// FUNC 8011a73c 320 X006
// MATCHING 8011a73c 320
#include "TOBJ.H"
typedef struct { char p[0x1c]; short w1c; } X;
typedef struct {
    TObj t;
} S;
extern unsigned char D_8009C942;
extern short D_8007A3F0[];
extern int func_8011AFB4(TObj *);
extern int func_8011B238(TObj *);

void func_8011A73C(TObj *o)
{
    TObj *p = (TObj *)o->d90;
    X *x = (X *)((char *)p + 0xb4);

    switch (o->step) {
    case 0:
        o->velY = 0;
        o->velV = 0;
        o->step++;
        *(int *)&o->wb4 = o->d34;
        break;
    case 1:
        if (p->b9c) {
            o->step++;
            break;
        }
        if (x->w1c == 0) {
            o->step = 4;
        }
        break;
    case 2:
        if (p->b9c == 0) {
            func_8011AFB4(o);
            o->step++;
        }
        break;
    case 3:
        if (func_8011AFB4(o) == 1) {
            o->step = 1;
        }
        break;
    case 4:
        func_8011B238(o);
        break;
    }
    if (D_8009C942 == 0) {
        o->velY = (o->velY + 0x20) & 0xff;
        o->velV = D_8007A3F0[o->velY];
        o->d34 = (unsigned short)o->wb4 + (o->velV << 4);
    }
}
