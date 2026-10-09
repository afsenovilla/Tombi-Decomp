// FUNC 8011fba0 628 X014
// MATCHING 8011fba0 628
#include "TOBJ.H"

typedef struct { short a, b, c, d; } E8;

extern int AnimAdvance(TObj *);
extern unsigned int FUN_8001f9e0(void);
extern unsigned short D_8009C962;
extern unsigned char D_80126110[];
extern void func_8011D88C(TObj *, int);
extern void func_8011F850(void);
extern void func_8011F888(void);
extern void FUN_8001f8e4(TObj *);
extern void func_8012441C(TObj *, unsigned char);
extern void func_80120638(TObj *);
extern void func_8011D948(TObj *);
extern void func_80116C9C(TObj *);

void func_8011FBA0(TObj *o)
{
    unsigned short *q = (unsigned short *)&o->wb4;

    switch (o->substep) {
    case 0:
        if (o->d38 < 0x80 && D_8009C962 != 7) {
            q[5] = D_80126110[FUN_8001f9e0() & 0xf];
            if (q[5] == 0)
                q[4] = 2;
            else
                q[4] = 0;
        } else {
            q[5] = 0;
            q[4] = 2;
        }
        q[3] = (unsigned char)o->d38;
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
            FUN_8001f8e4(o);
            if (q[5] == 0)
                func_8012441C(o, q[3]);
            else
                func_80120638(o);
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
                if (q[4] == 1)
                    q[3] = (o->d38 + 0x10) & 0xff;
                else
                    q[3] = (o->d38 - 0x10) & 0xff;
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
