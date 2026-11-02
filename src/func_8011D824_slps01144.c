// near-twin of PAL updateStaticSolid (tools/jp_autofix.py)
// VERSION SLPS_01144 x00 8011D824 160
#if defined(VERSION_SLPS_01144)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"

extern int ObjCullRegister(TObj *);
extern void cullObjectToDrawListMargin(TObj *, int);
extern void ObjFreeDup(TObj *);

void func_8011D824_slps01144(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->active = 2;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        break;
    case 1:
        cullObjectToDrawListMargin(o, 0x168);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
#endif
#endif
