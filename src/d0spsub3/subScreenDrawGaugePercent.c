// FUNC 800eb0a0 428 D0SPSUB3
// MATCHING 800eb0a0 428
typedef struct { char p[0x10]; short w10; char p12[0x46]; int *p58; int *p5c; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w; unsigned short attr; int img; } Text;
extern Slot SUBSCREEN_GAUGE_SLOTS[];
extern Text SUBSCREEN_DRAW_PARAMS;
extern unsigned char D_8009CD9C[];
extern void subScreenDrawSprite(int n);

void subScreenDrawGaugePercent(Menu *m, int i)
{
    Slot *s = &SUBSCREEN_GAUGE_SLOTS[i];
    int pct;
    int dx;
    SUBSCREEN_DRAW_PARAMS.x = s->x;
    SUBSCREEN_DRAW_PARAMS.y = s->y;
    SUBSCREEN_DRAW_PARAMS.w = m->w10 + s->dw;
    SUBSCREEN_DRAW_PARAMS.attr = 0x7C15;
    SUBSCREEN_DRAW_PARAMS.img = m->p58[s->idx];
    subScreenDrawSprite(1);
    pct = (60 - D_8009CD9C[i]) * 100 / 60;
    dx = 0;
    if (pct == 100) {
        SUBSCREEN_DRAW_PARAMS.x = s->x + 0x26;
        SUBSCREEN_DRAW_PARAMS.y = s->y;
        SUBSCREEN_DRAW_PARAMS.img = m->p5c[1];
        subScreenDrawSprite(1);
        dx = 8;
    }
    { int o = dx + 0x26; SUBSCREEN_DRAW_PARAMS.x = s->x + o; }
    SUBSCREEN_DRAW_PARAMS.y = s->y;
    SUBSCREEN_DRAW_PARAMS.img = m->p5c[pct / 10 % 10];
    subScreenDrawSprite(1);
}
