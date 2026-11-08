// VERSION SCUS_94236 scus94236 8004E244 424
// FLAGS -O2 -G0
// HEADERS tomba26
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (main/render/sprite.c, func_8004E244); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"

// INCLUDE_ASM("nonmatchings/main/render/sprite", func_8004E244);
typedef struct { s16 tpage; s16 x; s16 y; u8 u; u8 v; s16 w; s16 h; s16 cx; s16 cy; s16 semi; } SprPart;
extern SprPart *D_8007EBEC[];

void func_8004E244_scus94236(GameObject *obj)
{
    SprPart *e = D_8007EBEC[obj->id];
    SPRT *sprt;
    DR_MODE *drMode;
#if REGION == REGION_EU
    short r[4];
#endif

    while (e->tpage != 0x7FFF) {
        sprt = (SPRT *)D_1F800164;
        SetSprt(sprt);
        SetSemiTrans(sprt, e->semi);
        setRGB0(sprt, 0x80, 0x80, 0x80);
        sprt->code |= 1;
        setWH(sprt, e->w, e->h);
        setXY0(sprt, obj->x.p.whole + e->x, obj->y.p.whole + e->y + NTSC_PAL(0, 0x18));
        setUV0(sprt, e->u, e->v);
        sprt->clut = GetClut(e->cx, e->cy);
        AddPrim((u_long *)D_1F8001E0 + ((s8)obj->priority + 0x324), sprt);
        D_1F800164 += sizeof(SPRT);
        drMode = (DR_MODE *)D_1F800164;
        SetDrawMode(drMode, 0, 0, e->tpage, 0);
        AddPrim((u_long *)D_1F8001E0 + ((s8)obj->priority + 0x324), drMode);
        e++;
        D_1F800164 += sizeof(DR_MODE);
    }
#if REGION == REGION_EU
    r[0] = 0;
    r[1] = 0;
    r[2] = 0x140;
    r[3] = 0x10;
    addTilePrim(r, 0, 0, 1);
    r[1] = 0xF0;
    addTilePrim(r, 0, 0, 1);
#endif
}
#endif
