// FUNC 8011e40c 280 X001
// MATCHING 8011e40c 280
#include "spad.h"
#include "TOBJ.H"
extern void FUN_80018cf0(TObj *);
extern void ObjFreeDup(TObj *);
extern void l4Type16ObjectStep(TObj *, short);

void updateL4Type16Object(TObj *o)
{
    unsigned char *p;
    short n;

    switch (o->b04) {
    case 0:
        p = (unsigned char *)o + 0xa5;
        n = *(unsigned short *)(o->da0 + 4);
        do {
            *p++ = 0;
        } while (--n);
        o->b68 = 0;
        o->b6b = 0;
        o->b6a = 0;
        o->velV = 0;
        o->d34 = 0;
        o->step = 0;
        o->b04++;
        break;
    case 1:
        if ((*(short*)((char*)&SPAD.iec + 2)) < 0x4b0) {
            o->visible = 1;
            FUN_80018cf0(o);
        }
        l4Type16ObjectStep(o, (short)((*(unsigned short*)((char*)&SPAD.i168 + 2)) - o->a.p.whole) >> 4);
        o->b68 = 0;
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
