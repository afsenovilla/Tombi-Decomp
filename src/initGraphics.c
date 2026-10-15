// FUNC 80016b50 344 MAIN0
// MATCHING 80016b50 344
// VERSION SLPS_01144 slps01144 800160EC 344
// VERSION SLPM_80165 slpm80165 800157BC 344
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL main0 80016B50 (initGraphics.c): 128 -> 112.
#include "globals.h"
typedef struct { short x, y, w, h; } R;
extern short A0f8, A0fa, A0fc, A0fe, A100, A102, A104, A106, A108;
extern int A114, A110, A10c;
extern short B0e2, B0e6, B0ea, B0ee, B0f2, B0f6;
extern char C0c0[], C118[];
extern short G1f4;
extern void *G1e0;
extern void FUN_80063fdc(int, int), FUN_80063ffc(int), getBaseMatrix(char *), initLighting(char *);
extern void initDisplay(int, int, int), FUN_8005f290(R *, int, int, int), FUN_8005f604(char *, int);

void initGraphics(void)
{
    R r;
    FUN_80063fdc(0xa0, 0x70);
    FUN_80063ffc(0x220);
    A0f8 = 0x1000;
    A0fa = 0;
    A0fc = 0;
    A0fe = 0;
    A100 = 0x1000;
    A102 = 0;
    A104 = 0;
    A106 = 0;
    A108 = 0x1000;
    A114 = 0;
    A110 = 0;
    A10c = 0;
    getBaseMatrix(C0c0);
    B0e2 = 0;
    B0e6 = 0;
    B0ea = -0x220;
    B0ee = 0;
    B0f2 = 0;
    B0f6 = 0;
    initLighting(C118);
    initDisplay(0x60, 0x97, 0xff);
    r.w = 0x400;
    r.x = 0;
    r.y = 0;
    r.h = 0x200;
    FUN_8005f290(&r, 0, 0, 0);
    FUN_8005f604(OT_FRAMEBUFFER, 0x328);
    FUN_8005f604(OT_FRAMEBUFFER + 0xd10, 0x328);
    G1f4 = 0;
    G1e0 = OT_FRAMEBUFFER;
}
#elif defined(VERSION_SLPM_80165)
// Demo version of PAL main0 80016B50 (initGraphics.c): geometry offset y 0x70 and 0x808-entry ordering tables (PAL
// 0x80, 0x328).
#include "globals.h"
typedef struct { short x, y, w, h; } R;
extern short A0f8, A0fa, A0fc, A0fe, A100, A102, A104, A106, A108;
extern int A114, A110, A10c;
extern short B0e2, B0e6, B0ea, B0ee, B0f2, B0f6;
extern char C0c0[], C118[];
extern short G1f4;
extern void *G1e0;
extern void FUN_80063fdc(int, int), FUN_80063ffc(int), getBaseMatrix(char *), initLighting(char *);
extern void initDisplay(int, int, int), FUN_8005f290(R *, int, int, int), FUN_8005f604(char *, int);

void initGraphics(void)
{
    R r;
    FUN_80063fdc(0xa0, 0x70);
    FUN_80063ffc(0x220);
    A0f8 = 0x1000;
    A0fa = 0;
    A0fc = 0;
    A0fe = 0;
    A100 = 0x1000;
    A102 = 0;
    A104 = 0;
    A106 = 0;
    A108 = 0x1000;
    A114 = 0;
    A110 = 0;
    A10c = 0;
    getBaseMatrix(C0c0);
    B0e2 = 0;
    B0e6 = 0;
    B0ea = -0x220;
    B0ee = 0;
    B0f2 = 0;
    B0f6 = 0;
    initLighting(C118);
    initDisplay(0x60, 0x97, 0xff);
    r.w = 0x400;
    r.x = 0;
    r.y = 0;
    r.h = 0x200;
    FUN_8005f290(&r, 0, 0, 0);
    FUN_8005f604(OT_FRAMEBUFFER, 0x808);
    FUN_8005f604(OT_FRAMEBUFFER + 0x2090, 0x808);
    G1f4 = 0;
    G1e0 = OT_FRAMEBUFFER;
}
#else
#include "globals.h"
typedef struct { short x, y, w, h; } R;
extern short A0f8, A0fa, A0fc, A0fe, A100, A102, A104, A106, A108;
extern int A114, A110, A10c;
extern short B0e2, B0e6, B0ea, B0ee, B0f2, B0f6;
extern char C0c0[], C118[];
extern short G1f4;
extern void *G1e0;
extern void FUN_80063fdc(int, int), FUN_80063ffc(int), getBaseMatrix(char *), initLighting(char *);
extern void initDisplay(int, int, int), FUN_8005f290(R *, int, int, int), FUN_8005f604(char *, int);

void initGraphics(void)
{
    R r;
    FUN_80063fdc(0xa0, 0x80);
    FUN_80063ffc(0x220);
    A0f8 = 0x1000;
    A0fa = 0;
    A0fc = 0;
    A0fe = 0;
    A100 = 0x1000;
    A102 = 0;
    A104 = 0;
    A106 = 0;
    A108 = 0x1000;
    A114 = 0;
    A110 = 0;
    A10c = 0;
    getBaseMatrix(C0c0);
    B0e2 = 0;
    B0e6 = 0;
    B0ea = -0x220;
    B0ee = 0;
    B0f2 = 0;
    B0f6 = 0;
    initLighting(C118);
    initDisplay(0x60, 0x97, 0xff);
    r.w = 0x400;
    r.x = 0;
    r.y = 0;
    r.h = 0x200;
    FUN_8005f290(&r, 0, 0, 0);
    FUN_8005f604(OT_FRAMEBUFFER, 0x328);
    FUN_8005f604(OT_FRAMEBUFFER + 0xd10, 0x328);
    G1f4 = 0;
    G1e0 = OT_FRAMEBUFFER;
}
#endif
