// FUNC 800f0f20 236 D0SPSUB
// MATCHING 800f0f20 236
typedef struct { char p[0x10]; short w10; } Menu;
typedef struct { int a, b, c; } Tri;
extern unsigned short GAME, GAME_SELECTED_SECTION;
extern int MAP_AREA14_ROOM_NAMES[];
extern int *MAP_ROOM_NAMES[];
extern Tri *D_80108F94;
extern char SUBSCREEN_FONT[];
extern int SUBSCREEN_MAP_LINK;
typedef struct { short x, y, w; unsigned short attr; int img, pC, str; void *p14; } Text;
extern Text SUBSCREEN_DRAW_PARAMS;
extern void subScreenDrawString(int a, int b);

void subScreenMapDrawRoomName(Menu *m, int i, int j)
{
    SUBSCREEN_DRAW_PARAMS.x = 0x10;
    SUBSCREEN_DRAW_PARAMS.y = 0xE8;
    if (GAME == 0xE) SUBSCREEN_DRAW_PARAMS.str = MAP_AREA14_ROOM_NAMES[GAME_SELECTED_SECTION];
    else SUBSCREEN_DRAW_PARAMS.str = MAP_ROOM_NAMES[i][j];
    if (SUBSCREEN_MAP_LINK == 0xC) { register Tri *t asm("v1") = D_80108F94; SUBSCREEN_DRAW_PARAMS.str = t->c; }
    SUBSCREEN_DRAW_PARAMS.w = m->w10 + 1;
    SUBSCREEN_DRAW_PARAMS.attr = 0x7B92;
    SUBSCREEN_DRAW_PARAMS.p14 = SUBSCREEN_FONT;
    subScreenDrawString(5, 0);
}
