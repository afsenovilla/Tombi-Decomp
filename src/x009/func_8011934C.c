// FUNC 8011934c 1056 X009
// MATCHING 8011934c 1056
#include "TOBJ.H"
extern void *D_8012E9C0[];
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern void FUN_8001e4f0(int);
extern void func_80118970(TObj *);

#define SET(n)                          \
    {                                   \
        o->b0c = n;                     \
        o->anim = D_8012E9C0[(n) - 1];  \
        AnimLoadDuration(o);            \
        func_80118970(o);               \
    }

void func_8011934C(TObj *o)
{
    unsigned char v;

    switch (o->step) {
    case 0:
        switch (o->state) {
        case 0:
            v = (*(TObj **)&o->wa8)->b6a;
            if (v == 1) {
                o->state = 1;
                SET(4);
            } else if (v == 2) {
                o->state = 2;
                SET(9);
            }
            break;
        case 1:
            if (AnimAdvance(o)) {
                o->step = 1;
                o->state = 0;
                SET(2);
            }
            break;
        case 2:
            if (AnimAdvance(o)) {
                o->step = 2;
                o->state = 0;
                SET(3);
            }
            break;
        }
        break;
    case 1:
        switch (o->state) {
        case 0:
            v = (*(TObj **)&o->wa8)->b6a;
            if (v == 1) {
                if (o->visible) {
                    AnimAdvance(o);
                    if (++o->timer % 240 == 0)
                        FUN_8001e4f0(0x91);
                }
            } else if (v == 3) {
                o->state = 1;
                SET(5);
            } else if (v == 2) {
                o->state = 2;
                SET(6);
            }
            break;
        case 1:
            if (AnimAdvance(o)) {
                o->step = 0;
                o->state = 0;
                SET(1);
            }
            break;
        case 2:
            if (AnimAdvance(o)) {
                o->step = 2;
                o->state = 0;
                SET(3);
            }
            break;
        }
        break;
    case 2:
        switch (o->state) {
        case 0:
            if ((*(TObj **)&o->wa8)->b6a == 2 && o->visible) {
                AnimAdvance(o);
                if (++o->timer % 240 == 0)
                    FUN_8001e4f0(0x92);
            }
            v = (*(TObj **)&o->wa8)->b6a;
            if (v == 3) {
                o->state = 1;
                SET(8);
            } else if (v == 1) {
                o->state = 2;
                SET(7);
            }
            break;
        case 1:
            if (AnimAdvance(o)) {
                o->step = 0;
                o->state = 0;
                SET(1);
            }
            break;
        case 2:
            if (AnimAdvance(o)) {
                o->step = 1;
                o->state = 0;
                SET(2);
            }
            break;
        }
        break;
    }
}
