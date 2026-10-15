// FUNC 80119610 148 X014
// MATCHING 80119610 148
// VERSION SLPM_80165 x00 8011753C 160
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x014 80119610 (func_80119610.c): culls with a 360 margin, state 2 advances.
#include "TOBJ.H"

extern int ObjCullRegister(TObj *);
extern void ObjFreeDup(TObj *);

extern void func_80020760(TObj *, int);
void func_80119610(TObj *o)
{
    unsigned char s = o->b04;
    switch (s) {
    case 0:
        o->b04 = s + 1;
        o->active = 2;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        break;
    case 1:
        func_80020760(o, 360);
        break;
    case 2:
        o->b04 = s + 1;
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
#else
#include "TOBJ.H"

extern int ObjCullRegister(TObj *);
extern void ObjFreeDup(TObj *);

void updateStaticSolid(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->active = 2;
        break;
    case 1:
        ObjCullRegister(o);
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
#endif
