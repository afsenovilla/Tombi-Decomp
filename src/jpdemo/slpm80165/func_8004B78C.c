// JPDEMO slpm80165 8004B78C 360
// Demo version of PAL main0 80045684 (FUN_80045684.c): hit step 2 (PAL 7), no b[0] != 3 test, attached object at
// +0xd0 (PAL +0xe4).
extern short FUN_80043260(unsigned char *, unsigned char *);
extern void FUN_8001f96c(int, int, int, int);
extern short DAT_1f80019e;

#define U(p, o) (*(unsigned short *)((p) + (o)))
#define S(p, o) (*(short *)((p) + (o)))

static __inline__ short HIT(unsigned char *a, unsigned char *b)
{
    if ((unsigned short)(U(*(unsigned char **)(a + 0x44), 2) - U(*(unsigned char **)(b + 0x44), 2) + 0x2d) >= 0x5b)
        return 0;
    if ((unsigned short)(U(*(unsigned char **)(a + 0x40), 2) - U(*(unsigned char **)(b + 0x40), 2) + (U(a, 0x6c) + U(b, 0x6c))) > S(a, 0x6e) + S(b, 0x6e))
        return 0;
    if ((unsigned short)(U(a, 0x16) - U(b, 0x16) + (U(a, 0x70) + U(b, 0x70))) > S(a, 0x72) + S(b, 0x72))
        return 0;
    return 1;
}

void FUN_80045684(unsigned char *a, unsigned char *b)
{
    short r;
    int h;
    if (b[0] & 2) {
        h = HIT(a, b);
        if (h) {
            b[0] = 2;
            b[4] = 2;
            b[5] = 2;
        }
    } else {
        r = FUN_80043260(a, b);
        if (r != 0 && r == 1 && a[0xac] == 1) {
            b[0] = 6;
            b[4] = 2;
            b[5] = 0;
            *(unsigned char **)(a + 0xd0) = b;
            a[0xac] = 2;
            FUN_8001f96c(2, S(a, 0x12), S(a, 0x16), S(a, 0x1a));
            DAT_1f80019e = 0;
        }
    }
}
