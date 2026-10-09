// FUNC 80117de4 1184 X003
/* score 28: only a register swap left in the two fill() loops (cases 0/1): game keeps i in a1 and the
   loop bound b in a0, we get i in a0 and b in a1. Tried: param/local int/short combos, loop forms
   (for/while/do), clamps inside/outside the inline, swapped params, reusing a for b, register asm.
   o23: game asm reads as fill(short a, int b): i is an unextended copy of a (move a1,a0 before the b clamp), b' an
   extended copy (move a0,v1); that signature gives the loop exactly but swaps a/b at the caller (56, 44 with ternary b clamp).
   NB: this function is missing from notes/functions_x003.csv (covers pieces 80117F98/80118040/801180AC). */
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

static __inline__ void fill(L *o, unsigned char *tbl, int *list, int a, short b)
{
    short i;

    for (i = a; i <= b; i++) {
        o->items[o->count] = (int)tbl + list[i];
        o->count++;
    }
}

static __inline__ void addRange(L *o, unsigned char *tbl, int *list, short base, unsigned char n)
{
    short i;
    int k;

    for (i = 0; i < 11; i++) {
        k = base + i;
        if (k >= n) break;
        if (k >= 0) {
            o->items[i] = (int)tbl + list[k];
            o->count++;
        }
    }
}

void func_80117DE4(L *o, unsigned char *tbl)
{
    unsigned char n = tbl[0];
    int *list = (int *)(tbl + 4);
    short a;
    short b;
    short i, base;
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
        fill(o, tbl, list, a, b);
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
        fill(o, tbl, list, a, b);
        break;
    case 2:
        addRange(o, tbl, list, (D_1F800176 + getScrollOffsetX()) / 80, n);
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
        addRange(o, tbl, list, (D_1F800176 + getScrollOffsetX()) / 80, n);
        break;
    }
}
