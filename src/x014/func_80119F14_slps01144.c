// VERSION SLPS_01144 x14 80119F14 284
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#define U8(p, o) (*(unsigned char *)((p) + (o)))
#define S16(p, o) (*(short *)((p) + (o)))
#define PTR(p, o) (*(unsigned char **)((p) + (o)))
extern unsigned short D_8009E50A;
extern unsigned char *D_8009DEE0;
extern short boxesOverlap(unsigned char *, unsigned char *);
extern void decreaseObjectTimer(unsigned char *, int);

void func_80119F14_slps01144(unsigned char *a, unsigned char *b)
{
    if (U8(a, 0) & 2)
        return;
    if (!boxesOverlap(a, b))
        return;
    if (D_8009E50A == 7) {
        U8(a, 0) = 2;
        S16(a, 0x2e) = S16(PTR(b, 0x40), 2) > S16(PTR(a, 0x40), 2);
        U8(a, 4) = 2;
        U8(a, 5) = 0;
        U8(a, 6) = 0;
        decreaseObjectTimer(a, 1);
    } else {
        if (S16(PTR(a, 0x40), 2) > S16(PTR(b, 0x40), 2)) {
            S16(a, 0x2e) = 1;
            S16(a, 0x7c) = 0x200;
        } else {
            S16(a, 0x2e) = 0;
            S16(a, 0x7c) = -0x200;
        }
        U8(D_8009DEE0, 9) = 10;
        U8(a, 4) = 6;
        U8(a, 5) = 7;
        U8(a, 6) = 0;
    }
    U8(b, 0) = 2;
    U8(b, 4) = 2;
    U8(b, 5) = 0;
    U8(b, 6) = 0;
    *(short *)0x1F80019E = 0;
}
#endif
