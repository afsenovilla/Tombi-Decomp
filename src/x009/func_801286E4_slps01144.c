// VERSION SLPS_01144 x09 801286E4 944
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#define U8(p, o) (*(unsigned char *)((p) + (o)))
#define U16(p, o) (*(unsigned short *)((p) + (o)))
#define S16(p, o) (*(short *)((p) + (o)))
#define S32(p, o) (*(int *)((p) + (o)))
#define PTR(p, o) (*(unsigned char **)((p) + (o)))
#include "spad.h"
extern unsigned char D_800A7B78[];
extern unsigned char D_8009E954;
extern short D_800A7B8A;
extern int D_8012ADF4, D_8012ADF8;
extern unsigned char *D_8009DEE0;
extern unsigned char D_8009E4E7;
extern void func_80020DE4();
extern void func_8002122C(unsigned char *);
extern void func_80020E38(unsigned char *);
extern void func_80017D64(unsigned char *);
extern unsigned char *func_8002E3B4(int, int, int, int);

void func_801286E4_slps01144(unsigned char *a)
{
    unsigned char *P = D_800A7B78;
    unsigned char *e;
    int s_, t_;
    unsigned char *q_;
    switch (a[4]) {
    case 0:
        if (D_8009E954 == 0 || D_800A7B8A < 640) {
            a[4] = 3;
            break;
        }
        s_ = SPAD.i2e8;
        t_ = D_8012ADF4;
        S16(a, 0x1e) = 13;
        a[0xd] = 1;
        S16(a, 8) = 0x7809;
        a[0xa] = 2;
        a[0xf] = 5;
        S16(a, 0x2e) = 1;
        S16(a, 0x6c) = 16;
        S16(a, 0x6e) = 32;
        S16(a, 0x70) = 24;
        S16(a, 0x72) = 48;
        a[0x68] = 0;
        a[0] = 3;
        S32(a, 0x3c) = s_;
        S32(a, 0x24) = t_;
        func_80020DE4(a, s_);
        a[5] = 0;
        a[4]++;
        break;
    case 1:
        a[1] = 1;
        func_8002122C(a);
        switch (a[5]) {
        case 0:
            if (S16(P, 0x12) < S16(a, 0x12) + 16)
                S16(P, 0x12) = S16(a, 0x12) + 16;
            if (a[0x68]) {
                a[0x68] = 0;
                P[4] = 5;
                P[5] = 0;
                P[6] = 0;
                D_8009E4E7 = 1;
                S32(a, 0x24) = D_8012ADF8;
                func_80020DE4(a);
                PTR(a, 0x90) = func_8002E3B4(3, 1, 100, 156);
                a[5]++;
            }
            break;
        case 1:
            e = PTR(a, 0x90);
            if (e[4] >= 2) {
                e[4] = e[4] + 1;
                q_ = D_8009DEE0;
                asm volatile("" : "=r"(q_) : "0"(q_));
                S16(P, 0x2e) = 1;
                q_[9] = 5;
                P[0x9c] = 1;
                S16(P, 0x7c) = -1024;
                P[4] = 6;
                P[0x69] = 0;
                S16(P, 0xb2) = 0;
                S16(P, 0x80) = 0;
                S16(P, 0x82) = 0;
                S16(P, 0x7e) = 0;
                P[5] = 4;
                P[6] = 0;
                S16(a, 0x22) = 30;
                a[5]++;
            }
            break;
        case 2:
            if ((--U16(a, 0x22) << 16) == 0) {
                S16(a, 0x7c) = -512;
                S16(a, 0x7e) = -384;
                S16(a, 0x2e) = 0;
                S16(a, 0x22) = 50;
                a[5]++;
            }
            break;
        case 3:
            S32(a, 0x10) += S16(a, 0x7c) << 8;
            S32(a, 0x14) += S16(a, 0x7e) << 8;
            if (S16(a, 0x7e) < 256)
                S16(a, 0x7e) += 16;
            if ((--U16(a, 0x22) << 16) == 0)
                a[5]++;
            break;
        case 4:
            S16(a, 0x12) = U16(P, 0x12) + 32;
            S16(a, 0x16) = U16(P, 0x16) - 8;
            if (P[4] == 6)
                a[5]++;
            break;
        case 5:
            if (S16(P, 0x12) < 238)
                a[4]++;
            break;
        }
        func_80020E38(a);
        break;
    case 2:
        a[4]++;
        break;
    case 3:
        func_80017D64(a);
        break;
    }
}
#endif
