// FUNC 800edd28 480 D0SPSUB3
// MATCHING 800edd28 480
typedef struct { short x, y, w, h; } RECT;
typedef struct {
    unsigned char state, b1, b2, b3;
    char p4[0xc];
    short w10;
    char p12[4];
    short cursor;
    char p18[6];
    short sel;
    char p20[0x28];
    void *p48, *p4c, *p50, *p54, *p58, *p5c, *p60, *p64;
} Menu;
extern Menu MENU_STATE;
extern char D_800EEA00[], D_800EF600[], SUBSCREEN_TAB_FRAMES[], D_800F543C[], D_800F5448[], SUBSCREEN_DIGIT_FRAMES[], D_800F54D8[], D_800F57D8[], SUBSCREEN_TAB_EXTRA_FRAMES[], D_800F5400[];
extern void *D_1F8003A0;
extern unsigned short JOYPAD_STATE;
extern unsigned short PAD_PRESSED;
extern short SUBSCREEN_POPUP_CURSOR;
extern int SUBSCREEN_PAD_LAST, SUBSCREEN_PAD_REPEAT_TIMER;
extern void LoadImage(RECT *r, void *p);
extern int subScreenItemListStep(Menu *m);

static inline void menuInit(Menu *m)
{
    RECT r;
    r.x = 0x120;
    r.y = 0x1E0;
    r.w = 0x30;
    r.h = 0x1F;
    LoadImage(&r, D_800EEA00);
    D_1F8003A0 = D_800EF600;
    m->p48 = SUBSCREEN_TAB_FRAMES;
    m->p4c = D_800F5448;
    m->p50 = D_800F543C;
    m->p54 = SUBSCREEN_TAB_EXTRA_FRAMES + 8;
    m->p58 = D_800F5400 + 0x48;
    m->p5c = SUBSCREEN_DIGIT_FRAMES;
    m->p60 = D_800F57D8;
    m->p64 = D_800F54D8;
    m->w10 = 1;
    m->sel = -1;
}

int subScreenUpdate(int arg)
{
    Menu *m = &MENU_STATE;
    int k;
    switch (m->state) {
    case 0:
        menuInit(m);
        SUBSCREEN_POPUP_CURSOR = -1;
        m->cursor = 0;
        m->sel = -1;
        m->b1 = 0;
        m->b2 = 0;
        m->b3 = 0;
        m->state++;
    case 1:
        k = *(volatile unsigned short *)&PAD_PRESSED & 0xF0;
        if (k != SUBSCREEN_PAD_LAST) {
            SUBSCREEN_PAD_LAST = k;
            SUBSCREEN_PAD_REPEAT_TIMER = 0x20;
        } else if (--SUBSCREEN_PAD_REPEAT_TIMER == 0) {
            SUBSCREEN_PAD_REPEAT_TIMER = 4;
            JOYPAD_STATE |= k;
        }
        arg = subScreenItemListStep(m);
    }
    return arg;
}
