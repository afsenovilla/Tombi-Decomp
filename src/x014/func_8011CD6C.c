// FUNC 8011cd6c 248 X014
// MATCHING 8011cd6c 248
#define U8(p, o) (*(unsigned char *)((p) + (o)))
#define U16(p, o) (*(unsigned short *)((p) + (o)))
extern short boxesOverlap(unsigned char *, unsigned char *);
extern void decreaseObjectTimer(unsigned char *, int);
extern void FUN_800eea7c(unsigned char *, int, int);
extern unsigned char D_1F8001A4;
extern short D_1F80019E;

void func_8011CD6C(unsigned char *a, unsigned char *b)
{
    if (D_1F8001A4 != 0) return;
    if (U8(a, 0xac) == 2) return;
    if (a[0] & 2) return;
    if (boxesOverlap(a, b) == 0) return;
    if (U8(a, 0xac) >= 2) {
        a[0] = 2;
        U16(a, 0x2e) = 1;
        a[4] = 2;
        a[5] = 0;
        a[6] = 0;
        decreaseObjectTimer(a, 1);
    } else {
        U8(a, 0xac) = 0;
        b[0] = 2;
        b[0x6a] = 1;
        FUN_800eea7c(a, 0x10, 0);
        a[0] = 6;
        a[4] = 5;
        a[5] = 0x65;
        a[6] = 0;
    }
    D_1F80019E = 0;
}
