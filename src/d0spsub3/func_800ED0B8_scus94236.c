// VERSION SCUS_94236 dspsub3 800ED0B8 540
#if defined(VERSION_SCUS_94236)
// USA version of PAL d0spsub3 800edf08 (subScreenDrawItemPage.c)
typedef struct {
    unsigned char state, b1, b2, b3;
    char p4[0xc];
    short w10;
    char p12[2];
    short page;
    char p16[0x42];
    void **icons;
} Menu;
typedef struct { short x, y, dw, idx; } Slot;
extern Menu MENU_STATE;
extern Slot SUBSCREEN_PAGE_ARROW_SLOTS[], D_800FFAB4[];
extern void *D_800F45A4[];
extern unsigned short D_8009C60C;
extern short SUBSCREEN_POPUP_CURSOR;
extern short SUBSCREEN_DRAW_PARAMS[];
extern short SUBSCREEN_DRAW_Y[];
extern short SUBSCREEN_DRAW_TPAGE[];
extern short SUBSCREEN_DRAW_CLUT;
extern void * SUBSCREEN_DRAW_FRAME;
extern void subScreenDrawTabs(Menu *m);
extern void subScreenDrawPopupMenu(Menu *m);
extern void subScreenDrawItemGrid(Menu *m);
extern void subScreenDrawItemNames(Menu *m);
extern void subScreenDrawPageNumber(Menu *m, int n);
extern void subScreenDrawSprite(int n);

void subScreenDrawItemPage(void)
{
    Menu *m = &MENU_STATE;
    int pages;
    Slot *s;
    short end;
    subScreenDrawTabs(m);
    if (SUBSCREEN_POPUP_CURSOR >= 0) subScreenDrawPopupMenu(m);
    pages = (D_8009C60C + 7) / 8;
    subScreenDrawItemGrid(m);
    subScreenDrawItemNames(m);
    if (pages >= 2) {
        subScreenDrawPageNumber(m, pages);
        s = SUBSCREEN_PAGE_ARROW_SLOTS;
        while (s->x != -1) {
            if ((s->idx == 0x16 && m->page == 0) || (s->idx == 0x17 && m->page >= pages - 1)) {
                s++;
            } else {
                SUBSCREEN_DRAW_PARAMS[0] = s->x;
                SUBSCREEN_DRAW_Y[0] = s->y;
                SUBSCREEN_DRAW_CLUT = 0x7C15;
                SUBSCREEN_DRAW_TPAGE[0] = m->w10 + s->dw;
                SUBSCREEN_DRAW_FRAME = m->icons[s->idx];
                subScreenDrawSprite(1);
                s++;
            }
        }
    }
    end = -1;
    SUBSCREEN_DRAW_CLUT = 0x7812;
    for (s = D_800FFAB4; s->x != end; s++) {
        SUBSCREEN_DRAW_PARAMS[0] = s->x;
        SUBSCREEN_DRAW_Y[0] = s->y;
        SUBSCREEN_DRAW_TPAGE[0] = m->w10 + s->dw;
        SUBSCREEN_DRAW_FRAME = D_800F45A4[s->idx];
        subScreenDrawSprite(3);
    }
}
#endif
