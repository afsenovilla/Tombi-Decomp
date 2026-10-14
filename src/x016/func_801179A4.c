// FUNC 801179a4 412 X016
// MATCHING 801179a4 412
#include "gcd.h"
#include "gv6.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern unsigned short D_1F8001FC;
extern void func_801174B8(TObj *);
extern void func_8011767C(TObj *);
extern void FUN_80018980(TObj *);

void func_801179A4(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->step = 0;
        o->state = 0;
        break;
    case 1:
        switch (o->step) {
        case 0:
            if (GCD.bce05 == 0xff) break;
            if ((unsigned short)((*(Fix16 **)&PLAYER.obj.h)->p.whole - 0x82) >= 0x28) break;
            if (PLAYER.obj.y.p.whole != -0x3d) break;
            if (GVARS.b40 != 0 && GVARS.b41 == 0x3e) {
                o->step = 2;
                o->state = 0;
                break;
            }
            if ((*(volatile unsigned short *)&GV6.s70 & 0x10) && (D_1F8001FC & 0x2000)) {
                GVARS.b3e = 1;
                o->step = 1;
                o->state = 0;
            }
            break;
        case 1:
            func_801174B8(o);
            break;
        case 2:
            func_8011767C(o);
            break;
        }
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        FUN_80018980(o);
        break;
    }
}
