// FUNC 8011fe14 364 X014
// MATCHING 8011fe14 364
#include "TOBJ.H"

typedef struct { short a, b, c, d; } E8;

extern int AnimAdvance(TObj *);
extern void func_8011D88C(TObj *, int);
extern void func_8011F850(void);
extern void func_8011F888(void);
extern void func_80123698(TObj *);
extern void func_8011D948(TObj *);
extern void func_80116C9C(TObj *);

void func_8011FE14(TObj *o)
{
    switch (o->substep) {
    case 0:
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
            func_80123698(o);
            func_8011D948(o);
            func_80116C9C(o);
            o->timer = 0x14;
            o->substep++;
            func_8011F888();
        }
        break;
    case 3:
        if (AnimAdvance(o)) {
            E8 *t = (E8 *)o->d94;
            o->substep++;
            t += o->wae;
            o->timer = t->c;
            func_8011D88C(o, 0x12);
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
