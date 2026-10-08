// FUNC 80056420 2604 MAIN0
/* score 342: structure/frame match; remaining: register allocation (o in s0 vs s1 with the loop giv swapped; case temps y/w in a0/t0 vs v1/a2, k in a2 vs t0). short k gives 287 but duplicates the range check. Tried decl orders, types, block scopes, condition forms. */
typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0; unsigned short clut;
    short w, h;
} SPRT;
typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0; unsigned short clut;
    short x1, y1;
    unsigned char u1, v1; unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2; unsigned short pad1;
    short x3, y3;
    unsigned char u3, v3; unsigned short pad2;
} POLY_FT4;
typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    short x0, y0, w, h;
} TILE;
typedef struct { char pad[8]; short x, y, w, h, f; } R;
typedef struct { short u, v; unsigned char w, pw; short h; } UVT;
typedef struct { unsigned char u, v; } UV2;
typedef struct { unsigned char u0, v0, u1, v1, u2, v2, u3, v3; } UV8;

extern char *DAT_1f800164;
extern int DAT_1f8001e0;
extern UVT D_80080218[];
extern UV2 D_800801f0[];
extern UV8 D_800801f8[];
extern void SetSprt(SPRT *);
extern unsigned short GetClut(int, int);
extern void AddPrim(void *, void *);
extern void SetDrawMode(void *, int, int, int, void *);

#define setlen(p, n) (((unsigned char *)(p))[3] = (n))

void FUN_80056420(R *o, short c, short tp)
{
    SPRT *sp = (SPRT *)0x1f800000;
    POLY_FT4 *ft = (POLY_FT4 *)0x1f800014;
    TILE *ti = (TILE *)0x1f80003c;
    char pad[96];
    short pos[4][8];
    int k;
    short x;
    short y;
    short w;
    short h;
    int i;
    void *p;

    if (o->f != -1 && (unsigned)(k = o->f >> 12) < 8) {
        setlen(ft, 9);
        ft->code = 0x2c;
        ft->r0 = 0x80;
        ft->g0 = 0x80;
        ft->b0 = 0x80;
        switch (k) {
        case 0:
            w = 8; h = 16;
            x = o->x + o->w / 2 - 4;
            y = o->y + o->h - 6;
            break;
        case 1:
            w = 16; h = 16;
            x = o->x + 8;
            y = o->y + o->h - 8;
            break;
        case 2:
            w = 16; h = 8;
            x = o->x - 9;
            y = o->y + o->h / 2 - 6;
            break;
        case 3:
            w = 16; h = 16;
            x = o->x + 8;
            y = o->y - 9;
            break;
        case 4:
            w = 8; h = 16;
            x = o->x + o->w / 2 - 4;
            y = o->y - 10;
            break;
        case 5:
            w = 16; h = 16;
            x = o->x + o->w - 24;
            y = o->y - 9;
            break;
        case 6:
            w = 16; h = 8;
            x = o->x + o->w - 4;
            y = o->y + o->h / 2 - 6;
            break;
        case 7:
            w = 16; h = 16;
            x = o->x + o->w - 24;
            y = o->y + o->h - 8;
            break;
        default:
            goto skip;
        }
        ft->x3 = ft->x1 = x + w;
        ft->y1 = ft->y0 = y;
        ft->x2 = ft->x0 = x;
        ft->y3 = ft->y2 = y + h;
        ft->u0 = D_80080218[k].u;
        ft->v0 = D_80080218[k].v;
        h = D_80080218[k].h;
        w = D_80080218[k].w;
        ft->v1 = ft->v0;
        ft->u1 = ft->u0 + w;
        ft->u2 = ft->u0;
        ft->v2 = ft->v3 = h + ft->v0;
        ft->u3 = ft->u1;
        ft->clut = GetClut(0x160, c + 0x1e3);
        ft->tpage = tp;
        { void *p = DAT_1f800164;
        *(POLY_FT4 *)p = *ft;
        AddPrim((void *)(DAT_1f8001e0 + 8), p); }
        DAT_1f800164 += 0x28;
    }
skip:
    pos[0][0] = o->x;
    pos[0][1] = o->y;
    pos[1][0] = o->x + o->w - 8;
    pos[1][1] = o->y;
    pos[2][0] = o->x;
    pos[2][1] = o->y + o->h - 8;
    pos[3][0] = o->x + o->w - 8;
    pos[3][1] = o->y + o->h - 8;
    for (i = 0; i < 4; i++) {
        SetSprt(sp);
        sp->r0 = 0x80;
        sp->g0 = 0x80;
        sp->b0 = 0x80;
        sp->x0 = pos[i][0];
        sp->y0 = pos[i][1];
        sp->u0 = D_800801f0[i].u;
        sp->v0 = D_800801f0[i].v;
        sp->w = 8;
        sp->h = 8;
        sp->clut = GetClut(0x160, c + 0x1e3);
        { void *p = DAT_1f800164;
        *(SPRT *)p = *sp;
        AddPrim((void *)(DAT_1f8001e0 + 8), p); }
        DAT_1f800164 += 0x14;
    }
    pos[0][0] = o->x + 8;
    pos[0][1] = o->y;
    pos[0][2] = o->x + o->w - 8;
    pos[0][3] = o->y;
    pos[0][4] = o->x + 8;
    pos[0][5] = o->y + 8;
    pos[0][6] = o->x + o->w - 8;
    pos[0][7] = o->y + 8;
    pos[1][0] = o->x + 8;
    pos[1][1] = o->y + o->h - 8;
    pos[1][2] = o->x + o->w - 8;
    pos[1][3] = o->y + o->h - 8;
    pos[1][4] = o->x + 8;
    pos[1][5] = o->y + o->h;
    pos[1][6] = o->x + o->w - 8;
    pos[1][7] = o->y + o->h;
    pos[2][0] = o->x;
    pos[2][1] = o->y + 8;
    pos[2][2] = o->x + 8;
    pos[2][3] = o->y + 8;
    pos[2][4] = o->x;
    pos[2][5] = o->y + o->h - 8;
    pos[2][6] = o->x + 8;
    pos[2][7] = o->y + o->h - 8;
    pos[3][0] = o->x + o->w - 8;
    pos[3][1] = o->y + 8;
    pos[3][2] = o->x + o->w;
    pos[3][3] = o->y + 8;
    pos[3][4] = o->x + o->w - 8;
    pos[3][5] = o->y + o->h - 8;
    pos[3][6] = o->x + o->w;
    pos[3][7] = o->y + o->h - 8;
    for (i = 0; i < 4; i++) {
        setlen(ft, 9);
        ft->code = 0x2c;
        ft->r0 = 0x80;
        ft->g0 = 0x80;
        ft->b0 = 0x80;
        ft->x0 = pos[i][0];
        ft->y0 = pos[i][1];
        ft->x1 = pos[i][2];
        ft->y1 = pos[i][3];
        ft->x2 = pos[i][4];
        ft->y2 = pos[i][5];
        ft->x3 = pos[i][6];
        ft->y3 = pos[i][7];
        ft->u0 = D_800801f8[i].u0;
        ft->v0 = D_800801f8[i].v0;
        ft->u1 = D_800801f8[i].u1;
        ft->v1 = D_800801f8[i].v1;
        ft->u2 = D_800801f8[i].u2;
        ft->v2 = D_800801f8[i].v2;
        ft->u3 = D_800801f8[i].u3;
        ft->v3 = D_800801f8[i].v3;
        ft->clut = GetClut(0x160, c + 0x1e3);
        ft->tpage = tp;
        { void *p = DAT_1f800164;
        *(POLY_FT4 *)p = *ft;
        AddPrim((void *)(DAT_1f8001e0 + 8), p); }
        DAT_1f800164 += 0x28;
    }
    setlen(ti, 3);
    ti->code = 0x60;
    if (c) {
        ti->r0 = 0;
        ti->g0 = 0x50;
        ti->b0 = 0;
    } else {
        ti->r0 = 0xf8;
        ti->g0 = 0xe8;
        ti->b0 = 0xd8;
    }
    ti->x0 = o->x + 8;
    ti->y0 = o->y + 8;
    ti->w = o->w - 16;
    ti->h = o->h - 16;
    { void *p = DAT_1f800164;
    *(TILE *)p = *ti;
    AddPrim((void *)(DAT_1f8001e0 + 8), p); }
    p = DAT_1f800164 = DAT_1f800164 + 0x10;
    SetDrawMode(p, 0, 0, tp, 0);
    AddPrim((void *)(DAT_1f8001e0 + 8), p);
    DAT_1f800164 += 0xc;
}
