// FUNC 800e8a94 256 O0PTSUB0
// MATCHING 800e8a94 256
typedef struct { short x, y, w, h; } RECT;
extern unsigned char TITLE_LOGO_BOUNCE_PHASE;
extern unsigned short TITLE_LOGO_BOUNCE_PHASE_w; /* the same counter, read as a halfword */
extern short TITLE_LOGO_BOUNCE_AMPLITUDE;
extern unsigned short TITLE_LOGO_BOUNCE_OFFSET;
extern char TITLE_LOGO_SPRITE_RECS[], TITLE_LOGO_STATIC_SPRITE_RECS[];
extern short MulNegSin(int a, int b);
extern void addTilePrim(RECT *r, int a, int b, int c);
extern void titleDrawSpriteRecs(void *list, int x, short y, int dfe);

void titleLogoDraw(int *paused)
{
    RECT r;
    short v;
    if (*paused == 0) {
        v = MulNegSin(TITLE_LOGO_BOUNCE_PHASE, TITLE_LOGO_BOUNCE_AMPLITUDE) >> 4;
        TITLE_LOGO_BOUNCE_OFFSET = v;
        if (v < 0) TITLE_LOGO_BOUNCE_OFFSET = -v;
        TITLE_LOGO_BOUNCE_PHASE_w += 2;
        if (TITLE_LOGO_BOUNCE_AMPLITUDE > 0) TITLE_LOGO_BOUNCE_AMPLITUDE -= 0x20;
    }
    r.w = 0x140;
    r.x = 0;
    r.y = 0;
    r.h = 8;
    addTilePrim(&r, 0, 0, 1);
    titleDrawSpriteRecs(TITLE_LOGO_SPRITE_RECS, 0, -TITLE_LOGO_BOUNCE_OFFSET, 0);
    titleDrawSpriteRecs(TITLE_LOGO_STATIC_SPRITE_RECS, 0, 0, 0);
}
