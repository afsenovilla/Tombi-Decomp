// FUNC 800ea540 780 D0SPSUB3
// MATCHING 800ea540 780
// Draws two 3-digit counters of the event list: script flags == 0xFF (x 8) and != 0 (x 0x28), index 0x12 excluded,
// with the icon p58[0x18] between them.
// Idiom: every per-half temporary (h, t, b, y, q, k) lives in its own block so each is a block-local pseudo
// (local-alloc, one death); `b` narrower than int; x as the giv `8 + k * 10` (its init lands after the hoisted
// invariants) and the y constant as a block local (lowest global priority -> s6, first in the preheader).
typedef struct { char p[0x10]; short w10; char p12[0x46]; int *p58; int *p5c; } Menu;
extern short SUBSCREEN_DRAW_PARAMS, SUBSCREEN_DRAW_TPAGE;
extern short SUBSCREEN_DRAW_Y[];
extern int SUBSCREEN_DRAW_FRAME;
extern short SUBSCREEN_DRAW_PARAMS_a[], SUBSCREEN_DRAW_TPAGE_a[];
extern unsigned char SCRIPT_FLAGS[];
extern void subScreenDrawSprite(int n);

void func_800EA540(Menu *m)
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
            SUBSCREEN_DRAW_PARAMS = 8 + k * 10;
            SUBSCREEN_DRAW_Y[0] = y;
            SUBSCREEN_DRAW_TPAGE = m->w10;
            SUBSCREEN_DRAW_Y[2] = 0x7C15;
            SUBSCREEN_DRAW_FRAME = m->p5c[*q++];
            subScreenDrawSprite(1);
        }
    }
    SUBSCREEN_DRAW_PARAMS_a[0] = 0x20;
    SUBSCREEN_DRAW_Y[0] = 0xC8;
    SUBSCREEN_DRAW_TPAGE_a[0] = m->w10;
    SUBSCREEN_DRAW_Y[2] = 0x7C15;
    SUBSCREEN_DRAW_FRAME = m->p58[0x18];
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
            SUBSCREEN_DRAW_PARAMS = 0x28 + k * 10;
            SUBSCREEN_DRAW_Y[0] = y;
            SUBSCREEN_DRAW_TPAGE = m->w10;
            SUBSCREEN_DRAW_Y[2] = 0x7C15;
            SUBSCREEN_DRAW_FRAME = m->p5c[*q++];
            subScreenDrawSprite(1);
        }
    }
}
