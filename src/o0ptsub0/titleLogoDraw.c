// FUNC 800e8a94 256 O0PTSUB0
// MATCHING 800e8a94 256
typedef struct { short x, y, w, h; } RECT;
extern unsigned char D_800EB47C;
extern unsigned short D_800EB47C_w; /* the same counter, read as a halfword */
extern short D_800EB478;
extern unsigned short D_800EB470;
extern char D_800EB17C[], D_800EB1AC[];
extern short MulNegSin(int a, int b);
extern void addTilePrim(RECT *r, int a, int b, int c);
extern void titleDrawSpriteRecs(void *list, int x, short y, int dfe);

void titleLogoDraw(int *paused)
{
    RECT r;
    short v;
    if (*paused == 0) {
        v = MulNegSin(D_800EB47C, D_800EB478) >> 4;
        D_800EB470 = v;
        if (v < 0) D_800EB470 = -v;
        D_800EB47C_w += 2;
        if (D_800EB478 > 0) D_800EB478 -= 0x20;
    }
    r.w = 0x140;
    r.x = 0;
    r.y = 0;
    r.h = 8;
    addTilePrim(&r, 0, 0, 1);
    titleDrawSpriteRecs(D_800EB17C, 0, -D_800EB470, 0);
    titleDrawSpriteRecs(D_800EB1AC, 0, 0, 0);
}
