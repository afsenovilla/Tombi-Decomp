// VERSION SCUS_94236 dspsub3 800E8D68 500
#if defined(VERSION_SCUS_94236)
// USA version of PAL d0spsub 800e9ab0 (subScreenDrawStatusNotes.c)
typedef struct { char p[0x10]; short w10; } Menu;
typedef struct { short x, y, w; unsigned short attr; int img, pC, str; void *p14; } Text;
extern int GAME_AREA00_EVENT_CONTROL;
extern unsigned char D_8009C114, D_8009C119, D_8009C1A5;
extern char SUBSCREEN_FONT[];
extern int D_800FF000[];
extern int D_800FF004, D_800FF018, D_800FF01C;
extern Text SUBSCREEN_DRAW_PARAMS;
extern void subScreenDrawString(int a, int b);

void subScreenDrawStatusNotes(Menu *m)
{
    int n;
    if (GAME_AREA00_EVENT_CONTROL & 0x40) {
        SUBSCREEN_DRAW_PARAMS.x = 0x60;
        SUBSCREEN_DRAW_PARAMS.y = 0x88;
        SUBSCREEN_DRAW_PARAMS.w = m->w10;
        SUBSCREEN_DRAW_PARAMS.attr = 0x7C15;
        SUBSCREEN_DRAW_PARAMS.p14 = SUBSCREEN_FONT;
        SUBSCREEN_DRAW_PARAMS.str = D_800FF004;
        subScreenDrawString(5, 1);
    }
    n = D_8009C119;
    if (n >= 2) {
        SUBSCREEN_DRAW_PARAMS.x = 0x60;
        SUBSCREEN_DRAW_PARAMS.y = 0x98;
        SUBSCREEN_DRAW_PARAMS.w = m->w10;
        SUBSCREEN_DRAW_PARAMS.attr = 0x7C15;
        if (n == 0xFF) n = 5;
        SUBSCREEN_DRAW_PARAMS.str = D_800FF000[n];
        SUBSCREEN_DRAW_PARAMS.p14 = SUBSCREEN_FONT;
        subScreenDrawString(5, 1);
    }
    if (D_8009C114 == 0xFF) {
        SUBSCREEN_DRAW_PARAMS.x = 0xF0;
        SUBSCREEN_DRAW_PARAMS.y = 0x88;
        SUBSCREEN_DRAW_PARAMS.w = m->w10;
        SUBSCREEN_DRAW_PARAMS.attr = 0x7C15;
        SUBSCREEN_DRAW_PARAMS.p14 = SUBSCREEN_FONT;
        SUBSCREEN_DRAW_PARAMS.str = D_800FF018;
        subScreenDrawString(5, 1);
    }
    if (D_8009C1A5 == 0xFF) {
        SUBSCREEN_DRAW_PARAMS.x = 0xF0;
        SUBSCREEN_DRAW_PARAMS.y = 0x98;
        SUBSCREEN_DRAW_PARAMS.w = m->w10;
        SUBSCREEN_DRAW_PARAMS.attr = 0x7C15;
        SUBSCREEN_DRAW_PARAMS.p14 = SUBSCREEN_FONT;
        SUBSCREEN_DRAW_PARAMS.str = D_800FF01C;
        subScreenDrawString(5, 1);
    }
}
#endif
