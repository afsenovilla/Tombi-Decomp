// FUNC 8012049c 412 X014
// MATCHING 8012049c 412
#include "TOBJ.H"

typedef struct { int a; short t; short b; } E8;
typedef struct { short w[8]; } B4;

extern int AnimAdvance(TObj *o);
extern void func_8011D88C(TObj *o, int n);
extern void func_8011D948(TObj *o);
extern void func_80116C9C(TObj *o);
extern void func_80117428(TObj *o);
extern void func_8011F850(void);
extern void func_8011F888(void);
extern unsigned char D_8009D2B0, D_8009C93E, D_8009C942, D_8009C958;

void func_8012049C(TObj *o)
{
    B4 *b = (B4 *)&o->wb4;

    switch (o->substep) {
    case 0:
        o->active = 2;
        D_8009D2B0 = 0;
        D_8009C93E = 1;
        D_8009C942 = 1;
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
            func_80117428(o);
            D_8009C958 = 0;
            func_8011D948(o);
            func_80116C9C(o);
            o->substep++;
            func_8011F888();
        }
        break;
    case 3:
        if (AnimAdvance(o)) {
            E8 *e = (E8 *)o->d94;
            o->substep++;
            e += o->wae;
            o->timer = e->t;
            func_8011D88C(o, 0x12);
        }
        break;
    case 4:
        AnimAdvance(o);
        if (--o->timer == -1) {
            o->step = 0;
            o->state = 0;
            o->substep = 0;
            b->w[7] = 1;
        }
        break;
    }
}
