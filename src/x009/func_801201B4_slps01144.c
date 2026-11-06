// VERSION SLPS_01144 x09 801201B4 568
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#define U8(p, o) (*(unsigned char *)((p) + (o)))
#define U16(p, o) (*(unsigned short *)((p) + (o)))
#define S16(p, o) (*(short *)((p) + (o)))
#define PTR(p, o) (*(unsigned char **)((p) + (o)))
extern short func_8004E858(unsigned char *, unsigned char *);
extern void func_8004D980(unsigned char *, int);
extern void func_800208E4(int, int, int, int);
#define SPAD_19E (*(short *)0x1F80019E)

void func_801201B4_slps01144(unsigned char *a, unsigned char *b)
{
    short r;
    if (U8(a, 0x9e))
        return;
    r = func_8004E858(a, b);
    if (r == -1)
        return;
    switch (U8(a, 0xac)) {
    case 1:
        if (r == 3) {
            b[0] = 2;
            b[4] = 2;
            b[5] = 1;
            b[6] = 0;
            b[0x6a] = 0;
            PTR(a, 0xd4) = b;
            U8(a, 0xac) = 2;
            func_800208E4(2, S16(a, 0x12), S16(a, 0x16), S16(a, 0x1a));
            break;
        }
    case 0:
    case 3:
        if ((b[0] & 2) == 0 && (a[0] & 2) == 0) {
            if (r == 2 && b[0x6a] == 1) {
                if (U8(a, 0xac) == 1)
                    U8(a, 0xac) = 0;
                b[0] = 4;
                b[0x6a] = 2;
                a[0] = 2;
                a[4] = 2;
                a[5] = 1;
                a[6] = 0;
                a[7] = 0;
                func_8004D980(a, 1);
            } else if (U8(a, 0x9c) == 0 || r != (U16(a, 0x2e) & 1)) {
                int ah, bh;
                a[0] = 2;
                bh = S16(PTR(b, 0x40), 2);
                ah = S16(PTR(a, 0x40), 2);
                a[4] = 2;
                a[5] = 0;
                a[6] = 0;
                U16(a, 0x2e) = ah < bh;
                func_8004D980(a, 1);
            }
        }
        break;
    case 2:
        if (b[0] != 3 && b[0x6a] != U8(a, 0xac)) {
            int ah, bh;
            b[0] = 3;
            b[0x6a] = 0;
            b[4] = 2;
            ah = S16(PTR(a, 0x40), 2);
            bh = S16(PTR(b, 0x40), 2);
            b[5] = 0;
            b[6] = 0;
            U16(b, 0x2e) = bh < ah;
        }
        break;
    }
    SPAD_19E = 0;
}
#endif
