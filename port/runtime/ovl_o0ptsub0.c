/* O0PTSUB0 (title / options overlay): functions the decomp still has as asm only, translated by hand for the port
 * from build/full/asm/o0ptsub0/nonmatchings/o0ptsub0/<name>.s.  gen_port.py adds every `<PROG>_<name>(` defined
 * in port/runtime/ovl_*.c to the overlay dispatch table (port_ovlmap) of that overlay. */
#include "port_runtime.h"

typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;

extern int MulNegSin(int, int);
extern int MulCosDup(int, int);
extern int GetGraphType(void);
extern u8 D_800EA1D8[];          /* font table: 5 bytes per glyph (u, v, w, h, -) */

#define PRIM_PTR (*(u8 **)0x1F800164u)
#define OT_PTR   (*(u32 **)0x1F8001E0u)

/* func_800E9F5C (0x27C bytes): one big glyph of the rotating title menu as a POLY_FT4.
 * c = glyph index, ang = angle / column, off = -1 for the front row (or an angle offset), pal = CLUT row. */
void O0PTSUB0_func_800E9F5C(int c, int ang, int off, unsigned char pal) {
    const u8 *font = D_800EA1D8;
    int x0, x1, s1 = (ang + off) & 0xFF;
    if ((unsigned)(s1 - 0x41) < 0x7F) return;            /* facing away: not drawn */
    if ((short)off == -1) {
        int v = MulNegSin(0xE0, 0x5A);
        int w = font[(short)c * 5 + 2];
        x0 = w * (short)ang - v;
        x1 = w + x0;
    } else {
        x0 = -MulNegSin(ang & 0xFF, 0x5A);
        x1 = -MulNegSin(s1, 0x5A);
    }
    MulCosDup(ang & 0xFF, 0xFF);
    u8 *p = PRIM_PTR;
    const u8 *e = font + (short)c * 5;
    p[3] = 9;
    p[7] = 0x2C;
    p[4] = p[5] = p[6] = 0x80;
    *(u16 *)(p + 0x08) = x0 + 0xA0;
    *(u16 *)(p + 0x10) = x1 + 0xA0;
    *(u16 *)(p + 0x0A) = 0xB8;
    *(u16 *)(p + 0x12) = 0xB8;
    p[7] &= 0xFD;
    *(u16 *)(p + 0x18) = *(u16 *)(p + 0x08);
    *(u16 *)(p + 0x1A) = e[3] + 0xB8;
    *(u16 *)(p + 0x20) = *(u16 *)(p + 0x10);
    *(u16 *)(p + 0x22) = *(u16 *)(p + 0x1A);
    p[0x0C] = e[0];
    p[0x0D] = e[1];
    p[0x1C] = p[0x0C];
    p[0x15] = p[0x0D];
    p[0x14] = p[0x0C] + e[2];
    p[0x1D] = p[0x0D] + e[3];
    *(u16 *)(p + 0x0E) = (((pal & 0xFF) + 0x1E0) << 6) | 0x15;
    p[0x24] = p[0x14];
    p[0x25] = p[0x1D];
    {
        int t = GetGraphType();
        *(u16 *)(p + 0x16) = (t == 1 || t == 2) ? 0x86 : 0x26;
    }
    u32 *ot = OT_PTR;
    *(u32 *)p = (*(u32 *)p & 0xFF000000u) | (*ot & 0xFFFFFFu);
    *ot = (*ot & 0xFF000000u) | ((u32)p & 0xFFFFFFu);
    PRIM_PTR = p + 0x28;
}
