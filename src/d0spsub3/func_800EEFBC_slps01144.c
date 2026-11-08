// VERSION SLPS_01144 dspsub3 800EEFBC 496
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub3 800edd28 (subScreenUpdate.c): extra p20 frame table, held pad 0xF000
typedef struct { short x, y, w, h; } RECT;
typedef struct {
    unsigned char state, b1, b2, b3;
    char p4[0xc];
    short w10;
    char p12[4];
    short cursor;
    char p18[6];
    short sel;
    void *p20;
    char p24[0x24];
    void *p48, *p4c, *p50, *p54, *p58, *p5c, *p60, *p64;
} Menu;
extern Menu D_800A6120;
extern char D_800EFE28[], D_800F0A28[], D_800F8E88[], D_800F9598[], D_800F95D0[], D_800F95B8[], D_800F95C4[], D_800F95E0[], D_800F9644[], D_800F9970[], D_800F9670[];
extern void *D_1F8003A0;
extern unsigned short JOYPAD_STATE;
extern unsigned short D_800A1380;
extern short D_800FF324;
extern int D_800FF494, D_800FF498;
extern void LoadImage(RECT *r, void *p);
extern int func_800EDC50(Menu *m);

static inline void menuInit(Menu *m)
{
    RECT r;
    r.x = 0x120;
    r.y = 0x1E0;
    r.w = 0x30;
    r.h = 0x1F;
    LoadImage(&r, D_800EFE28);
    D_1F8003A0 = D_800F0A28;
    m->p20 = D_800F8E88;
    m->p48 = D_800F9598;
    m->p4c = D_800F95D0;
    m->p50 = D_800F95B8;
    m->p54 = D_800F95C4;
    m->p58 = D_800F95E0;
    m->p5c = D_800F9644;
    m->p60 = D_800F9970;
    m->p64 = D_800F9670;
    m->w10 = 1;
    m->sel = -1;
}

int func_800EEFBC(int arg)
{
    Menu *m = &D_800A6120;
    int k;
    switch (m->state) {
    case 0:
        menuInit(m);
        D_800FF324 = -1;
        m->cursor = 0;
        m->sel = -1;
        m->b1 = 0;
        m->b2 = 0;
        m->b3 = 0;
        m->state++;
    case 1:
        k = *(volatile unsigned short *)&D_800A1380 & 0xF000;
        if (k != D_800FF494) {
            D_800FF494 = k;
            D_800FF498 = 0x20;
        } else if (--D_800FF498 == 0) {
            D_800FF498 = 4;
            JOYPAD_STATE |= k;
        }
        arg = func_800EDC50(m);
    }
    return arg;
}
#endif
