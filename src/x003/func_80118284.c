// FUNC 80118284 740 X003
// MATCHING 80118284 740
#include "TOBJ.H"
typedef struct { short x, y, w, h; } RECT;
extern unsigned short D_1F8001F8;
extern unsigned char D_801356E8[], D_80135700[], D_80135710[], D_801356A0[], D_801356B0[], D_801356C0[], D_801356D4[];
extern char D_801353A0[], D_801354E0[], D_801355C0[], D_80134FE0[], D_801350A0[], D_80135180[], D_80135280[];
extern void LoadImage(RECT *, void *);

#define B(o, n) (((unsigned char *)(o))[n])

void func_80118284(TObj *o)
{
    RECT r;

    if ((D_1F8001F8 & 7) == 0) {
        r.x = 0xf0;
        r.y = 0x1ea;
        r.w = 0x10;
        r.h = 1;
        LoadImage(&r, D_801353A0 + D_801356E8[B(o, 0x169)] * 32);
        if (++B(o, 0x169) >= 0x16) B(o, 0x169) = 0;
        r.x = 0xf0;
        r.y = 0x1eb;
        r.w = 0x10;
        r.h = 1;
        LoadImage(&r, D_801354E0 + D_80135700[B(o, 0x16a)] * 32);
        if (++B(o, 0x16a) >= 0x10) B(o, 0x16a) = 0;
        r.x = 0xf0;
        r.y = 0x1f5;
        r.w = 0x10;
        r.h = 1;
        LoadImage(&r, D_801355C0 + D_80135710[B(o, 0x16b)] * 32);
        if (++B(o, 0x16b) >= 0x10) B(o, 0x16b) = 0;
        r.x = 0x90;
        r.y = 0x1e0;
        r.w = 0x10;
        r.h = 1;
        LoadImage(&r, D_80134FE0 + D_801356A0[B(o, 0x165)] * 32);
        if (++B(o, 0x165) >= 0xe) B(o, 0x165) = 0;
        r.x = 0x90;
        r.y = 0x1e1;
        r.w = 0x10;
        r.h = 1;
        LoadImage(&r, D_801350A0 + D_801356B0[B(o, 0x166)] * 32);
        if (++B(o, 0x166) >= 0x10) B(o, 0x166) = 0;
        r.x = 0x90;
        r.y = 0x1e2;
        r.w = 0x10;
        r.h = 1;
        LoadImage(&r, D_80135180 + D_801356C0[B(o, 0x167)] * 32);
        if (++B(o, 0x167) >= 0x12) B(o, 0x167) = 0;
        r.x = 0x90;
        r.y = 0x1e3;
        r.w = 0x10;
        r.h = 1;
        LoadImage(&r, D_80135280 + D_801356D4[B(o, 0x168)] * 32);
        if (++B(o, 0x168) >= 0x14) B(o, 0x168) = 0;
    }
}
