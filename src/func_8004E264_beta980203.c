// VERSION BETA_980203 beta980203 8004E264 1456
// FLAGS -O2 -G0
// HEADERS tomba26
#if defined(VERSION_BETA_980203)
// Beta version of USA func_8004F5A4 (src/func_8004F5A4_scus94236.c), same body under the beta names; portado de psx_tomba (main/render/sprite.c, func_8004F5A4); licencia MIT del proyecto original.
#ifndef SKIP_ASM
#define SKIP_ASM
#endif
#include "common.h"
#include "game.h"
// beta names
extern __typeof__(D_800123E4) D_80011FFC;
extern __typeof__(GetGraphType) func_8005D190_beta980203;
#define D_800123E4 D_80011FFC
#define D_8007ED10 D_8007C140
#define D_8007ED30 D_8007C160
#define D_8007ED50 D_8007C180
#define D_8007ED70 D_8007C1A0
#define GetGraphType func_8005D190_beta980203
// INCLUDE_ASM("nonmatchings/main/render/sprite", func_8004E244);
typedef struct { s16 tpage; s16 x; s16 y; u8 u; u8 v; s16 w; s16 h; s16 cx; s16 cy; s16 semi; } SprPart;
extern SprPart *D_8007EBEC[];
// INCLUDE_ASM("nonmatchings/main/render/sprite", func_8004E468);
typedef struct { s16 x, y; } Pt_4E468;
extern Pt_4E468 D_8007EC5C[];
// INCLUDE_ASM("nonmatchings/main/render/sprite", func_8004E590);
typedef struct { char p[0x24]; u16 **tbl; char q[0x30 - 0x28]; u16 *dot; } O_4E590;
// INCLUDE_ASM("nonmatchings/main/render/sprite", func_8004E714);
typedef struct {
    char pad0[0x18];
    u8 *volatile base;
    char pad1C[4];
    u16 **tbl;
    u16 **digits;
    char pad28[0x14];
    u8 ap[8];
} O_4E714;
// INCLUDE_ASM("nonmatchings/main/render/sprite", func_8004E900);
typedef struct {
    char pad[0x18];
    u8 *base;
    s16 **tbl;
    char pad20[0x52 - 0x20];
    s16 n;
} SPR_4E900;
extern void func_8004F3DC(SPR_4E900 *s, s16 x, s16 y, u16 v);
// INCLUDE_ASM("nonmatchings/main/render/sprite", func_8004EB10);
typedef struct { u8 a, pa, b, pb, c, pc; } Col6_4EB10;
extern Col6_4EB10 D_8007EC80[];
// INCLUDE_ASM("nonmatchings/main/render/sprite", drawMessageGlyphs);
#define SETUV0(p, u, v) do { (p)->u0 = (u); (p)->v0 = (v); } while (0)
typedef struct { s16 clip[4]; s16 ofs[2]; s16 tw[4]; u16 tpage; char rest[0x68 - 0x16]; } DRAWENV_4EFA8;
#undef SETUV0
// INCLUDE_ASM("nonmatchings/main/render/sprite", func_8004F5A4);
typedef struct { s16 x; s16 y; u8 *text; } MenuLine;
typedef struct { s16 x; s16 y; } MenuPos;
extern MenuLine D_8007ED10[];
extern MenuLine D_8007ED30[];
extern MenuLine D_8007ED50[];
extern MenuLine D_8007ED70[];
extern u8 D_8007C0C4[][2];
extern u8 D_8007C0C5[][2];
extern MenuPos D_8007C1C8[];
extern s16 D_8007C1CA[][2];
extern u8 D_8009C10B_Arr[] asm("D_800A2883");
void func_8004E264_beta980203(void)
{
    MenuLine *l;
    s32 kind;
    u8 *c;
    u8 ch;
    u16 x;
    s16 y;
    TILE *tile;
    SPRT *sprt;
    void *r;
    s32 tp;
    s16 *e;
    SprFrag *f;
    POLY_FT4 *poly;
    s16 n;
    s32 k;
    s32 w;
    s32 tpage;

    tile = (TILE *)D_1F800164;
    setWH(tile, 0x140, 0xF0);
    setlen(tile, 3);
    setXY0(tile, 0, 0);
    tile->code = 0x60;
    SetSemiTrans(tile, 1);
    setRGB0(tile, 0x20, 0x20, 0x20);
    AddPrim((u_long *)D_1F8001E0 + 4, tile);
    r = D_1F800164 = D_1F800164 + sizeof(TILE);
    SetDrawMode(r, 0, 0, 0, 0);
    AddPrim((u_long *)D_1F8001E0 + 4, r);
    D_1F800164 = D_1F800164 + sizeof(DR_MODE);
    switch (SPAD->menuCloseMode) {
    case 0:
        l = D_8007ED70;
        kind = 1;
        break;
    case 1:
        l = D_8007ED10;
        kind = 0;
        break;
    case 2:
        l = D_8007ED30;
        kind = 2;
        break;
    case 3:
        kind = 2;
        l = D_8007ED50;
        break;
    }
    for (; l->x != -1; l++) {
        x = l->x;
        y = l->y;
        c = l->text;
        while ((ch = *c) != 0xFF) {
            c++;
            if (ch) {
                sprt = (SPRT *)D_1F800164;
                setlen(sprt, 4);
                sprt->code = 0x65;
                SetSemiTrans(sprt, 0);
                setXY0(sprt, x, y);
                setUV0(sprt, D_8007C0C4[ch][0], D_8007C0C5[ch][0]);
                setWH(sprt, 8, 0x10);
                sprt->clut = GetClut(0x160, 0x1E3);
                AddPrim((u_long *)D_1F8001E0 + 1, sprt);
                D_1F800164 = D_1F800164 + sizeof(SPRT);
            }
            x += 8;
        }
    }
    r = D_1F800164;
    if (GetGraphType() == 1)
        tp = 0x24;
    else if (GetGraphType() == 2)
        tp = 0x24;
    else
        tp = 0x14;
    SetDrawMode(r, 0, 0, tp, 0);
    AddPrim((u_long *)D_1F8001E0 + 1, r);
    tile = (TILE *)(D_1F800164 = D_1F800164 + sizeof(DR_MODE));
    switch (kind) {
    case 0:
        tile->x0 = 0x74;
        k = D_8009C10B_Arr[0];
        setWH(tile, 0x50, 0x10);
        k <<= 4;
        k += 0x48;
        tile->y0 = k;
        break;
    case 1:
        tile->x0 = 0x84;
        k = D_8009C10B_Arr[0];
        setWH(tile, 0x38, 0x10);
        k <<= 4;
        k += 0x48;
        tile->y0 = k;
        break;
    case 2:
        k = D_8009C10B_Arr[0];
        tile->y0 = 0x68;
        setWH(tile, 0x18, 0x10);
        tile->x0 = k * 0x34 + 0x78;
        break;
    }
    setlen(tile, 3);
    tile->code = 0x60;
    SetSemiTrans(tile, 1);
    setRGB0(tile, 0xFF, 0xFF, 0);
    AddPrim((u_long *)D_1F8001E0 + 1, tile);
    D_1F800164 = D_1F800164 + sizeof(TILE);
    e = (s16 *)(D_1F8002D8[0] + *((s16 **)D_800123E4)[kind] * 4);
    n = e[0];
    f = (SprFrag *)(D_1F8002D8[0] + e[1]);
    do {
        poly = (POLY_FT4 *)D_1F800164;
        setlen(poly, 9);
        poly->code = 0x2D;
        SetSemiTrans(poly, 1);
        *(u32 *)&poly->u0 = *(u32 *)f;
        *(u32 *)&poly->u1 = *(u32 *)&f->uv1;
        *(u16 *)&poly->u2 = *(u32 *)&f->uv2;
        *(u16 *)&poly->u3 = *(u32 *)&f->uv3;
        poly->tpage = 0x14;
        setXY0(poly, D_8007C1C8[kind].x + f->dx, D_8007C1CA[kind][0] + f->dy);
        w = f->w;
        poly->x2 = poly->x0;
        poly->x1 = poly->x0 + w;
        poly->y1 = poly->y0;
        poly->y2 = poly->y0 + f->h;
        poly->x3 = poly->x1;
        poly->y3 = poly->y2;
        poly->clut = GetClut(0x160, 0x1E5);
        if (GetGraphType() == 1 || GetGraphType() == 2)
            tpage = 0x24;
        else
            tpage = 0x14;
        poly->tpage = tpage;
        AddPrim((u_long *)D_1F8001E0 + 1, poly);
        f++;
        n--;
        D_1F800164 = D_1F800164 + sizeof(POLY_FT4);
    } while (n != 0);
}
#endif
