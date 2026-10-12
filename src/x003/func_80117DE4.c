// FUNC 80117de4 1184 X003
// MATCHING 80117de4 1184
/* Loops written as macros on ONE function-scope `short i` shared by all four loops: the shared loop variable is
   what keeps i in a1 (it conflicts with k in a0 in cases 2/3) and makes cse extend i rather than a before the loop.
   Covers csv pieces 80117F98/80118040/801180AC. */
typedef struct { short x, y, w, h; } RECT;
typedef struct {
    unsigned char p0[3];
    unsigned char count;
    int items[0x58];
    unsigned char b164;
} L;
extern unsigned short D_8009C962;
extern int D_800A4574;
extern short D_1F800176;
extern unsigned char D_8009D2C3;
extern int getScrollOffsetX(void);
extern void FUN_8005f488(RECT *, int, int);
extern void func_80118284(L *);

#define FILL() \
    for (i = a; i <= b; i++) { \
        o->items[o->count] = (int)tbl + list[i]; \
        o->count++; \
    }

#define ADDRANGE() \
    for (i = 0; i < 11; i++) { \
        k = base + i; \
        if (k >= n) break; \
        if (k >= 0) { \
            o->items[i] = (int)tbl + list[k]; \
            o->count++; \
        } \
    }

void func_80117DE4(L *o, unsigned char *tbl)
{
    unsigned char n = tbl[0];
    int *list = (int *)(tbl + 4);
    short a;
    short b;
    short i, base;
    int k;
    RECT r;

    switch (D_8009C962) {
    case 0:
    case 4:
        if (D_800A4574 == 0) {
            a = (D_1F800176 + 0x140) / 80;
            b = a + 8;
        } else {
            { int t = getScrollOffsetX() + 0x140; a = (D_1F800176 + t) / 80; }
            b = a + 9;
        }
        if (a < 0) a = 0;
        if (b >= n) b = n - 1;
        FILL();
        break;
    case 1:
    case 5:
        if (D_800A4574 == 0) {
            a = (D_1F800176 - 0xa28) / 80;
            b = a + 8;
        } else {
            { int t = getScrollOffsetX() - 0xa28; a = (D_1F800176 + t) / 80; }
            b = a + 9;
        }
        if (D_1F800176 >= 0x12c1) a--;
        if (a < 0) a = 0;
        if (b >= n) b = n - 1;
        FILL();
        break;
    case 2:
        base = (D_1F800176 + getScrollOffsetX()) / 80;
        ADDRANGE();
        func_80118284(o);
        break;
    case 3:
        if ((D_8009D2C3 & 2) && o->b164 == 0) {
            r.x = 0xa0;
            r.y = 0x1e0;
            r.w = 0x10;
            r.h = 0x20;
            FUN_8005f488(&r, 0x90, 0x1e0);
            r.x = 0x280;
            r.y = 0;
            r.w = 0x40;
            r.h = 0xe0;
            FUN_8005f488(&r, 0x80, 0x100);
            o->b164++;
        }
        base = (D_1F800176 + getScrollOffsetX()) / 80;
        ADDRANGE();
        break;
    }
}
