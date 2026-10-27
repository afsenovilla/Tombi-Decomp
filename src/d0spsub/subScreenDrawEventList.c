// FUNC 800ea370 632 D0SPSUB
// MATCHING 800ea370 632
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
extern Slot D_801087F0[];
extern unsigned char D_801086F0[];
extern Text D_8011113C;
extern int D_8011114C;
extern unsigned char SCRIPT_FLAGS[];
extern int D_801093C4[];
extern int D_801096D0;
extern char D_800FFD98[];
extern TILE *NEXT_PRIM;
extern unsigned long *CURRENT_OT;
extern void subScreenDrawString(int a, int b);

void subScreenDrawEventList(Menu *m)
{
    TILE *S = (TILE *)0x1F800000;
    TILE *p;
    Slot *s;
    for (s = D_801087F0; s->x != -1; s++) {
        int id;
        short k;
        D_8011113C.x = s->x + 0x14;
        D_8011113C.y = s->y - 8;
        D_8011113C.w = m->w10 + s->dw;
        id = D_801086F0[m->page * 10 + s->idx] - 1;
        k = id;
        if (SCRIPT_FLAGS[id] == 0 || SCRIPT_FLAGS[id] == 0xFF) D_8011113C.attr = 0x7C15;
        else D_8011113C.attr = 0x7C55;
        if (SCRIPT_FLAGS[k] != 0) D_8011114C = D_801093C4[k];
        else D_8011114C = D_801096D0;
        D_8011113C.p14 = D_800FFD98;
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
            S->x0 = D_8011113C.x - 0x20;
            S->y0 = D_8011113C.y - 1;
            p = NEXT_PRIM;
            *p = *S;
            addPrim(CURRENT_OT + 6, p);
            NEXT_PRIM++;
        }
    }
}
