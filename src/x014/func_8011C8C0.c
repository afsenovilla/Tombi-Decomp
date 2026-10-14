// FUNC 8011c8c0 220 X014
// MATCHING 8011c8c0 220
#include "spad.h"
#define U8(p, o) (*(unsigned char *)((p) + (o)))
#define U16(p, o) (*(unsigned short *)((p) + (o)))
#define S16(p, o) (*(short *)((p) + (o)))
#define PTR(p, o) (*(unsigned char **)((p) + (o)))
extern short boxesOverlap(unsigned char *, unsigned char *);
extern void decreaseObjectTimer(unsigned char *, int);

void func_8011C8C0(unsigned char *a, unsigned char *b)
{
    if (U8(a, 0xac) == 2) return;
    if (a[0] & 2) return;
    if (boxesOverlap(a, b) == 0) return;
    if (SPAD.b1a4 != 0) return;
    if (b[3] == 1) {
        b[0] = 2;
        b[4] = 2;
        b[5] = 0;
        b[6] = 0;
    } else {
        int ah, bh;
        a[0] = 2;
        bh = S16(PTR(b, 0x40), 2);
        ah = S16(PTR(a, 0x40), 2);
        a[4] = 2;
        a[5] = 0;
        a[6] = 0;
        U16(a, 0x2e) = ah < bh;
        decreaseObjectTimer(a, 1);
    }
    SPAD.s19e = 0;
}
