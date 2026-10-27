// FUNC 800eabf4 652 D0SPSUB3
// MATCHING 800eabf4 652
typedef struct { char p[0x10]; short w10; char p12[2]; short page; char p16[8]; short sel; char p20[0x3C]; int *p5c; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w, attr; int img; int pC; int str; void *p14; } Text;
extern Slot SUBSCREEN_ITEM_SLOTS[];
extern Text SUBSCREEN_DRAW_PARAMS;
extern unsigned short D_8009D2A4;
extern unsigned char SUBSCREEN_ITEM_LIST[];
extern unsigned char ITEM_NAME_INDEX[];
extern int ITEM_NAME_STRINGS[];
extern unsigned char ITEM_NAME_CLUTS[];
extern unsigned char D_8009D0A4[];
extern char SUBSCREEN_FONT[];
extern void subScreenDrawString(int a, int b);
extern void subScreenDrawSprite(int n);

void subScreenDrawItemNames(Menu *m)
{
    Slot *s;
    for (s = SUBSCREEN_ITEM_SLOTS; s->x != -1; s++) {
        int i;
        int n;
        SUBSCREEN_DRAW_PARAMS.x = s->x + 0x18;
        SUBSCREEN_DRAW_PARAMS.y = s->y;
        SUBSCREEN_DRAW_PARAMS.w = m->w10 + 1;
        i = m->page * 10 + s->idx;
        if (i >= D_8009D2A4) continue;
        SUBSCREEN_DRAW_PARAMS.str = ITEM_NAME_STRINGS[ITEM_NAME_INDEX[SUBSCREEN_ITEM_LIST[i]]];
        SUBSCREEN_DRAW_PARAMS.p14 = SUBSCREEN_FONT;
        if (i == m->sel) SUBSCREEN_DRAW_PARAMS.attr = 0x7C15;
        else SUBSCREEN_DRAW_PARAMS.attr = (((ITEM_NAME_CLUTS[SUBSCREEN_ITEM_LIST[i]] & 0x7F) + 0x1F0) << 6) | 0x15;
        subScreenDrawString(5, 1);
        n = D_8009D0A4[SUBSCREEN_ITEM_LIST[i]];
        if (ITEM_NAME_CLUTS[SUBSCREEN_ITEM_LIST[i]] & 0x80) {
            SUBSCREEN_DRAW_PARAMS.x += 0xB0;
            SUBSCREEN_DRAW_PARAMS.y += 0x10;
            SUBSCREEN_DRAW_PARAMS.w = m->w10;
            SUBSCREEN_DRAW_PARAMS.img = m->p5c[10];
            subScreenDrawSprite(1);
            SUBSCREEN_DRAW_PARAMS.x += 0xA;
            if (n >= 10) {
                SUBSCREEN_DRAW_PARAMS.img = m->p5c[n / 10 % 10];
                subScreenDrawSprite(1);
            }
            SUBSCREEN_DRAW_PARAMS.x += 0xA;
            SUBSCREEN_DRAW_PARAMS.img = m->p5c[n % 10];
            subScreenDrawSprite(1);
        }
    }
}
