// FUNC 8011efa4 400 X004
// MATCHING 8011efa4 400
#define U8(p, o) (*(unsigned char *)((p) + (o)))
#define U16(p, o) (*(unsigned short *)((p) + (o)))
#define S16(p, o) (*(short *)((p) + (o)))
#define PTR(p, o) (*(unsigned char **)((p) + (o)))
extern short FUN_800435e0(unsigned char *, unsigned char *);
extern void FUN_8004258c(unsigned char *, int);
extern unsigned char DAT_1f8001a4;

void FUN_8011efa4(unsigned char *a, unsigned char *b)
{
    short r = FUN_800435e0(a, b);
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
            break;
        }
    case 0:
    case 3:
        if ((b[0] & 2) == 0) {
            b[0x69] = 8;
            if (DAT_1f8001a4 == 0 && (a[0] & 2) == 0) {
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
