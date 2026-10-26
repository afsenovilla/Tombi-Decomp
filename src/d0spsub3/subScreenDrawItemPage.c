// FUNC 800edf08 548 D0SPSUB3
// MATCHING 800edf08 548
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
extern Slot D_8010075C[], D_801007BC[];
extern void *D_800F5400[];
extern unsigned short D_8009D2A4;
extern short D_80100918;
extern short D_80100B70[];
extern short D_80100B72[];
extern short D_80100B74[];
extern short D_80100B76;
extern void * D_80100B78;
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
    if (D_80100918 >= 0) subScreenDrawPopupMenu(m);
    pages = (D_8009D2A4 + 9) / 10;
    subScreenDrawItemGrid(m);
    subScreenDrawItemNames(m);
    if (pages >= 2) {
        subScreenDrawPageNumber(m, pages);
        s = D_8010075C;
        while (s->x != -1) {
            if ((s->idx == 0x16 && m->page == 0) || (s->idx == 0x17 && m->page >= pages - 1)) {
                s++;
            } else {
                D_80100B70[0] = s->x;
                D_80100B72[0] = s->y;
                D_80100B76 = 0x7C15;
                D_80100B74[0] = m->w10 + s->dw;
                D_80100B78 = m->icons[s->idx];
                subScreenDrawSprite(1);
                s++;
            }
        }
    }
    end = -1;
    D_80100B76 = 0x7812;
    for (s = D_801007BC; s->x != end; s++) {
        D_80100B70[0] = s->x;
        D_80100B72[0] = s->y;
        D_80100B74[0] = m->w10 + s->dw;
        D_80100B78 = D_800F5400[s->idx];
        subScreenDrawSprite(3);
    }
}
