// FUNC 800ec504 160 D0SPSUB2
// MATCHING 800ec504 160
extern short MEMCARD_DRAW_PARAMS, MEMCARD_DRAW_Y, MEMCARD_DRAW_TPAGE, MEMCARD_DRAW_CLUT;
extern int MEMCARD_DRAW_STR;
extern void *MEMCARD_DRAW_FONT;
extern char MEMCARD_FONT[];
extern int *MEMCARD_LOCATION_STRINGS[];
extern unsigned short GetClut(int x, int y);
extern void memCardDrawString(int a, int b);

void memCardDrawFileLocation(int tbl, int idx, int x, int y)
{
    MEMCARD_DRAW_PARAMS = x;
    MEMCARD_DRAW_Y = y;
    MEMCARD_DRAW_TPAGE = 4;
    MEMCARD_DRAW_CLUT = GetClut(0x150, 0x1F0);
    MEMCARD_DRAW_FONT = MEMCARD_FONT;
    MEMCARD_DRAW_STR = MEMCARD_LOCATION_STRINGS[tbl][idx];
    memCardDrawString(5, 0);
}
