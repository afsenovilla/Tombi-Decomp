// FUNC 8011610c 328 X006
// MATCHING 8011610c 328
// VERSION SLPS_01144 x06 801153C0 328
// VERSION SCUS_94236 x06 8011545C 328
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"
extern volatile int CAMERA_TARGET_Y_FIX;

void scrollCameraDepthStep(TObj *o)
{
    unsigned char flag = 0;
    int k, c, v;
    int d = CAMERA_TARGET_Y_FIX - 0x780000;
    d -= SPAD.if0;
    k = 0x500000;
    d += k;
    c = 0x340000;

    if (d < 0) {
        d += c;
        flag = 1;
        if (d >= 0) {
            o->b.raw = 0;
            return;
        }
    }
    if (flag == 0) {
        if (d > 0x10000) {
            if (d < o->b.raw) {
                if (d > 0x3ffff) { v = 0x40000; goto st; }
                o->b.raw = d;
            } else {
                if (o->b.raw < 0) o->b.raw = 0;
                d = 0x8000;
                v = o->b.raw + d;
                goto st;
            }
        } else {
            SPAD.if0 = CAMERA_TARGET_Y_FIX + k - 0x780000;
            return;
        }
    } else {
        if (d < -0x40000) {
            if (o->b.raw < d) {
                if (d > -0x40000) o->b.raw = d;
                else o->b.raw = -0x40000;
            } else {
                if (o->b.raw > 0) o->b.raw = 0;
                v = o->b.raw - 0x4000;
            st:
                o->b.raw = v;
            }
        } else {
            o->b.raw = d;
        }
    }
    *(volatile int *)&SPAD.if0 += o->b.raw;
}
#elif defined(VERSION_SCUS_94236)
// USA version of PAL x006 8011610C (scrollCameraDepthStep.c): SLPS_01144 branch of src/x006/scrollCameraDepthStep.c [-O2 -G0] (tools/demo_sweep.py)
#include "spad.h"
#include "TOBJ.H"
extern volatile int CAMERA_TARGET_Y_FIX;

void scrollCameraDepthStep(TObj *o)
{
    unsigned char flag = 0;
    int k, c, v;
    int d = CAMERA_TARGET_Y_FIX - 0x780000;
    d -= SPAD.if0;
    k = 0x500000;
    d += k;
    c = 0x340000;

    if (d < 0) {
        d += c;
        flag = 1;
        if (d >= 0) {
            o->b.raw = 0;
            return;
        }
    }
    if (flag == 0) {
        if (d > 0x10000) {
            if (d < o->b.raw) {
                if (d > 0x3ffff) { v = 0x40000; goto st; }
                o->b.raw = d;
            } else {
                if (o->b.raw < 0) o->b.raw = 0;
                d = 0x8000;
                v = o->b.raw + d;
                goto st;
            }
        } else {
            SPAD.if0 = CAMERA_TARGET_Y_FIX + k - 0x780000;
            return;
        }
    } else {
        if (d < -0x40000) {
            if (o->b.raw < d) {
                if (d > -0x40000) o->b.raw = d;
                else o->b.raw = -0x40000;
            } else {
                if (o->b.raw > 0) o->b.raw = 0;
                v = o->b.raw - 0x4000;
            st:
                o->b.raw = v;
            }
        } else {
            o->b.raw = d;
        }
    }
    *(volatile int *)&SPAD.if0 += o->b.raw;
}
#else
#include "spad.h"
#include "TOBJ.H"
extern volatile int CAMERA_TARGET_Y_FIX;

void scrollCameraDepthStep(TObj *o)
{
    unsigned char flag = 0;
    int k, c, v;
    int d = CAMERA_TARGET_Y_FIX - 0x800000;
    d -= SPAD.if0;
    k = 0x500000;
    d += k;
    c = 0x340000;

    if (d < 0) {
        d += c;
        flag = 1;
        if (d >= 0) {
            o->b.raw = 0;
            return;
        }
    }
    if (flag == 0) {
        if (d > 0x10000) {
            if (d < o->b.raw) {
                if (d > 0x3ffff) { v = 0x40000; goto st; }
                o->b.raw = d;
            } else {
                if (o->b.raw < 0) o->b.raw = 0;
                d = 0x8000;
                v = o->b.raw + d;
                goto st;
            }
        } else {
            SPAD.if0 = CAMERA_TARGET_Y_FIX + k - 0x800000;
            return;
        }
    } else {
        if (d < -0x40000) {
            if (o->b.raw < d) {
                if (d > -0x40000) o->b.raw = d;
                else o->b.raw = -0x40000;
            } else {
                if (o->b.raw > 0) o->b.raw = 0;
                v = o->b.raw - 0x4000;
            st:
                o->b.raw = v;
            }
        } else {
            o->b.raw = d;
        }
    }
    *(volatile int *)&SPAD.if0 += o->b.raw;
}
#endif
