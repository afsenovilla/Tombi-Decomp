// VERSION SLPM_80165 slpm80165 80018254 140
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only: clears a text grid of 0x24-byte sprite cells (code 0, the grid's CLUT word) and homes its cursor.
typedef struct {
    char p0[4];
    char *cells;
    char p8[0x20 - 8];
    unsigned short clutX;
    unsigned short clutY;
    short curX;
    short curY;
    short clutRow;
    char p2a[0x2e - 0x2a];
    unsigned short w;
    unsigned short h;
} Grid;

void func_80018254(Grid *g)
{
    char *c = g->cells;
    int x, y;
    int clut;
    char pad[0];
    g->curY = 0;
    g->curX = 0;
    clut = ((g->clutY + g->clutRow) << 6) | ((g->clutX >> 4) & 0x3f);
    for (y = 0; y < g->h; y++) {
        for (x = 0; x < g->w; x++) {
            *(unsigned short *)c = 0;
            *(unsigned short *)(c + 2) = clut;
            c += 0x24;
        }
    }
}
#endif
