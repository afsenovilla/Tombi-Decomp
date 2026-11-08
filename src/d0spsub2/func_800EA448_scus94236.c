// VERSION SCUS_94236 dspsub2 800EA448 220
#if defined(VERSION_SCUS_94236)
// USA version of PAL d0spsub2 800ea948 (memCardDrawSlotSelect.c)
typedef struct L { short x0, x1, x2, x3; short a, b, c, d; short m; } L;
extern char MEMCARD_SLOT_PROMPT_STRING[], MEMCARD_SLOT1_LABEL_STRING[], MEMCARD_SLOT2_LABEL_STRING[];
extern short MEMCARD_DRAW_TPAGE;
extern void memCardDrawParts(char *s, int x, int y);
extern void memCardDrawCursorTile(short x, short y);
extern void AddDrawMode(int a, int b);
extern void drawBalloonFrame(L *, int, int);

void memCardDrawSlotSelect(int yes, int no, int sel)
{
    L l;
    if (sel != 0) {
        if (yes) memCardDrawParts(MEMCARD_SLOT1_LABEL_STRING, 0x68, 0x78);
        if (no) memCardDrawParts(MEMCARD_SLOT2_LABEL_STRING, 0xA8, 0x78);
        if (sel == 1) memCardDrawCursorTile(0x68, 0x70);
        else if (sel == 2) memCardDrawCursorTile(0xA8, 0x70);
        MEMCARD_DRAW_TPAGE = 1;
        AddDrawMode(1, 1);
        l.m = -1;
        l.a = 0x60;
        l.b = 0x68;
        l.c = 0x80;
        l.d = 0x20;
        drawBalloonFrame(&l, 0, 0x14);
    }
}
#endif
