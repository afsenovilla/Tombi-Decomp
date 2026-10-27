// FUNC 800e9df8 712 D0SPSUB3
// MATCHING 800e9df8 712
typedef struct { char p[0x10]; short w10; short mode; char p14[2]; short cursor; char p18[0x30]; int *p48; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w, attr; int img; } Text;
extern Slot SUBSCREEN_TAB_SLOTS_MAIN[], SUBSCREEN_TAB_SLOTS_MAP[], SUBSCREEN_TAB_SLOTS_MODE7[], SUBSCREEN_TAB_SLOT_MINUS2[];
extern short SUBSCREEN_TAB_IDS[][12];
extern int SUBSCREEN_TAB_EXTRA_FRAMES[];
extern Text SUBSCREEN_DRAW_PARAMS;
extern short SUBSCREEN_DRAW_CLUT;
extern int SUBSCREEN_DRAW_FRAME;
extern void subScreenDrawSprite(int n);
extern void subScreenDrawSpriteFT4(int n);

void subScreenDrawTabs(Menu *m)
{
    Slot *s;
    switch (m->mode) {
    case 0:
    case 1:
    case 2:
        s = SUBSCREEN_TAB_SLOTS_MAIN;
        break;
    case 3:
        s = SUBSCREEN_TAB_SLOTS_MAP;
        break;
    case 7:
        s = SUBSCREEN_TAB_SLOTS_MODE7;
        break;
    }
    for (; s->x != -1; s++) {
        short id;
        int idx, mode;
        SUBSCREEN_DRAW_PARAMS.x = s->x;
        SUBSCREEN_DRAW_PARAMS.y = s->y;
        SUBSCREEN_DRAW_PARAMS.w = m->w10 + s->dw;
        mode = m->mode;
        idx = s->idx;
        id = SUBSCREEN_TAB_IDS[mode][idx];
        if (id != -1) {
            if (idx < 4) {
                if (idx == m->cursor) {
                    SUBSCREEN_DRAW_PARAMS.attr = -0x6EE;
                    SUBSCREEN_DRAW_PARAMS.img = m->p48[7];
                    subScreenDrawSprite(2);
                    SUBSCREEN_DRAW_PARAMS.attr = 0x7BD2;
                } else {
                    SUBSCREEN_DRAW_PARAMS.attr = 0x7912;
                }
            } else if (idx == m->cursor) {
                SUBSCREEN_DRAW_PARAMS.attr = 0x7F52;
            } else if ((idx == 4 && mode == 1) || (idx == 5 && mode == 2) || (idx == 6 && mode == 0) ||
                       (idx == 7 && mode == 3) || (idx == 4 && mode == 7)) {
                SUBSCREEN_DRAW_CLUT = 0x7F92;
            } else {
                SUBSCREEN_DRAW_CLUT = 0x7852;
            }
            if (s->x == -2) {
                s = SUBSCREEN_TAB_SLOT_MINUS2;
                SUBSCREEN_DRAW_PARAMS.x = s->x;
                SUBSCREEN_DRAW_PARAMS.y = s->y;
                SUBSCREEN_DRAW_PARAMS.w = m->w10 + s->dw;
                SUBSCREEN_DRAW_PARAMS.img = SUBSCREEN_TAB_EXTRA_FRAMES[0];
                subScreenDrawSpriteFT4(1);
                s++;
                SUBSCREEN_DRAW_PARAMS.x = s->x;
                SUBSCREEN_DRAW_PARAMS.y = s->y;
                SUBSCREEN_DRAW_PARAMS.w = m->w10 + s->dw;
                SUBSCREEN_DRAW_PARAMS.img = SUBSCREEN_TAB_EXTRA_FRAMES[1];
                subScreenDrawSpriteFT4(1);
            } else {
                SUBSCREEN_DRAW_FRAME = m->p48[id];
                subScreenDrawSprite(1);
            }
        }
    }
}
