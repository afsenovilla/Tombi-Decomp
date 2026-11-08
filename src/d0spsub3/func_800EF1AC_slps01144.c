// VERSION SLPS_01144 dspsub3 800EF1AC 936
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub3 800edf08 (subScreenDrawItemPage.c): tab frames drawn here, mode title, 16 items per page
typedef struct {
    unsigned char state, b1, b2, b3;
    char p4[0xc];
    short w10;
    short mode;
    short page;
    char p16[0x32];
    void **p48, **p4c;
    char p50[8];
    void **p58;
    char p5c[4];
    void **p60;
} Menu;
typedef struct { short x, y, dw, idx; } Slot;
extern Menu D_800A6120;
extern Slot D_800FE6E4[], D_800FE6CC[], D_800FF154[], D_800FF1E4[];
extern unsigned short D_8009EE4C;
extern short D_800FF324;
extern short D_800FF57C[], D_800FF57E[], D_800FF580[], D_800FF582[];
extern void *D_800FF584[];
extern void func_800EB590(Menu *m);
extern void func_800ED28C(Menu *m);
extern void func_800EBF08(Menu *m);
extern void func_800EC25C(Menu *m);
extern void subScreenDrawPageNumber(Menu *m, int n);
extern void func_800EC920(int n);

void func_800EF1AC(void)
{
    Menu *m = &D_800A6120;
    int pages;
    Slot *s;
    short end;
    func_800EB590(m);
    switch (D_800A6120.mode) {
    case 1:
    case 2:
    case 3:
        s = D_800FE6E4;
        break;
    case 0:
    case 7:
        s = D_800FE6CC;
        break;
    }
    D_800FF580[0] = m->w10 + s->dw;
    for (; s->x != -1; s++) {
        int k;
        short y;
        D_800FF57C[0] = s->x;
        y = s->y;
        asm volatile("li %0,0x7A92" : "=r"(k)); /* the game reloads the constant in the loop */
        D_800FF582[0] = k;
        D_800FF57E[0] = y;
        if (s->idx == 4) D_800FF580[1] = k; /* == D_800FF582[0]: its own address load */
        D_800FF584[0] = m->p48[s->idx];
        func_800EC920(1);
    }
    if (D_800FF324 >= 0) func_800ED28C(m);
    pages = (D_8009EE4C + 15) / 16;
    func_800EBF08(m);
    func_800EC25C(m);
    if (pages >= 2) {
        subScreenDrawPageNumber(m, pages);
        s = D_800FF154;
        while (s->x != -1) {
            if ((s->idx == 0x16 && m->page == 0) || (s->idx == 0x17 && m->page >= pages - 1)) {
                s++;
            } else {
                D_800FF57C[0] = s->x;
                D_800FF57E[0] = s->y;
                D_800FF580[0] = m->w10 + s->dw;
                D_800FF582[0] = 0x7C15;
                D_800FF584[0] = m->p58[s->idx];
                func_800EC920(1);
                s++;
            }
        }
    }
    D_800FF582[0] = 0x7912;
    D_800FF580[0] = m->w10;
    if (m->mode >= 6) {
        D_800FF57C[0] = 0x20;
        D_800FF57E[0] = 0x10;
        D_800FF584[0] = m->p4c[1];
    } else {
        D_800FF57C[0] = 0x20;
        if (m->mode == 3) D_800FF57E[0] = 0x18;
        else D_800FF57E[0] = 0x10;
        D_800FF584[0] = m->p4c[m->mode];
    }
    func_800EC920(1);
    end = -1;
    D_800FF582[0] = 0x7BD3;
    for (s = D_800FF1E4; s->x != end; s++) {
        D_800FF57C[0] = s->x;
        D_800FF57E[0] = s->y;
        D_800FF580[0] = m->w10 + s->dw;
        D_800FF584[0] = m->p60[s->idx];
        func_800EC920(3);
    }
}
#endif
