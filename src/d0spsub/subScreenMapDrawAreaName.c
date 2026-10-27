// FUNC 800f0e0c 276 D0SPSUB
// MATCHING 800f0e0c 276
typedef struct { char p[0x10]; short w10; } Menu;
typedef struct { short x, y, w; unsigned short attr; int img, pC, str; void *p14; } Text;
extern unsigned short GAME, GAME_SELECTED_SECTION;
extern int MAP_AREA14_AREA_NAMES[], MAP_AREA_NAMES[];
extern int D_80108BCC, D_80108BE0;
extern char SUBSCREEN_FONT[];
extern int SUBSCREEN_MAP_LINK;
extern Text SUBSCREEN_DRAW_PARAMS;
extern void subScreenDrawString(int a, int b);

void subScreenMapDrawAreaName(Menu *m, int i)
{
    SUBSCREEN_DRAW_PARAMS.x = 0x10;
    SUBSCREEN_DRAW_PARAMS.y = 0xD8;
    if (GAME == 0xE) SUBSCREEN_DRAW_PARAMS.str = MAP_AREA14_AREA_NAMES[GAME_SELECTED_SECTION + GAME];
    else SUBSCREEN_DRAW_PARAMS.str = MAP_AREA_NAMES[i];
    switch (SUBSCREEN_MAP_LINK) {
    case 0x26:
    case 0x27:
    case 0x2E:
        SUBSCREEN_DRAW_PARAMS.str = D_80108BCC;
        break;
    case 0xC:
        SUBSCREEN_DRAW_PARAMS.str = D_80108BE0;
        break;
    }
    SUBSCREEN_DRAW_PARAMS.w = m->w10 + 1;
    SUBSCREEN_DRAW_PARAMS.attr = 0x7B92;
    SUBSCREEN_DRAW_PARAMS.p14 = SUBSCREEN_FONT;
    subScreenDrawString(5, 0);
}
