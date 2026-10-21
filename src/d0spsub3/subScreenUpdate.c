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
extern Menu D_800A45D8;
extern char D_800EEA00[], D_800EF600[], D_800F5408[], D_800F543C[], D_800F5448[], D_800F54AC[], D_800F54D8[], D_800F57D8[], D_800F5434[], D_800F5400[];
extern void *D_1F8003A0;
extern unsigned short D_1F8001FC;
extern unsigned short D_8009D670;
extern short D_80100918;
extern int D_80100A88, D_80100A8C;
extern void LoadImage(RECT *r, void *p);
extern int func_800EC89C(Menu *m);

static inline void menuInit(Menu *m)
{
    RECT r;
    r.x = 0x120;
    r.y = 0x1E0;
    r.w = 0x30;
    r.h = 0x1F;
    LoadImage(&r, D_800EEA00);
    D_1F8003A0 = D_800EF600;
    m->p48 = D_800F5408;
    m->p4c = D_800F5448;
    m->p50 = D_800F543C;
    m->p54 = D_800F5434 + 8;
    m->p58 = D_800F5400 + 0x48;
    m->p5c = D_800F54AC;
    m->p60 = D_800F57D8;
    m->p64 = D_800F54D8;
    m->w10 = 1;
    m->sel = -1;
}

int subScreenUpdate(int arg)
{
    Menu *m = &D_800A45D8;
    int k;
    switch (m->state) {
    case 0:
        menuInit(m);
        D_80100918 = -1;
        m->cursor = 0;
        m->sel = -1;
        m->b1 = 0;
        m->b2 = 0;
        m->b3 = 0;
        m->state++;
    case 1:
        k = *(volatile unsigned short *)&D_8009D670 & 0xF0;
        if (k != D_80100A88) {
            D_80100A88 = k;
            D_80100A8C = 0x20;
        } else if (--D_80100A8C == 0) {
            D_80100A8C = 4;
            D_1F8001FC |= k;
        }
        arg = func_800EC89C(m);
    }
    return arg;
}
