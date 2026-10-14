// FUNC 8002df70 988 MAIN0
// MATCHING 8002df70 988
#include "spad.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u_long;
typedef struct { short x, y; short w, h; } RECT;
typedef struct { u_long tag; u_long code[3]; u_long p[13]; } DR_LOAD;
typedef struct { unsigned addr : 24; unsigned len : 8; u8 r0, g0, b0, code; } P_TAG;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (u_long)(_addr))
#define getaddr(p) (u_long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
#define setRECT(r, _x, _y, _w, _h) (r)->x = (_x), (r)->y = (_y), (r)->w = (_w), (r)->h = (_h)
extern int D_1F800164;
extern void SetDrawLoad(DR_LOAD *p, RECT *rect);

typedef struct {
    short id;
    u16 mask;
    u8 x, y, b6, b7, b8, b9;
} GlyphSlot;
extern GlyphSlot GLYPH_CACHE[];
extern u16 *WFM3_DATA[];

short loadMessageGlyph(int p, int id, int bank)
{
    RECT rect;
    int i;
    int freeSlot;
    int j;
    short key;
    int n;
    u16 *src;
    u16 *dst;
    u16 w;
    u16 h;
    u16 a;
    u16 b;
    DR_LOAD *load;

    freeSlot = -1;
    key = (bank << 12) | (id & 0xfff);
    for (i = 0; i < 0x3c; i++) {
        if (GLYPH_CACHE[i].id == key) {
            GLYPH_CACHE[i].mask |= 1 << p;
            return i;
        }
        if (GLYPH_CACHE[i].id == -1) {
            freeSlot = i;
        }
    }
    if ((short)freeSlot == -1) {
        return -1;
    }
    GLYPH_CACHE[(short)freeSlot].mask |= 1 << p;
    src = WFM3_DATA[bank];
    dst = (u16 *)(id * 2 + (int)src);
    src = (u16 *)((u8 *)src + dst[0x48]);
    w = *src++;
    h = *src++;
    n = w * h;
    a = *src++;
    b = *src++;
    GLYPH_CACHE[(short)freeSlot].id = key;
    GLYPH_CACHE[(short)freeSlot].b8 = 0;
    GLYPH_CACHE[(short)freeSlot].b6 = a;
    GLYPH_CACHE[(short)freeSlot].b7 = h;
    GLYPH_CACHE[(short)freeSlot].b9 = b;
    if (n < 0x1b) {
        load = (DR_LOAD *)D_1F800164;
        setRECT(&rect, GLYPH_CACHE[(short)freeSlot].x, GLYPH_CACHE[(short)freeSlot].y, w, h);
        SetDrawLoad(load, &rect);
        dst = (u16 *)load->p;
        for (j = 0; j < w * h; j++) {
            *dst++ = *src++;
        }
        addPrim((*(u_long*)&SPAD.i1e0) + 4, load);
        D_1F800164 += sizeof(DR_LOAD);
    } else {
        load = (DR_LOAD *)D_1F800164;
        setRECT(&rect, GLYPH_CACHE[(short)freeSlot].x, GLYPH_CACHE[(short)freeSlot].y, w, h >> 1);
        SetDrawLoad(load, &rect);
        dst = (u16 *)load->p;
        for (j = 0; j < w * (u16)(h >> 1); j++) {
            *dst++ = *src++;
        }
        addPrim((*(u_long*)&SPAD.i1e0) + 4, load);
        D_1F800164 += sizeof(DR_LOAD);
        load = (DR_LOAD *)D_1F800164;
        setRECT(&rect, GLYPH_CACHE[(short)freeSlot].x, GLYPH_CACHE[(short)freeSlot].y + (h >> 1), w, h >> 1);
        SetDrawLoad(load, &rect);
        dst = (u16 *)load->p;
        for (j = 0; j < w * (u16)(h >> 1); j++) {
            *dst++ = *src++;
        }
        addPrim((*(u_long*)&SPAD.i1e0) + 4, load);
        D_1F800164 += sizeof(DR_LOAD);
    }
    return freeSlot;
}
