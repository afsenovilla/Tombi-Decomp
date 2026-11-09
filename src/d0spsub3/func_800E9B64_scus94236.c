// VERSION SCUS_94236 dspsub3 800E9B64 924
#if defined(VERSION_SCUS_94236)
// USA version of PAL d0spsub 800ea8f4 (subScreenDrawItemGrid.c)
typedef struct { char p[0x10]; short w10; char p12[2]; short page; short cursor; char p18[6]; short sel; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w, attr; int img; } Text;
extern Slot SUBSCREEN_ITEM_SLOTS[];
extern Text SUBSCREEN_DRAW_PARAMS;
extern int D_80012370;
extern unsigned short D_8009C60C;
extern unsigned char SUBSCREEN_ITEM_LIST[];
extern unsigned char D_8009C25F, D_8009C360, D_8009C260, D_8009BCF8, D_8009C36E, D_8009C363;
extern unsigned char D_8009C3F8[]; /* GAME+0x730 */
#define G30 (D_8009C3F8 - 0x700) /* GAME+0x30 (same base register in the game) */
extern void subScreenDrawItemIcon(int n, int sel, int flag, int nocheck);

void subScreenDrawItemGrid(Menu *m)
{
    Slot *s;
    for (s = SUBSCREEN_ITEM_SLOTS; s->x != -1; s++) {
        short i;
        int flag, sel;
        SUBSCREEN_DRAW_PARAMS.x = s->x;
        SUBSCREEN_DRAW_PARAMS.y = s->y;
        SUBSCREEN_DRAW_PARAMS.img = D_80012370;
        SUBSCREEN_DRAW_PARAMS.w = m->w10 + 2;
        i = s->idx + m->page * 8;
        if (i >= D_8009C60C) continue;
        sel = i == m->sel;
        flag = 0;
        switch (SUBSCREEN_ITEM_LIST[i]) {
        case 10:
            if (D_8009C25F == 1) flag = 1;
            break;
        case 12:
            if (D_8009C360 == 1) flag = 1;
            break;
        case 20:
            if (D_8009C260 == 1) flag = 1;
            break;
        case 24:
            if (D_8009C3F8[0] == 0xFF && D_8009C3F8[0x223] == 2) flag = 1;
            break;
        case 28:
            if (D_8009C3F8[0] == 0xFF && D_8009C3F8[0x223] == 3) flag = 1;
            break;
        case 29:
            if (D_8009C3F8[0] == 0xFF && D_8009C3F8[0x223] == 1) flag = 1;
            break;
        case 32:
            if (D_8009C3F8[0] == 0xFF && D_8009C3F8[0x223] == 0) flag = 1;
            break;
        case 98:
            if (D_8009BCF8 == 1) flag = 1;
            break;
        case 99:
            if (D_8009BCF8 == 2) flag = 1;
            break;
        case 100:
            if (D_8009BCF8 == 3) flag = 1;
            break;
        case 96:
            if (D_8009C36E != 0) flag = 1;
            break;
        case 132:
            if (D_8009C363 != 0) flag = 1;
            break;
        case 27:
            if ((unsigned)(G30[0] - 1) >= 2 && G30[0x922] == 0) flag = 1;
            break;
        case 33:
            if ((unsigned)(G30[0] - 1) >= 2 && G30[0x922] == 8) flag = 1;
            break;
        case 34:
            if ((unsigned)(G30[0] - 1) >= 2 && G30[0x922] == 9) flag = 1;
            break;
        case 21:
            if ((unsigned)(G30[0] - 1) >= 2 && G30[0x922] == 5) flag = 1;
            break;
        case 22:
            if ((unsigned)(G30[0] - 1) >= 2 && G30[0x922] == 6) flag = 1;
            break;
        case 23:
            if ((unsigned)(G30[0] - 1) >= 2 && G30[0x922] == 7) flag = 1;
            break;
        }
        subScreenDrawItemIcon(SUBSCREEN_ITEM_LIST[i], sel, flag, m->cursor);
    }
}
#endif
