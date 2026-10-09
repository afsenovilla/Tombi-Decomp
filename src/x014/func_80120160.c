// FUNC 80120160 464 X014
// MATCHING 80120160 464
#include "TOBJ.H"

typedef struct { short a, b, c, d; } E8;

extern int AnimAdvance(TObj *);
extern unsigned int FUN_8001f9e0(void);
extern void func_8011D88C(TObj *, int);
extern void func_8011F850(void);
extern void func_8011F888(void);
extern void func_80124848(TObj *, int, unsigned char);
extern void func_8011D948(TObj *);
extern void func_80116C9C(TObj *);

void func_80120160(TObj *o)
{
    unsigned short *q = (unsigned short *)&o->wb4;

    switch (o->substep) {
    case 0:
        q[4] = 1;
        q[5] = FUN_8001f9e0() & 3;
        func_8011D88C(o, 0x14);
        o->substep++;
        func_8011F850();
        break;
    case 1:
        if (AnimAdvance(o)) {
            func_8011D88C(o, 0x15);
            o->substep++;
        }
        break;
    case 2:
        if (AnimAdvance(o)) {
            func_80124848(o, 1, q[5]);
            func_8011D948(o);
            func_80116C9C(o);
            o->substep++;
            func_8011F888();
        }
        break;
    case 3:
        if (AnimAdvance(o)) {
            if (q[4] != 0) {
                q[4]--;
                q[5] = (q[5] + 1) & 3;
                o->substep = 2;
                func_8011D88C(o, 0x15);
            } else {
                E8 *t = (E8 *)o->d94;
                o->substep++;
                t += o->wae;
                o->timer = t->c;
                func_8011D88C(o, 0x12);
            }
        }
        break;
    case 4:
        AnimAdvance(o);
        if (--o->timer == -1) {
            o->state = 0;
            o->substep = 0;
            o->step++;
        }
        break;
    }
}
