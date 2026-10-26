// FUNC 800ea5e8 780 D0SPSUB
// MATCHING 800ea5e8 780
// Draws two 3-digit counters of the event list: script flags == 0xFF (x 8) and != 0 (x 0x28), index 0x12 excluded,
// with the icon p58[0x18] between them.
// Idiom: every per-half temporary (h, t, b, y, q, k) lives in its own block so each is a block-local pseudo
// (local-alloc, one death); `b` narrower than int; x as the giv `8 + k * 10` (its init lands after the hoisted
// invariants) and the y constant as a block local (lowest global priority -> s6, first in the preheader).
typedef struct { char p[0x10]; short w10; char p12[0x46]; int *p58; int *p5c; } Menu;
extern short D_8011113C, D_80111140;
extern short D_8011113E[];
extern int D_80111144;
extern short D_8011113C_a[], D_80111140_a[];
extern unsigned char SCRIPT_FLAGS[];
extern void subScreenDrawSprite(int n);

void func_800EA5E8(Menu *m)
{
    unsigned char d[3];
    int i, n;
    n = 0;
    for (i = 0; i < 0x100; i++) {
        if (i != 0x12 && SCRIPT_FLAGS[i] == 0xFF) n++;
    }
    {
        int h, t; unsigned char b;
        h = n / 100;
        t = n / 10;
        b = t - h * 10;
        d[2] = n - t * 10;
        h %= 10;
        d[1] = b;
        d[0] = h;
    }
    {
        unsigned char *q; int k; short y = 0xC8;
        for (q = d, k = 0; k < 3; k++) {
            D_8011113C = 8 + k * 10;
            D_8011113E[0] = y;
            D_80111140 = m->w10;
            D_8011113E[2] = 0x7C15;
            D_80111144 = m->p5c[*q++];
            subScreenDrawSprite(1);
        }
    }
    D_8011113C_a[0] = 0x20;
    D_8011113E[0] = 0xC8;
    D_80111140_a[0] = m->w10;
    D_8011113E[2] = 0x7C15;
    D_80111144 = m->p58[0x18];
    subScreenDrawSprite(1);
    n = 0;
    for (i = 0; i < 0x100; i++) {
        if (i != 0x12 && SCRIPT_FLAGS[i] != 0) n++;
    }
    {
        int h, t; unsigned char b;
        h = n / 100;
        t = n / 10;
        b = t - h * 10;
        d[2] = n - t * 10;
        h %= 10;
        d[1] = b;
        d[0] = h;
    }
    {
        unsigned char *q; int k; short y = 0xC8;
        for (q = d, k = 0; k < 3; k++) {
            D_8011113C = 0x28 + k * 10;
            D_8011113E[0] = y;
            D_80111140 = m->w10;
            D_8011113E[2] = 0x7C15;
            D_80111144 = m->p5c[*q++];
            subScreenDrawSprite(1);
        }
    }
}
