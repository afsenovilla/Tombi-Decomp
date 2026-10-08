// FUNC 800597bc 1416 MAIN0
// MATCHING 800597bc 1416
typedef struct { unsigned tag; unsigned char r0, g0, b0, code; short x0, y0, w, h; } TILE;
typedef struct { unsigned tag; unsigned char r0, g0, b0, code; short x0, y0; unsigned char u0, v0; unsigned short clut; short w, h; } SPRT;
typedef struct {
    unsigned tag; unsigned char r0, g0, b0, code;
    short x0, y0; int uv0[0]; unsigned char u0, v0; unsigned short clut; short x1, y1; int uv1; short x2, y2; unsigned short uv2, pad1; short x3, y3; unsigned short uv3, pad2;
} FT4;
typedef struct { int uv0; int uv1; unsigned short uv2; unsigned char w, h; unsigned short uv3; signed char dx, dy; } PT;
typedef struct { short x, y; unsigned char *s; } ENT;
typedef struct { unsigned char u, v; } UV;
typedef struct { short x, y; } XY;

extern char *DAT_1f800164;
extern int DAT_1f8001e0;
extern char *DAT_1f8002d8;
extern unsigned char DAT_1f8003cd;
extern unsigned char DAT_8009cda3[];
extern ENT D_8008037c[], D_8008039c[], D_800803bc[], D_800803dc[];
extern UV D_80080300[];
extern XY D_80080404[];
extern short *D_80012284[];
extern void SetSemiTrans(void *, int);
extern void AddPrim(void *, void *);
extern void SetDrawMode(void *, int, int, int, void *);
extern unsigned short GetClut(int, int);
extern int GetGraphType(void);

void FUN_800597bc(void)
{
    ENT *e;
    int mode;
    unsigned char *s;
    unsigned char c;
    unsigned short x;
    short y;
    TILE *t;
    SPRT *sp;
    void *r;
    int tp;
    short *h;
    PT *pt;
    FT4 *p;
    short n;
    int w;
    int k;

    switch (DAT_1f8003cd) {
    case 0:
        e = D_800803dc;
        mode = 1;
        break;
    case 1:
        e = D_8008037c;
        mode = 0;
        break;
    case 2:
        e = D_8008039c;
        mode = 2;
        break;
    case 3:
        mode = 2;
        e = D_800803bc;
        break;
    }
    t = (TILE *)DAT_1f800164;
    t->w = 0x140;
    t->h = 0x100;
    ((unsigned char *)t)[3] = 3;
    t->x0 = 0;
    t->y0 = 0;
    t->code = 0x60;
    SetSemiTrans(t, 1);
    t->r0 = 0x20;
    t->g0 = 0x20;
    t->b0 = 0x20;
    AddPrim((void *)(DAT_1f8001e0 + 0x10), t);
    r = DAT_1f800164 = DAT_1f800164 + 0x10;
    SetDrawMode(r, 0, 0, 0, 0);
    AddPrim((void *)(DAT_1f8001e0 + 0x10), r);
    DAT_1f800164 = DAT_1f800164 + 0xc;
    for (; e->x != -1; e++) {
        x = e->x;
        y = e->y;
        s = e->s;
        while ((c = *s) != 0xff) {
            s++;
            if (c) {
                sp = (SPRT *)DAT_1f800164;
                ((unsigned char *)sp)[3] = 4;
                sp->code = 0x65;
                SetSemiTrans(sp, 0);
                sp->x0 = x;
                sp->y0 = y;
                sp->u0 = D_80080300[c].u;
                sp->v0 = D_80080300[c].v;
                sp->w = 8;
                sp->h = 0x10;
                sp->clut = GetClut(0x160, 0x1e3);
                AddPrim((void *)(DAT_1f8001e0 + 4), sp);
                DAT_1f800164 = DAT_1f800164 + 0x14;
            }
            x += 8;
        }
    }
    r = DAT_1f800164;
    if (GetGraphType() == 1)
        tp = 0x24;
    else if (GetGraphType() == 2)
        tp = 0x24;
    else
        tp = 0x14;
    SetDrawMode(r, 0, 0, tp, 0);
    AddPrim((void *)(DAT_1f8001e0 + 4), r);
    t = (TILE *)(DAT_1f800164 = DAT_1f800164 + 0xc);
    switch (mode) {
    case 0:
        t->x0 = 0x74;
        k = DAT_8009cda3[0];
        t->w = 0x50;
        t->h = 0x10;
        k <<= 4; k += 0x58;
        t->y0 = k;
        break;
    case 1:
        t->x0 = 0x84;
        k = DAT_8009cda3[0];
        t->w = 0x38;
        t->h = 0x10;
        k <<= 4; k += 0x58;
        t->y0 = k;
        break;
    case 3:
        t->x0 = 0x74;
        k = DAT_8009cda3[0];
        t->w = 0x58;
        t->h = 0x10;
        k <<= 4; k += 0x58;
        t->y0 = k;
        break;
    case 2:
    case 4:
        k = DAT_8009cda3[0];
        t->y0 = 0x78;
        t->w = 0x28;
        t->h = 0x10;
        t->x0 = k * 0x30 + 0x74;
        break;
    }
    ((unsigned char *)t)[3] = 3;
    t->code = 0x60;
    SetSemiTrans(t, 1);
    t->r0 = 0xff;
    t->g0 = 0xff;
    t->b0 = 0;
    AddPrim((void *)(DAT_1f8001e0 + 4), t);
    DAT_1f800164 = DAT_1f800164 + 0x10;
    h = (short *)(DAT_1f8002d8 + *D_80012284[mode] * 4);
    n = h[0];
    pt = (PT *)(DAT_1f8002d8 + h[1]);
    do {
        p = (FT4 *)DAT_1f800164;
        ((unsigned char *)p)[3] = 9;
        p->code = 0x2d;
        SetSemiTrans(p, 1);
        *(int *)&p->u0 = pt->uv0;
        p->uv1 = pt->uv1;
        p->uv2 = pt->uv2;
        p->uv3 = pt->uv3;
        *(short *)((char *)p + 0x16) = 0x14;
        p->x0 = D_80080404[mode].x + pt->dx;
        p->y0 = D_80080404[mode].y + pt->dy;
        w = pt->w;
        p->x2 = p->x0;
        p->x1 = p->x0 + w;
        p->y1 = p->y0;
        p->y2 = p->y0 + pt->h;
        p->x3 = p->x1;
        p->y3 = p->y2;
        p->clut = GetClut(0x160, 0x1e5);
        pt++;
        AddPrim((void *)(DAT_1f8001e0 + 4), p);
        n--;
        DAT_1f800164 = DAT_1f800164 + 0x28;
    } while (n != 0);
}
