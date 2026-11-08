// VERSION SLPS_01144 dspsub3 800EB9A0 604
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800ea370 (dspsub3 copy; subScreenDrawEventList.c), from the USA one
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
extern Slot D_800FE0BC[];
extern unsigned char D_800FDFBC[];
extern Text D_800FF57C;
extern int D_800FF58C;
extern unsigned char SCRIPT_FLAGS[];
extern int D_800FE920[];
extern int D_800FEC2C;
extern char D_800FAABC[];
extern TILE *NEXT_PRIM;
extern unsigned long *CURRENT_OT;
extern void func_800ED024(int pri);

void func_800EB9A0(Menu *m)
{
    TILE *S = (TILE *)0x1F800000;
    TILE *p;
    Slot *s;
    for (s = D_800FE0BC; s->x != -1; s++) {
        int id;
        short k;
        D_800FF57C.x = s->x + 0x14;
        D_800FF57C.y = s->y - 3;
        D_800FF57C.w = m->w10 + s->dw;
        id = D_800FDFBC[m->page * 16 + s->idx] - 1;
        k = id;
        if (SCRIPT_FLAGS[id] == 0 || SCRIPT_FLAGS[id] == 0xFF) D_800FF57C.attr = 0x7C15;
        else D_800FF57C.attr = 0x7C55;
        if (SCRIPT_FLAGS[k] != 0) D_800FF58C = D_800FE920[k];
        else D_800FF58C = D_800FEC2C;
        D_800FF57C.p14 = D_800FAABC;
        func_800ED024(5);
        if (m->sel == m->page * 16 + s->idx) {
            S->len = 3;
            S->code = 0x60;
            S->g0 = 0xFF;
            S->w = 0x9A;
            S->h = 0x12;
            S->r0 = 0;
            S->b0 = 0;
            S->code |= 2;
            S->x0 = D_800FF57C.x - 0x20;
            S->y0 = D_800FF57C.y - 6;
            p = NEXT_PRIM;
            *p = *S;
            addPrim(CURRENT_OT + 6, p);
            NEXT_PRIM++;
        }
    }
}
#endif
