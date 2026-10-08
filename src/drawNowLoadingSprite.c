// FUNC 80021478 588 MAIN0
// MATCHING 80021478 588
// Portado de psx_tomba (drawutil.c, drawNowLoadingSprite); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"
extern u_char D_800A1FF8[];
extern u_char D_800A2008[];
typedef struct {
    u_short u;
    u_short v;
    u_short w;
    u_short h;
    short clutX;
    short clutY;
} UiSpriteDef;

extern UiSpriteDef D_8007B30C[];

void drawUiSprite(short x, short y, short sprt_id);
void drawNowLoadingSprite(int x, int y, short sprt_id, short tpage, short arg4);




//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/render/drawutil", drawUiSprite);

void drawNowLoadingSprite(int x, int y, short sprt_id, short tpage, short arg4)
{
    int pad;
    int w;
    int h;
    POLY_FT4* poly;

    if (sprt_id == 0) {
        int scale = D_8007D988[arg4];
        pad = (u_int)scale >> 11;
    } else {
        int scale = D_8007D988[arg4];
        pad = ((scale << 6) >> 16) + 4;
    }

    poly = D_8009C8A8;
    setlen(poly, 9);
    setcode(poly, 0x2C);
    setRGB0(poly, 128, 128, 128);
    poly->code = (u_char) (poly->code & 0xFD);
    poly->x0 = x - pad;
    poly->y0 = y - pad;
    w = D_8007B30C[sprt_id].w;
    poly->x1 = pad + (x + w);
    poly->y1 = y - pad;
    poly->x2 = x - pad;
    h = D_8007B30C[sprt_id].h;
    poly->y2 = pad + (y + h);
    w = D_8007B30C[sprt_id].w;
    poly->x3 = pad + (x + w);
    h = D_8007B30C[sprt_id].h;
    poly->y3 = pad + (y + h);
    poly->u0 = D_8007B30C[sprt_id].u;
    poly->v0 = D_8007B30C[sprt_id].v;
    poly->u1 = poly->u0 + D_8007B30C[sprt_id].w - 1;
    poly->v1 = poly->v0;
    poly->u2 = poly->u0;
    poly->v2 = poly->v0 + D_8007B30C[sprt_id].h - 1;
    poly->u3 = poly->u1;
    poly->v3 = poly->v2;
    setClut(poly, D_8007B30C[sprt_id].clutX, D_8007B30C[sprt_id].clutY);
    poly->tpage = tpage;
    addPrim(CURRENT_OT, poly);
    D_8009C8A8 += sizeof(POLY_FT4);
}
