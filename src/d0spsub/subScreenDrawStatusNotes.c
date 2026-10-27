// FUNC 800e9ab0 516 D0SPSUB
// MATCHING 800e9ab0 516
typedef struct { char p[0x10]; short w10; } Menu;
typedef struct { short x, y, w; unsigned short attr; int img, pC, str; void *p14; } Text;
extern int GAME_AREA00_EVENT_CONTROL;
extern unsigned char D_8009CDAC, D_8009CDB1, D_8009CE3D;
extern char SUBSCREEN_FONT[];
extern int D_801091A4[];
extern int D_801091A8, D_801091BC, D_801091C0;
extern Text SUBSCREEN_DRAW_PARAMS;
extern void subScreenDrawString(int a, int b);

void subScreenDrawStatusNotes(Menu *m)
{
    int n;
    if (GAME_AREA00_EVENT_CONTROL & 0x40) {
        SUBSCREEN_DRAW_PARAMS.x = 0x60;
        SUBSCREEN_DRAW_PARAMS.y = 0x98;
        SUBSCREEN_DRAW_PARAMS.w = m->w10 + 1;
        SUBSCREEN_DRAW_PARAMS.attr = 0x7C15;
        SUBSCREEN_DRAW_PARAMS.p14 = SUBSCREEN_FONT;
        SUBSCREEN_DRAW_PARAMS.str = D_801091A8;
        subScreenDrawString(5, 1);
    }
    n = D_8009CDB1;
    if (n >= 2) {
        SUBSCREEN_DRAW_PARAMS.x = 0x60;
        SUBSCREEN_DRAW_PARAMS.y = 0xA8;
        SUBSCREEN_DRAW_PARAMS.w = m->w10 + 1;
        SUBSCREEN_DRAW_PARAMS.attr = 0x7C15;
        if (n == 0xFF) n = 5;
        SUBSCREEN_DRAW_PARAMS.str = D_801091A4[n];
        SUBSCREEN_DRAW_PARAMS.p14 = SUBSCREEN_FONT;
        subScreenDrawString(5, 1);
    }
    if (D_8009CDAC == 0xFF) {
        SUBSCREEN_DRAW_PARAMS.x = 0xF0;
        SUBSCREEN_DRAW_PARAMS.y = 0x98;
        SUBSCREEN_DRAW_PARAMS.w = m->w10 + 1;
        SUBSCREEN_DRAW_PARAMS.attr = 0x7C15;
        SUBSCREEN_DRAW_PARAMS.p14 = SUBSCREEN_FONT;
        SUBSCREEN_DRAW_PARAMS.str = D_801091BC;
        subScreenDrawString(5, 1);
    }
    if (D_8009CE3D == 0xFF) {
        SUBSCREEN_DRAW_PARAMS.x = 0xF0;
        SUBSCREEN_DRAW_PARAMS.y = 0xA8;
        SUBSCREEN_DRAW_PARAMS.w = m->w10 + 1;
        SUBSCREEN_DRAW_PARAMS.attr = 0x7C15;
        SUBSCREEN_DRAW_PARAMS.p14 = SUBSCREEN_FONT;
        SUBSCREEN_DRAW_PARAMS.str = D_801091C0;
        subScreenDrawString(5, 1);
    }
}
