// FUNC 800ea2c8 632 D0SPSUB3
// MATCHING 800ea2c8 632
typedef struct { unsigned addr: 24; unsigned len: 8; } P_TAG;
typedef struct {
    unsigned addr: 24;
    unsigned len: 8;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short w, h;
} TILE;
typedef struct { char p[0x10]; short w10; char p12[2]; short page; char p16[8]; short sel; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w, attr; char p8[0xC]; void *p14; } Text;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
extern Slot SUBSCREEN_EVENT_SLOTS[];
extern unsigned char SUBSCREEN_EVENT_ORDER[];
extern Text SUBSCREEN_DRAW_PARAMS;
extern int SUBSCREEN_DRAW_STR;
extern unsigned char SCRIPT_FLAGS[];
extern int EVENT_NAME_STRINGS[];
extern int EVENT_NAME_UNKNOWN_STRING;
extern char SUBSCREEN_FONT[];
extern TILE *NEXT_PRIM;
extern unsigned long *CURRENT_OT;
extern void subScreenDrawString(int a, int b);

void subScreenDrawEventList(Menu *m)
{
    TILE *S = (TILE *)0x1F800000;
    TILE *p;
    Slot *s;
    for (s = SUBSCREEN_EVENT_SLOTS; s->x != -1; s++) {
        int id;
        short k;
        SUBSCREEN_DRAW_PARAMS.x = s->x + 0x14;
        SUBSCREEN_DRAW_PARAMS.y = s->y - 8;
        SUBSCREEN_DRAW_PARAMS.w = m->w10 + s->dw;
        id = SUBSCREEN_EVENT_ORDER[m->page * 10 + s->idx] - 1;
        k = id;
        if (SCRIPT_FLAGS[id] == 0 || SCRIPT_FLAGS[id] == 0xFF) SUBSCREEN_DRAW_PARAMS.attr = 0x7C15;
        else SUBSCREEN_DRAW_PARAMS.attr = 0x7C55;
        if (SCRIPT_FLAGS[k] != 0) SUBSCREEN_DRAW_STR = EVENT_NAME_STRINGS[k];
        else SUBSCREEN_DRAW_STR = EVENT_NAME_UNKNOWN_STRING;
        SUBSCREEN_DRAW_PARAMS.p14 = SUBSCREEN_FONT;
        subScreenDrawString(5, 1);
        if (m->sel == m->page * 10 + s->idx) {
            S->len = 3;
            S->code = 0x60;
            S->g0 = 0xFF;
            S->w = 0xF0;
            S->h = 0x12;
            S->r0 = 0;
            S->b0 = 0;
            S->code |= 2;
            S->x0 = SUBSCREEN_DRAW_PARAMS.x - 0x20;
            S->y0 = SUBSCREEN_DRAW_PARAMS.y - 1;
            p = NEXT_PRIM;
            *p = *S;
            addPrim(CURRENT_OT + 6, p);
            NEXT_PRIM++;
        }
    }
}
