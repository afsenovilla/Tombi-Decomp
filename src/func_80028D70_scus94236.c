// VERSION SCUS_94236 scus94236 80028D70 388
// FLAGS -O2 -G0
// HEADERS tomba26
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (main/render/ui.c, func_80028D70); licencia MIT del proyecto original.
#ifndef SKIP_ASM
#define SKIP_ASM
#endif
#include "common.h"
#include "game.h"
#include "game/gte.h"
// INCLUDE_ASM("nonmatchings/main/render/ui", func_80024CFC);

typedef struct {
    u16 clut, tpage;
    unsigned long c0;
    u16 p8[3];
    u16 uv0;
    u16 p10[3];
    u16 uv1;
    u16 p18[3];
    u16 uv2;
} Face_24CFC;
// INCLUDE_ASM("nonmatchings/main/render/ui", func_80024EEC);

typedef struct {
    u16 clut, tpage;
    unsigned long c0;
    u16 p08[3];
    u16 uv0;
    u16 p10[3];
    u16 uv1;
    u16 p18[3];
    u16 uv2;
    u16 p20[3];
    u16 uv3;
} Face_24EEC;
// INCLUDE_ASM("nonmatchings/main/render/ui", func_800251C0);

typedef struct {
    u16 clut, tpage;
    unsigned long c0, c1, c2;
    u16 p10[3];
    u16 uv0;
    u16 p18[3];
    u16 uv1;
    u16 p20[3];
    u16 uv2;
} Face_251C0;
// INCLUDE_ASM("nonmatchings/main/render/ui", func_800253C8);

typedef struct {
    u16 clut, tpage;
    unsigned long c0, c1, c2;
    unsigned long c3;
    u16 p14[3];
    u16 uv0;
    u16 p1c[3];
    u16 uv1;
    u16 p24[3];
    u16 uv2;
    u16 p2c[3];
    u16 uv3;
} Face_253C8;
// INCLUDE_ASM("nonmatchings/main/render/ui", func_800263F0);



typedef struct { char pad[0xe]; u8 b0e; } O;
typedef struct { long c[4]; } C4_263F0;
extern C4_263F0 D_8007C160[];
// INCLUDE_ASM("nonmatchings/main/render/ui", func_80026694);
typedef struct {
    unsigned long tag;
    unsigned long c0;
    u16 x0, y0;
    u16 x1, y1;
    u16 x2, y2;
} PolyF3_26694;
typedef struct {
    unsigned long tag;
    unsigned long c0;
    u16 x0, y0;
    unsigned long w0c;
    u16 x1, y1;
    unsigned long w14;
    u16 x2, y2;
} PolyFT3_26694;
// INCLUDE_ASM("nonmatchings/main/render/ui", func_80026E48);
typedef struct {
    unsigned long tag;
    unsigned long c0;
    u16 x0, y0;
    u16 x1, y1;
    u16 x2, y2;
} PolyF3_26E48;
typedef struct {
    unsigned long tag;
    unsigned long c0;
    u16 x0, y0;
    unsigned long c1;
    u16 x1, y1;
    unsigned long c2;
    u16 x2, y2;
} PolyG3_26E48;
extern char *volatile D_1F80008C_CharPtrVolatile asm("MODEL_VERTEX_PTR");
// INCLUDE_ASM("nonmatchings/main/render/ui", func_80027600);
typedef struct {
    unsigned long tag;
    unsigned long c0;
    u16 x0, y0;
    u16 x1, y1;
    u16 x2, y2;
} PolyF3_27600;
typedef struct {
    unsigned long tag;
    unsigned long c0;
    u16 x0, y0;
    unsigned long w0c;
    u16 x1, y1;
    unsigned long w14;
    u16 x2, y2;
} PolyFT3_27600;
// INCLUDE_ASM("nonmatchings/main/render/ui", func_800288C4);
typedef struct { char p0[9]; u8 b9; } G_288C4;
#define AXIS(v, c, a, hi, lo) ({                                  \
    s32 d;                                                      \
    s32 r;                                                      \
    d = a - 0x50;                                               \
    if ((u32)d < 0x60) r = 0;                              \
    else {                                                      \
        d = a - 0x30;                                           \
        if ((u16)d < 0xa0) { (v) = 1; { s32 t = ((u16)d < 0x50) ? hi : lo; r = t; } } \
        else {                                                  \
            d = a - 0x10;                                       \
            (a) = 0;                                            \
            if ((u16)d < 0xe0) { (v) = 2; { s32 t = ((u16)d < 0x70) ? hi : lo; r = t; } } \
            else { (v) = 3; { s32 t = (c < 0x80) ? hi : lo; r = t; } }         \
        }                                                       \
    }                                                           \
    r; })
#undef AXIS
// INCLUDE_ASM("nonmatchings/main/render/ui", func_80028A74);
typedef struct { u8 b[6]; } PadAlign;
extern const PadAlign D_80010368;
// INCLUDE_ASM("nonmatchings/main/render/ui", func_80028CE4);
extern s32 PadSetAct(s32, u8 *, s32);
// INCLUDE_ASM("nonmatchings/main/render/ui", func_80028D70);
typedef struct { u16 kind; char p2[6]; u8 x; u8 y; char pA[2]; } PadState;
s16 func_80028D70_scus94236(port)
s16 port;
{
    PadState *g = (PadState *)D_8009C978 + port;
    s32 kind;
    s32 btn;
    s32 y;
    s32 a, b;

    switch (port) {
    case 0:
        if (D_8009EB58 != 0) return 0;
        PadInfoMode(0, 2, 0);
        btn = ~D_8009EB5A;
        y = (D_8009EB59 >> 4) & 7;
        g->kind = kind = y;
        break;
    case 1:
        if (D_8009EB7A != 0) return 0;
        PadInfoMode(1, 2, 0);
        btn = ~D_8009EB7C;
        y = (D_8009EB7B >> 4) & 7;
        g->kind = kind = y;
        break;
    }
    switch ((u16)kind) {
    case 4:
        g->x = 0x80;
        g->y = 0x80;
        break;
    case 7:
        if (port != 0) break;
        g->x = D_8009EB5E;
        g->y = kind = D_8009EB5F;
        if (!(btn & 0xF0)) {
            btn &= 0xFF0F;
            a = (g->x == 0) << 7;
            if (g->x == 0xFF) a = 0x20;
            b = ((u8)kind == 0) << 4;
            a |= btn;
            y = 0x40;
            if ((u8)kind == 0xFF) b = y;
            btn = b | a;
        }
        break;
    }
    return btn;
}
#endif
