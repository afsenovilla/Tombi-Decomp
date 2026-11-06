// VERSION SLPS_01144 x04 8011B39C 352
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"

extern Fix16 *PLAYER_H;
extern unsigned short D_8009E50A;
extern void ObjListPush_1F800224(TObj *);
extern void ObjFreeDup(TObj *);
#define S16A (*(short *)0x1F80016A)
#define S172 (*(short *)0x1F800172)
#define S176 (*(short *)0x1F800176)

void func_8011B39C_slps01144(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        switch (o->subtype) {
        case 0:
            break;
        case 1:
            o->d84 = 0;
            o->d88 = 0xc00;
            o->d8c = 0;
            break;
        case 2:
            o->d84 = 0;
            o->d88 = 0x800;
            o->d8c = 0;
            break;
        }
        break;
    case 1:
        switch (o->subtype) {
        case 0:
            if (S172 == 810) {
                if (S16A < 146) PLAYER_H->p.whole = 146;
            }
            goto vis;
        case 1:
            if (D_8009E50A == 2 && S176 < 1110) break;
            goto vis;
        case 2:
        vis:
            o->visible = 1;
            ObjListPush_1F800224(o);
            break;
        }
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
#endif
