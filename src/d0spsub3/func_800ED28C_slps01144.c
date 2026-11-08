// VERSION SLPS_01144 dspsub3 800ED28C 1108
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800ebf34 (subScreenDrawPopupMenu.c)
// USA version of PAL d0spsub 800EBF34 (func_800ED28C.c): PAL C of src/d0spsub/func_800ED28C.c (1.000 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
typedef struct { unsigned addr: 24; unsigned len: 8; } P_TAG;
typedef struct {
    unsigned addr: 24;
    unsigned len: 8;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short w, h;
} TILE;
typedef struct { short x0, x1, x2, x3; short a, b, c, d; short m; short pad[3]; } L;
typedef struct { short x, y, w, attr; int img, pC, str; void *p14; } Text;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
extern TILE *NEXT_PRIM;
extern unsigned long *CURRENT_OT;
extern Text D_800FF57C;
extern short D_800FF324, D_800FF328, D_800FF32C, D_800FF330;
extern short D_800FF334[];
extern short D_800FF354[];
extern int D_800FE4A4[];
extern int *D_800FE640[];
extern char D_800FAABC[];
extern void func_800ED024(int pri);
extern void func_800498AC_slps01144(L *l, int c, int tp);

void func_800ED28C(void)
{
    TILE *T = (TILE *)0x1F800014;
    TILE *p;
    char env[0x60];
    L l;
    L *lp;
    int i;
    for (i = 0; i < D_800FF328; i++) {
        D_800FF57C.x = 0x2c;
        D_800FF57C.y = (short)(i * 16 + 0x78) - D_800FF328 * 8 + 12;
        D_800FF57C.w = 2;
        D_800FF57C.attr = 0x7C15;
        D_800FF57C.str = D_800FE4A4[D_800FF334[i]];
        D_800FF57C.p14 = D_800FAABC;
        func_800ED024(1);
    }
    lp = &l;
    p = NEXT_PRIM;
    T->len = 3;
    T->code = 0x60;
    T->g0 = 0xFF;
    T->x0 = 0x2c;
    T->w = 0x70;
    T->r0 = 0;
    T->b0 = 0;
    T->h = 0x10;
    T->code &= ~2;
    T->y0 = (short)(D_800FF324 * 16 + 0x78) - D_800FF328 * 8 + 6;
    *p = *T;
    addPrim(CURRENT_OT + 2, p);
    NEXT_PRIM++;
    l.m = -1;
    l.a = 40;
    l.b = 0x78 - D_800FF328 * 8;
    l.c = 0x78;
    l.d = D_800FF328 * 16 + 0xC;
    func_800498AC_slps01144(lp, 0, 0x14);
    if (D_800FF32C >= 0) {
        for (i = 0; i < D_800FF330; i++) {
            D_800FF57C.x = 0xA4;
            D_800FF57C.y = (short)(i * 16 + 0x78) - D_800FF330 * 8 + 12;
            D_800FF57C.w = 2;
            D_800FF57C.attr = 0x7C15;
            D_800FF57C.str = D_800FE640[D_800FF334[D_800FF324]][D_800FF354[i]];
            D_800FF57C.p14 = D_800FAABC;
            func_800ED024(1);
        }
        p = NEXT_PRIM;
        T->len = 3;
        T->code = 0x60;
        T->g0 = 0xFF;
        T->x0 = 0xA4;
        T->w = 0x70;
        T->r0 = 0;
        T->b0 = 0;
        T->h = 0x10;
        T->code &= ~2;
        T->y0 = (short)(D_800FF32C * 16 + 0x78) - D_800FF330 * 8 + 6;
        *p = *T;
        addPrim(CURRENT_OT + 2, p);
        NEXT_PRIM++;
        lp->m = -1;
        lp->a = 0xA0;
        lp->b = 0x78 - D_800FF330 * 8;
        lp->c = 0x78;
        lp->d = D_800FF330 * 16 + 0xC;
        func_800498AC_slps01144(lp, 0, 0x14);
    }
}
#endif
