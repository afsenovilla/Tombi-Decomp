// FUNC 800ea0c0 520 D0SPSUB3
// MATCHING 800ea0c0 520
typedef struct { char p[0x10]; short w10; char p12[2]; short page; char p16[0x4e]; unsigned short **icons; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w; } Text;
extern Slot SUBSCREEN_EVENT_SLOTS[];
extern unsigned char SUBSCREEN_EVENT_ORDER[];
extern Text SUBSCREEN_DRAW_PARAMS;
extern short SUBSCREEN_DRAW_CLUT;
extern unsigned short *SUBSCREEN_DRAW_FRAME;
extern unsigned char SCRIPT_FLAGS[];
extern void subScreenDrawSprite(int n);

void subScreenDrawEventIcons(Menu *m)
{
    Slot *s;
    for (s = SUBSCREEN_EVENT_SLOTS; s->x != -1; s++) {
        int id;
        short k;
        SUBSCREEN_DRAW_PARAMS.x = s->x;
        SUBSCREEN_DRAW_PARAMS.y = s->y;
        SUBSCREEN_DRAW_PARAMS.w = m->w10 + s->dw + 2;
        id = SUBSCREEN_EVENT_ORDER[m->page * 10 + s->idx];
        k = id - 1;
        if (id - 1 >= 0xFF) continue;
        switch (id) {
        case 0x57: SUBSCREEN_DRAW_CLUT = 0x7D12; break;
        case 0x58: SUBSCREEN_DRAW_CLUT = 0x7D52; break;
        case 0x59: SUBSCREEN_DRAW_CLUT = 0x7D92; break;
        case 0x75: SUBSCREEN_DRAW_CLUT = 0x7DD2; break;
        case 0x98: SUBSCREEN_DRAW_CLUT = 0x7A93; break;
        case 0x9E: SUBSCREEN_DRAW_CLUT = 0x7AD3; break;
        case 0xA0: SUBSCREEN_DRAW_CLUT = 0x7B13; break;
        case 0xA5: SUBSCREEN_DRAW_CLUT = 0x7B53; break;
        default: SUBSCREEN_DRAW_CLUT = 0; break;
        }
        if (k < 0) k = 0;
        if (SCRIPT_FLAGS[k] == 0xFF) {
            SUBSCREEN_DRAW_FRAME = m->icons[k];
            subScreenDrawSprite(1);
        }
    }
}
