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
extern Slot D_800FF354[];
extern unsigned char D_800FF254[];
extern Text D_80100B70;
extern int D_80100B80;
extern unsigned char SCRIPT_FLAGS[];
extern int D_800FFF28[];
extern int D_80100234;
extern char D_800F68FC[];
extern TILE *NEXT_PRIM;
extern unsigned long *CURRENT_OT;
extern void subScreenDrawString(int a, int b);

void subScreenDrawEventList(Menu *m)
{
    TILE *S = (TILE *)0x1F800000;
    TILE *p;
    Slot *s;
    for (s = D_800FF354; s->x != -1; s++) {
        int id;
        short k;
        D_80100B70.x = s->x + 0x14;
        D_80100B70.y = s->y - 8;
        D_80100B70.w = m->w10 + s->dw;
        id = D_800FF254[m->page * 10 + s->idx] - 1;
        k = id;
        if (SCRIPT_FLAGS[id] == 0 || SCRIPT_FLAGS[id] == 0xFF) D_80100B70.attr = 0x7C15;
        else D_80100B70.attr = 0x7C55;
        if (SCRIPT_FLAGS[k] != 0) D_80100B80 = D_800FFF28[k];
        else D_80100B80 = D_80100234;
        D_80100B70.p14 = D_800F68FC;
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
            S->x0 = D_80100B70.x - 0x20;
            S->y0 = D_80100B70.y - 1;
            p = NEXT_PRIM;
            *p = *S;
            addPrim(CURRENT_OT + 6, p);
            NEXT_PRIM++;
        }
    }
}
