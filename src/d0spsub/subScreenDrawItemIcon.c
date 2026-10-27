// FUNC 800eb7a8 1296 D0SPSUB
// MATCHING 800eb7a8 1296
typedef struct { unsigned addr: 24; unsigned len: 8; } P_TAG;
typedef struct {
    unsigned addr: 24;
    unsigned len: 8;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short w, h;
} SPRT;
typedef struct {
    unsigned addr: 24;
    unsigned len: 8;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short w, h;
} TILE;
typedef struct { short next; unsigned char fn, a; short b, c; } Cond;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
typedef struct { unsigned short x, y; short w; } Text;
extern Text D_8011113C;
extern unsigned short *D_80111144;
extern char *D_1F8002D8;
#define BANK (*(char * volatile *)&D_1F8002D8)
extern SPRT *NEXT_PRIM;
extern unsigned long *CURRENT_OT;
extern unsigned char D_80109C88[];
extern unsigned char D_800FECE8[];
extern Cond *D_800FFAF8[];
extern int (*D_800FFBB0[])(int a, int b, int c);
extern void AddDrawMode(int a, int b);

void subScreenDrawItemIcon(int n, int sel, int flag, int nocheck)
{
    SPRT *S = (SPRT *)0x1F800000;
    TILE *T = (TILE *)0x1F800014;
    unsigned short *idx = (unsigned short *)0x1F800024;
    SPRT *p;
    TILE *q;
    S->len = 4;
    S->code = 0x64;
    S->code |= 1;
    S->r0 = 0x80;
    S->g0 = 0x80;
    S->b0 = 0x80;
    S->code &= ~2;
    S->w = 0x10;
    S->h = 0x10;
    S->x0 = D_8011113C.x;
    S->y0 = D_8011113C.y;
    S->u0 = n % 15 * 16 + 8;
    S->v0 = n / 15 * 16 + 8;
    S->clut = ((D_80109C88[n] + 0x1E0) << 6) | 0x13;
    p = NEXT_PRIM;
    *p = *S;
    addPrim(CURRENT_OT + 5, p);
    NEXT_PRIM++;
    AddDrawMode(D_8011113C.w, 5);
    if (flag) {
        unsigned char *e;
        unsigned short id;
        short *ent;
        id = *D_80111144;
        ent = (short *)BANK;
        ent += id * 2;
        *idx = id;
        e = ent[1] + (unsigned char *)BANK;
        S->len = 4;
        S->code = 0x65;
        S->r0 = 0x80;
        S->g0 = 0x80;
        S->b0 = 0x80;
        S->code &= ~2;
        S->x0 = D_8011113C.x + (signed char)e[0xE] - 4;
        S->y0 = D_8011113C.y + (signed char)e[0xF] + 8;
        S->u0 = e[0];
        S->v0 = e[1];
        S->w = e[0xA];
        S->h = e[0xB];
        S->clut = 0x78D7;
        p = NEXT_PRIM;
        *p = *S;
        addPrim(CURRENT_OT + 5, p);
        NEXT_PRIM++;
        D_8011113C.w = 0x15;
        AddDrawMode(0x15, 5);
    }
    if (sel) {
        T->len = 3;
        T->code = 0x60;
        T->r0 = 0;
        T->g0 = 0xFF;
        T->b0 = 0;
        if (!nocheck) {
            Cond *c = D_800FFAF8[D_800FECE8[n]];
            Cond *base = c;
            int r;
            for (;;) {
                r = D_800FFBB0[c->fn](c->a, c->b, c->c);
                if (c->next == -1) break;
                if (r) c = base + c->next;
                else c++;
            }
            if (!r) {
                T->r0 = 0;
                T->g0 = 0x60;
                T->b0 = 0;
            }
        }
        T->w = 0xF0;
        T->h = 0x12;
        T->code |= 2;
        T->x0 = D_8011113C.x - 1;
        T->y0 = D_8011113C.y - 1;
        q = (TILE *)NEXT_PRIM;
        *q = *T;
        addPrim(CURRENT_OT + 6, q);
        NEXT_PRIM = (SPRT *)((TILE *)NEXT_PRIM + 1);
    }
}
