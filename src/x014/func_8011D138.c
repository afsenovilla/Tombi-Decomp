// FUNC 8011d138 420 X014
// MATCHING 8011d138 420
#define U8(p, o) (*(unsigned char *)((p) + (o)))
#define U16(p, o) (*(unsigned short *)((p) + (o)))
#define S16(p, o) (*(short *)((p) + (o)))
#define PTR(p, o) (*(unsigned char **)((p) + (o)))
extern short func_800435E0(unsigned char *, unsigned char *);
extern void FUN_8004258c(unsigned char *, int);
extern void FUN_8001f96c(int, int, int, int);
extern unsigned char D_1F8001A4;

void func_8011D138(unsigned char *a, unsigned char *b)
{
    short r = func_800435E0(a, b);
    if (r == -1)
        return;
    switch (U8(a, 0xac)) {
    case 1:
        if (r < 3) {
            b[0] = 4;
            b[4] = 2;
            b[5] = 1;
            b[6] = 0;
            b[0x69] = 0;
            U16(b, 0x2e) = U16(a, 0x2e) & 1;
            PTR(a, 0xe4) = b;
            U8(a, 0xac) = 2;
            FUN_8001f96c(2, S16(a, 0x12), S16(a, 0x16), S16(a, 0x1a));
            break;
        }
    case 0:
    case 3:
        if ((b[0] & 2) == 0 && D_1F8001A4 == 0 && (a[0] & 2) == 0) {
            int ah, bh;
            a[0] = 2;
            bh = S16(PTR(b, 0x40), 2);
            ah = S16(PTR(a, 0x40), 2);
            a[4] = 2;
            a[5] = 0;
            a[6] = 0;
            U16(a, 0x2e) = ah < bh;
            FUN_8004258c(a, 1);
        }
        break;
    case 2:
        if (b[0] != 3) {
            int ah, bh;
            ah = S16(PTR(a, 0x40), 2);
            bh = S16(PTR(b, 0x40), 2);
            b[0] = 3;
            b[4] = 2;
            b[5] = 0;
            b[6] = 0;
            b[0x69] = 0;
            U16(b, 0x7a) = bh < ah;
        }
        break;
    }
}
