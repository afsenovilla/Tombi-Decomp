// near-twin of PAL playerInitAreaHook (tools/jp_autofix.py)
// VERSION SLPS_01144 x03 8011CB3C 244
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
extern int D_800A0C7C;
extern unsigned short GAME_SELECTED_SECTION, D_8009E52A;
extern unsigned char D_8009E96B, D_8009E4E2, D_8009EE6A;
void func_8011CB3C(TObj *o)
{
    unsigned char *p;
    D_800A0C7C = 0;
    if ((GAME_SELECTED_SECTION == 1 || GAME_SELECTED_SECTION == 5) && D_8009E52A == 1) {
        o->b04 = 5;
        o->step = 9;
    }
    if ((GAME_SELECTED_SECTION == 0 || GAME_SELECTED_SECTION == 4) && D_8009E96B != 0xff && D_8009E52A == 4) {
        D_8009E4E2 = 1;
        o->d60 = 1;
        o->active = 1;
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
        o->substep = 0;
    }
    p = &D_8009EE6A;
    if (*p == 1) {
        o->active = 5;
        o->b04 = 5;
        o->step = 0x40;
        o->visible = 0;
        D_8009E4E2 = 1;
        *p = 2;
    }
}
#endif
