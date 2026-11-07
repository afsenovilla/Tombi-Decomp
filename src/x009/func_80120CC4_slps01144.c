// VERSION SLPS_01144 x09 80120CC4 712
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#define U8(p, o) (*(unsigned char *)((p) + (o)))
#define U16(p, o) (*(unsigned short *)((p) + (o)))
#define S16(p, o) (*(short *)((p) + (o)))
#define S32(p, o) (*(int *)((p) + (o)))
#define PTR(p, o) (*(unsigned char **)((p) + (o)))
extern int D_8012B7B4, D_8012B7B8, D_8012B774;
extern short D_800A60CA;
extern void func_80020DE4(unsigned char *);
extern void func_800208B4(unsigned char *, int);
extern void func_8002122C(unsigned char *);
extern int func_80020E38(unsigned char *);
#include "spad.h"
#define SPSUM ({ int _p = 0x1F800000, _x, _q; asm("" : "=r"(_p) : "0"(_p)); _x = *(unsigned short *)(_p + 0x1f8); asm volatile("" : "=r"(_x) : "0"(_x) : "memory"); _q = 0x1F800000; _x + *(int *)(_q + 0x198); })

void func_80120CC4_slps01144(unsigned char *a)
{
    unsigned char *new_var;
    unsigned char *b;
    char t;
    switch (U8(a, 6)) {
    case 0:
        a[0] = 7;
        a[0x6a] = 1;
        S16(a, 0xb4) = 3;
        S16(a, 0x22) = 1;
        S16(a, 0x76) = 0;
        S16(a, 0x82) = 0;
        S16(a, 0xac) = 16;
        a[6]++;
        S32(a, 0x24) = D_8012B7B4;
        func_80020DE4(a);
        func_800208B4(a, 152);
        if ((a[3] & 0x7f) == 0) {
            b = PTR(a, 0x90);
            new_var = b + 0x40;
            S16(PTR(a, 0x40), 2) = U16(a, 0xb6) + U16(PTR(new_var, 0), 2);
            U16(a, 0x16) = U16(a, 0xb8) + U16(b, 0x16);
            S16(PTR(a, 0x44), 2) = U16(a, 0xba) + U16(PTR(b, 0x44), 2);
        } else {
            S16(PTR(a, 0x40), 2) = U16(a, 0xb6);
            U16(a, 0x16) = U16(a, 0xb8);
            S16(PTR(a, 0x44), 2) = U16(a, 0xba);
        }
        break;
    case 1:
        t = SPSUM;
        if ((t & 3) != 0)
            func_8002122C(a);
        func_80020E38(a);
        S16(a, 0x82) += 0x20;
        if (S16(a, 0x82) > 0x400)
            S16(a, 0x82) = 0x400;
        S16(a, 0x76) += S16(a, 0x82);
        if (S16(a, 0x76) >= 0x3001)
            S16(a, 0x22) = 0;
        S32(a, 0x14) -= S16(a, 0x82) << 8;
        if (S16(a, 0x22) == 0) {
            S16(a, 0xac) = 17;
            a[6]++;
            S32(a, 0x24) = D_8012B7B8;
            func_80020DE4(a);
        }
        t = SPSUM;
        if ((t & 0xf) == 0) {
            if (D_800A60CA + 152 < S16(a, 0x16)) {
                a[4] = 3;
                a[5] = 0;
                a[6] = 0;
            }
        }
        break;
    case 2:
        if (func_80020E38(a)) {
            S16(a, 0xb4) = 3;
            a[6] = 0;
            S16(a, 0xac) = 0;
            a[5]++;
            S32(a, 0x24) = D_8012B774;
            func_80020DE4(a);
        }
        t = SPSUM;
        if ((t & 3) != 0)
            func_8002122C(a);
        break;
    }
}
#endif
