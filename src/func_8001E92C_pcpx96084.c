// VERSION PCPX_96084 tomba 8001E92C 308
// FLAGS -O2 -G0 -fno-strength-reduce
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Text window: sets the CLUT (row +0x22 + cy, x +0x20) of a w x h block of glyph primitives starting at cell
// (x, y); the primitives are 36 bytes (modes 0 and 2) or 44 bytes (mode 1), +0x2E cells per row.
typedef struct { short x0, x1; char p[32]; } P36;
typedef struct { short x0, x1; char p[40]; } P44;
typedef struct {
    unsigned short mode; short pad2; void *prims; char p8[0x20 - 8]; unsigned short x20, y22; char p24[0x2e - 0x24]; unsigned short cols;
} TB;

void func_8001E92C_pcpx96084(TB *t, int x, int y, int w, int h, int cy)
{
    int cols = t->cols;
    unsigned short clut = ((t->y22 + cy) << 6) | ((t->x20 >> 4) & 0x3f);
    register int i asm("$8");
    switch (t->mode) {
    case 0:
        { int m = y * cols; m += x; x = m; }
        do {
            P36 *p;
            i = w;
            p = (P36 *)t->prims + x;
            do {
                p->x1 = clut;
                p++;
            } while (--i);
            x += cols;
        } while (--h);
        break;
    case 1:
        { int m = y * cols; m += x; x = m; }
        do {
            P44 *p;
            i = w;
            p = (P44 *)((P36 *)t->prims + x);
            do {
                p->x1 = clut;
                p++;
            } while (--i);
            x += cols;
        } while (--h);
        break;
    case 2:
        { int m = y * cols; m += x; x = m; }
        do {
            P36 *p;
            i = w;
            p = (P36 *)t->prims + x;
            do {
                p->x1 = clut;
                p++;
            } while (--i);
            x += cols;
        } while (--h);
        break;
    }
}

#endif
