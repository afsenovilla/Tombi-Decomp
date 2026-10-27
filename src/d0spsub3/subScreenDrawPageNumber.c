// FUNC 800eae80 544 D0SPSUB3
// MATCHING 800eae80 544
typedef struct { char p[0x10]; short w10; char p12[2]; short page; char p16[0x42]; unsigned short **icons; unsigned short **digits; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w, attr; unsigned short *img; } Text;
extern Slot SUBSCREEN_PAGE_FOOTER_SLOTS[];
extern Text SUBSCREEN_DRAW_PARAMS;
extern unsigned short *SUBSCREEN_DRAW_FRAME;
extern void subScreenDrawSprite(int n);

void subScreenDrawPageNumber(Menu *m, int n)
{
    Slot *s;
    int t;
    for (s = SUBSCREEN_PAGE_FOOTER_SLOTS; s->x != -1; s++) {
        SUBSCREEN_DRAW_PARAMS.x = s->x;
        SUBSCREEN_DRAW_PARAMS.y = s->y;
        SUBSCREEN_DRAW_PARAMS.w = m->w10 + s->dw;
        SUBSCREEN_DRAW_PARAMS.attr = 0x7C15;
        switch (s->idx) {
        case -1:
            t = (m->page + 1) / 10;
            if (t != 0) {
                SUBSCREEN_DRAW_PARAMS.img = m->digits[t];
                subScreenDrawSprite(1);
            }
            break;
        case -2:
            t = (m->page + 1) % 10;
            SUBSCREEN_DRAW_PARAMS.img = m->digits[t];
            subScreenDrawSprite(1);
            break;
        case -3:
            t = n / 10;
            if (t != 0) {
                SUBSCREEN_DRAW_PARAMS.img = m->digits[t];
                subScreenDrawSprite(1);
            }
            break;
        case -4:
            t = n % 10;
            SUBSCREEN_DRAW_PARAMS.img = m->digits[t];
            subScreenDrawSprite(1);
            break;
        default:
            SUBSCREEN_DRAW_FRAME = m->icons[s->idx];
            subScreenDrawSprite(1);
            break;
        }
    }
}
