// VERSION PCPX_96084 tomba 8003C264 152
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version: SLPS_01144 C of src/func_8011D824_slps01144.c (func_8011D824_slps01144, twin score 0.687), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
#include "TOBJ.H"

extern int ObjCullRegister(TObj *);
extern void cullObjectToDrawListMargin(TObj *, int);
extern void ObjFreeDup(TObj *);

void func_8011D824_slps01144(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
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
