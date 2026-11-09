// FUNC 8002052c 172 MAIN0
// MATCHING 8002052c 172
// VERSION SLPS_01144 slps01144 800214A4 172
// VERSION SLPM_80165 slpm80165 80020C14 172
// VERSION SCUS_94236 scus94236 800230BC 172
// VERSION BETA_980203 beta980203 800229BC 172
#if defined(VERSION_BETA_980203)
// Beta version of PAL main0 8002052C (despawnObjectOffscreenMargin.c): SCUS_94236 C of src/despawnObjectOffscreenMargin.c (despawnObjectOffscreenMargin, twin score 1.000), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
#include "camera.h"
#if defined(VERSION_SLPS_01144) || defined(VERSION_SCUS_94236)
// Japanese version of PAL main0 8002052C (despawnObjectOffscreenMargin.c): 160 -> 152.
#include "spad.h"
#include "TOBJ.H"

void despawnObjectOffscreenMargin(TObj *o, int a)
{
    short y;
    if (o->active == 0) return;
    y = o->y.p.whole;
    if (CAMERA_SCROLL.maxY + 0x98 > y) {
        if ((unsigned short)(o->h->p.whole - (*(unsigned short*)((char*)&SPAD.i174 + 2)) + a) < (short)a * 2 + 0x140 &&
            (unsigned short)(SPAD.s186 - y + a) < (short)a * 2 + 0xf0) return;
    }
    o->b04 = 3;
    o->active = 2;
    o->visible = 0;
}
#elif defined(VERSION_SLPM_80165)
// Demo version of PAL main0 8002052C (func_8002052C.c): bottom margin 0x98 (PAL 0xa0).
#include "spad.h"
#include "TOBJ.H"

void func_8002052C(TObj *o, int a)
{
    short y;
    if (o->active == 0) return;
    y = o->y.p.whole;
    if (CAMERA_SCROLL.maxY + 0x98 > y) {
        if ((unsigned short)(o->h->p.whole - (*(unsigned short*)((char*)&SPAD.i174 + 2)) + a) < (short)a * 2 + 0x140 &&
            (unsigned short)(SPAD.s186 - y + a) < (short)a * 2 + 0xf0) return;
    }
    o->b04 = 3;
    o->active = 2;
    o->visible = 0;
}
#else
#include "spad.h"
#include "TOBJ.H"

void despawnObjectOffscreenMargin(TObj *o, int a)
{
    short y;
    if (o->active == 0) return;
    y = o->y.p.whole;
    if (CAMERA_SCROLL.maxY + 0x98 > y) {
        if ((unsigned short)(o->h->p.whole - (*(unsigned short*)((char*)&SPAD.i174 + 2)) + a) < (short)a * 2 + 0x140 &&
            (unsigned short)(SPAD.s186 - y + a) < (short)a * 2 + 0xf0) return;
    }
    o->b04 = 3;
    o->active = 2;
    o->visible = 0;
}
#endif
#else
#include "camera.h"
#if defined(VERSION_SLPS_01144) || defined(VERSION_SCUS_94236)
// Japanese version of PAL main0 8002052C (despawnObjectOffscreenMargin.c): 160 -> 152.
#include "spad.h"
#include "TOBJ.H"

void despawnObjectOffscreenMargin(TObj *o, int a)
{
    short y;
    if (o->active == 0) return;
    y = o->y.p.whole;
    if (CAMERA_SCROLL.maxY + 0x98 > y) {
        if ((unsigned short)(o->h->p.whole - (*(unsigned short*)((char*)&SPAD.i174 + 2)) + a) < (short)a * 2 + 0x140 &&
            (unsigned short)(SPAD.s186 - y + a) < (short)a * 2 + 0xf0) return;
    }
    o->b04 = 3;
    o->active = 2;
    o->visible = 0;
}
#elif defined(VERSION_SLPM_80165)
// Demo version of PAL main0 8002052C (func_8002052C.c): bottom margin 0x98 (PAL 0xa0).
#include "spad.h"
#include "TOBJ.H"

void func_8002052C(TObj *o, int a)
{
    short y;
    if (o->active == 0) return;
    y = o->y.p.whole;
    if (CAMERA_SCROLL.maxY + 0x98 > y) {
        if ((unsigned short)(o->h->p.whole - (*(unsigned short*)((char*)&SPAD.i174 + 2)) + a) < (short)a * 2 + 0x140 &&
            (unsigned short)(SPAD.s186 - y + a) < (short)a * 2 + 0xf0) return;
    }
    o->b04 = 3;
    o->active = 2;
    o->visible = 0;
}
#else
#include "spad.h"
#include "TOBJ.H"

void despawnObjectOffscreenMargin(TObj *o, int a)
{
    short y;
    if (o->active == 0) return;
    y = o->y.p.whole;
    if (CAMERA_SCROLL.maxY + 0xa0 > y) {
        if ((unsigned short)(o->h->p.whole - (*(unsigned short*)((char*)&SPAD.i174 + 2)) + a) < (short)a * 2 + 0x140 &&
            (unsigned short)(SPAD.s186 - y + a) < (short)a * 2 + 0xf0) return;
    }
    o->b04 = 3;
    o->active = 2;
    o->visible = 0;
}
#endif
#endif
