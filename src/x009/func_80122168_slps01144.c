// VERSION SLPS_01144 x09 80122168 892
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#define U8(p, o) (*(unsigned char *)((p) + (o)))
#define U16(p, o) (*(unsigned short *)((p) + (o)))
#define S16(p, o) (*(short *)((p) + (o)))
#define S32(p, o) (*(int *)((p) + (o)))
#define PTR(p, o) (*(unsigned char **)((p) + (o)))
#include "spad.h"
extern unsigned char D_800A7B78[];
extern unsigned char D_8009EE59;
extern char D_8007170C[], D_80071718[];
extern int D_8012B780;
extern void func_8002085C(unsigned char *);
extern void func_80020DE4(unsigned char *);
extern void func_80020E38(unsigned char *);
extern void func_80020A2C(unsigned char *);
extern void func_80020A98(unsigned char *);
extern int func_80020958(void);
extern void func_8001E2D4(int);
extern void func_80120BD0_slps01144(unsigned char *);
extern void func_8012170C_slps01144(unsigned char *);
extern short func_80041224(unsigned char *, short, short, short);
extern short func_80040E40(unsigned char *, short, short);

#define GROUND(a) \
    if (a[0x69] == 1 || func_80040E40(a, S16(PTR(a, 0x40), 2), U16(a, 0x16) + 16)) \
        a[0x69] = 0;

static __inline__ int chk(unsigned char *a)
{
    if (S16(a, 0xb4) == 3) return 0; return S16(a, 0xb4) != D_8009EE59;
}
#define CHK(a) \
    if (chk(a)) { \
        a[5] = 3; \
        a[6] = 0; \
    }

void func_80122168_slps01144(unsigned char *a)
{
    short old;
    int t, r, d, c;
    switch (a[6]) {
    case 0:
        func_8002085C(a);
        S16(a, 0x22) = 200;
        S32(a, 0x28) = (int)D_8007170C;
        S16(a, 0xac) = 3;
        a[6]++;
        S32(a, 0x24) = D_8012B780;
        func_80020DE4(a);
        break;
    case 1:
        func_80020E38(a);
        if ((D_800A7B78[0] & 2) == 0)
            func_80020A2C(a);
        func_80120BD0_slps01144(a);
        old = U16(a, 0x2e);
        func_8002085C(a);
        if (old != U16(a, 0x2e)) {
            S16(a, 0x80) = 30;
            a[6] = 2;
        }
        if ((U8(a, 0x9d) & 2) && U16(a, 0x2e) == (U8(a, 0x9d) & 1)) {
            r = 1;
        } else {
            d = (U16(a, 0x2e) & 1) ? -16 : 16;
            r = func_80041224(a, S16(PTR(a, 0x40), 2) + d, S16(a, 0x16), U16(a, 0x2e)) != 0;
        }
        if (r)
            a[6] = 2;
        GROUND(a)
        if (S16(a, 0x22) == 0) {
            S16(a, 0x22) = 200;
            a[6] = 3;
        } else {
            S16(a, 0x22)--;
        }
        CHK(a)
        break;
    case 2:
        if ((short)--U16(a, 0x80) == -1)
            a[6] = 1;
        func_80020E38(a);
        func_80020A98(a);
        GROUND(a)
        CHK(a)
        break;
    case 3:
        a[6] = 0;
        func_8012170C_slps01144(a);
        break;
    case 4:
        func_8002085C(a);
        a[6] = 1;
        S32(a, 0x28) = (int)D_80071718;
        S16(a, 0xac) = 3;
        S32(a, 0x24) = D_8012B780;
        func_80020DE4(a);
        if (a[1])
            func_8001E2D4(150);
        break;
    }
    if (a[1]) {
        t = SPAD.s1f8 + SPAD.i198;
        if ((t & 0x7f) == 0) {
            if (func_80020958() & 1)
                func_8001E2D4(150);
        } else if ((t & 0x3f) == 0) {
            func_8001E2D4(150);
        }
    }
    if (S16(a, 0x20))
        S16(a, 0x20)--;
}
#endif
