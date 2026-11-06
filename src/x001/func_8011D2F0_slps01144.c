// VERSION SLPS_01144 x01 8011D2F0 288
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"

extern void ObjListPush_1F800224(TObj *);
extern void ObjFreeDup(TObj *);
extern void func_8011D410(TObj *, int);

void func_8011D2F0_slps01144(TObj *o)
{
    unsigned char s = o->b04;
    short n;
    short *p;

    switch (s) {
    case 0:
        p = (short *)o->da0;
        p += 2;
        n = *p;
        p += 2;
        do {
            p[17] = 0;
            n--;
            p += 26;
        } while (n);
        o->b68 = 0;
        o->b6b = 0;
        o->b6a = 0;
        o->velV = 0;
        o->d34 = 0;
        o->step = 0;
        o->b04++;
        break;
    case 1:
        if (((short *)&SPAD.iec)[1] < 1200) {
            o->visible = 1;
            ObjListPush_1F800224(o);
        }
        func_8011D410(o, (short)(((unsigned short *)&SPAD.i168)[1] - (unsigned short)o->a.p.whole) >> 4);
        o->b68 = 0;
        break;
    case 2:
        o->b04 = s + 1;
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
#endif
