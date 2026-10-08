// FUNC 80123a1c 328 X000
// MATCHING 80123a1c 328
#include "tobj.h"
extern void f0(TObj *), f2(TObj *), f3(TObj *);
extern int f1(TObj *);
extern unsigned char c4553;
extern unsigned char sel[];
extern void (*fn[])(TObj *);
void FUN_80123a1c(TObj *o)
{
    int s;
    short t;
    switch (o->b04) {
    case 0:
        f0(o);
        o->timer = 8;
        o->step = 0;
        o->b04++;
        break;
    case 1:
        s = o->step;
        if (s != 1) {
            if (1 < s || s != 0 || --o->timer != 0)
                goto done;
        } else {
            if (f1(o) == 0) goto done;
            c4553 = 4;
        }
        o->step++;
done:
        f2(o);
        break;
    case 2:
        fn[sel[o->subtype]](o);
        break;
    case 3:
        f3(o);
        break;
    }
}
