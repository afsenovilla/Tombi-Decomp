// VERSION SLPS_01144 slps01144 8004CB14 1060
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
typedef struct { unsigned tag; unsigned char r0, g0, b0, code; short x0, y0, w, h; } TILE;
typedef struct {
    unsigned tag; unsigned char r0, g0, b0, code;
    short x0, y0; int uv0[0]; unsigned char u0, v0; unsigned short clut; short x1, y1; int uv1; short x2, y2; unsigned short uv2, pad1; short x3, y3; unsigned short uv3, pad2;
} FT4;
typedef struct { int uv0; int uv1; int uv2; } PT;

extern char *NEXT_PRIM;
extern int CURRENT_OT;
extern char *DAT_1f8002d8;
extern unsigned char GAME_INVENTORY_SCREEN[];
extern unsigned short D_800783F0[], D_800783F8[], D_80078404[];
extern short **D_800783CC[], **D_800783E8[];
extern void SetSemiTrans(void *, int);
extern void AddPrim(void *, void *);
extern void SetDrawMode(void *, int, int, int, void *);
extern unsigned short GetClut(int, int);

void func_8004CB14_slps01144(void)
{
    TILE *t;
    void *r;
    short **h;
    FT4 *p;
    short n;
    int i, w, h2, dx; short dxs;
    unsigned char *q, *pt;
    unsigned short *px, *py, *a, *b;

    t = (TILE *)NEXT_PRIM;
    t->w = 0x140;
    t->h = 0xf0;
    t->x0 = 0;
    t->y0 = 0;
    ((unsigned char *)t)[3] = 3;
    t->code = 0x60;
    SetSemiTrans(t, 1);
    t->r0 = 0x20;
    t->g0 = 0x20;
    t->b0 = 0x20;
    AddPrim((void *)(CURRENT_OT + 0x10), t);
    r = NEXT_PRIM = NEXT_PRIM + 0x10;
    SetDrawMode(r, 0, 0, 0, 0);
    AddPrim((void *)(CURRENT_OT + 0x10), r);
    NEXT_PRIM = NEXT_PRIM + 0xc;
    if (GAME_INVENTORY_SCREEN[0] < 4) {
        t = (TILE *)NEXT_PRIM;
        t->w = 0x30;
        t->h = 0xc;
        t->x0 = 0x88;
        t->y0 = D_800783F0[GAME_INVENTORY_SCREEN[0]];
        ((unsigned char *)t)[3] = 3;
        t->code = 0x60;
        SetSemiTrans(t, 1);
        t->r0 = 0xff;
        t->g0 = 0xff;
        t->b0 = 0;
        AddPrim((void *)(CURRENT_OT + 4), t);
        r = NEXT_PRIM = NEXT_PRIM + 0x10;
        SetDrawMode(r, 0, 0, 0, 0);
        AddPrim((void *)(CURRENT_OT + 4), r);
        NEXT_PRIM = NEXT_PRIM + 0xc;
    }
    i = 1;
    if (GAME_INVENTORY_SCREEN[0] < 4)
        i = 5;
    if (i >= 0) {
    do {
    {   /* permuter: the do/while(0) keeps the px address in v0, as retail */
        int k = i;
        py = &D_800783F8[k];
        do { px = &D_80078404[k]; } while (0);
    }
    do {
        if (GAME_INVENTORY_SCREEN[0] < 4)
            h = D_800783CC[i];
        else
            h = D_800783E8[i];
        a = px;
        b = py;
        {
            short *hh = (short *)(DAT_1f8002d8 + **h * 4);
            n = hh[0];
            pt = (unsigned char *)DAT_1f8002d8 + hh[1];
        }
        q = pt + 11;
        do {
            p = (FT4 *)NEXT_PRIM;
            ((unsigned char *)p)[3] = 9;
            p->code = 0x2d;
            if (i == 0)
                SetSemiTrans(p, 1);
            else
                SetSemiTrans(p, 0);
            *(int *)&p->u0 = *(int *)pt;
            p->uv1 = *(int *)(q - 7);
            p->uv2 = *(int *)(q - 3);
            p->uv3 = *(int *)(q + 1);
            if (i != 0) {
                ((unsigned char *)p)[20] = ((unsigned char *)p)[20] + 1;
                ((unsigned char *)p)[36] = ((unsigned char *)p)[36] + 1;
                ((unsigned char *)p)[29] = ((unsigned char *)p)[29] + 1;
                ((unsigned char *)p)[37] = ((unsigned char *)p)[37] + 1;
            }
            *(short *)((char *)p + 0x16) = 0x14;
            dxs = ((signed char *)q)[3] + 160;
            p->x0 = *a + dxs;
            p->y0 = *b + ((signed char *)q)[4];
            w = q[-1];
            p->x2 = p->x0;
            p->x1 = p->x0 + w;
            p->y1 = p->y0;
            p->y2 = p->y0 + q[0];
            p->x3 = p->x1;
            p->y3 = p->y2;
            if (i == 0) {
                p->clut = GetClut(0x160, 0x1e5);
                AddPrim((void *)(CURRENT_OT + 8), p);
            } else {
                p->clut = GetClut(0x160, 0x1e3);
                AddPrim((void *)CURRENT_OT, p);
            }
            q += 16;
            n--;
            NEXT_PRIM = NEXT_PRIM + 0x28;
            pt += 16;
        } while (n != 0);
    } while (py--, i--, px--, i >= 0);
    } while (0);
    }
}
#endif
