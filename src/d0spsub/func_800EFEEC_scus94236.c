// VERSION SCUS_94236 dspsub 800EFEEC 268
#if defined(VERSION_SCUS_94236)
// USA version of PAL d0spsub 800f0e0c (subScreenMapDrawAreaName.c)
typedef struct { char p[0x10]; short w10; } Menu;
typedef struct { short x, y, w; unsigned short attr; int img, pC, str; void *p14; } Text;
extern unsigned short GAME, GAME_SELECTED_SECTION;
extern int MAP_AREA14_AREA_NAMES[], MAP_AREA_NAMES[];
extern int D_80107D8C, D_80107DA0;
extern char SUBSCREEN_FONT[];
extern int SUBSCREEN_MAP_LINK;
extern Text SUBSCREEN_DRAW_PARAMS;
extern void subScreenDrawString(int a, int b);

void subScreenMapDrawAreaName(Menu *m, int i)
{
    SUBSCREEN_DRAW_PARAMS.x = 0x10;
    SUBSCREEN_DRAW_PARAMS.y = 0xB8;
    if (GAME == 0xE) SUBSCREEN_DRAW_PARAMS.str = MAP_AREA14_AREA_NAMES[GAME_SELECTED_SECTION];
    else SUBSCREEN_DRAW_PARAMS.str = MAP_AREA_NAMES[i];
    switch (SUBSCREEN_MAP_LINK) {
    case 0x26:
    case 0x27:
    case 0x2E:
        SUBSCREEN_DRAW_PARAMS.str = D_80107D8C;
        break;
    case 0xC:
        SUBSCREEN_DRAW_PARAMS.str = D_80107DA0;
        break;
    }
    SUBSCREEN_DRAW_PARAMS.w = m->w10;
    SUBSCREEN_DRAW_PARAMS.attr = 0x7B92;
    SUBSCREEN_DRAW_PARAMS.p14 = SUBSCREEN_FONT;
    subScreenDrawString(5, 0);
}
#endif
