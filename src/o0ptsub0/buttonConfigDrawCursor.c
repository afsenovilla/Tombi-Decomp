// FUNC 800e8fd8 248 O0PTSUB0
// MATCHING 800e8fd8 248
// VERSION SCUS_94236 optsub00 800E82F4 248
// VERSION SLPS_01144 optsub00 800EA534 248
#if defined(VERSION_SCUS_94236)
// USA version of PAL o0ptsub0 800E8FD8 (buttonConfigDrawCursor.c): PAL C of src/o0ptsub0/buttonConfigDrawCursor.c (1.000 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
typedef struct {
    unsigned addr: 24;
    unsigned len: 8;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short w, h;
} TILE;
extern TILE D_1F800000;
extern TILE *NEXT_PRIM;
extern unsigned long *CURRENT_OT;

void buttonConfigDrawCursor(void)
{
    TILE *p = NEXT_PRIM;
    D_1F800000.len = 3;
    D_1F800000.code = 0x60;
    D_1F800000.g0 = 0xFF;
    D_1F800000.code |= 2;
    D_1F800000.x0 = 0x60;
    D_1F800000.y0 = 0x3c;
    D_1F800000.w = 0x38;
    D_1F800000.r0 = 0;
    D_1F800000.b0 = 0;
    D_1F800000.h = 0x18;
    *p = *(TILE *)0x1F800000;
    p->addr = CURRENT_OT[6];
    NEXT_PRIM++;
    CURRENT_OT[6] = (CURRENT_OT[6] & 0xFF000000) | ((unsigned long)p & 0xFFFFFF);
}
#elif defined(VERSION_SLPS_01144)
// JP buttonConfigDrawCursor (PAL o0ptsub0 800e8fd8): cursor at 0x60,0x4c, width 0x30
typedef struct {
    unsigned addr: 24;
    unsigned len: 8;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short w, h;
} TILE;
extern TILE D_1F800000;
extern TILE *NEXT_PRIM;
extern unsigned long *CURRENT_OT;

void buttonConfigDrawCursor(void)
{
    TILE *p = NEXT_PRIM;
    D_1F800000.len = 3;
    D_1F800000.code = 0x60;
    D_1F800000.g0 = 0xFF;
    D_1F800000.code |= 2;
    D_1F800000.x0 = 0x60;
    D_1F800000.y0 = 0x4C;
    D_1F800000.w = 0x30;
    D_1F800000.r0 = 0;
    D_1F800000.b0 = 0;
    D_1F800000.h = 0x18;
    *p = *(TILE *)0x1F800000;
    p->addr = CURRENT_OT[6];
    NEXT_PRIM++;
    CURRENT_OT[6] = (CURRENT_OT[6] & 0xFF000000) | ((unsigned long)p & 0xFFFFFF);
}
#else
typedef struct {
    unsigned addr: 24;
    unsigned len: 8;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short w, h;
} TILE;
extern TILE D_1F800000;
extern TILE *NEXT_PRIM;
extern unsigned long *CURRENT_OT;

void buttonConfigDrawCursor(void)
{
    TILE *p = NEXT_PRIM;
    D_1F800000.len = 3;
    D_1F800000.code = 0x60;
    D_1F800000.g0 = 0xFF;
    D_1F800000.code |= 2;
    D_1F800000.x0 = 0x1C;
    D_1F800000.y0 = 0x4C;
    D_1F800000.w = 0x40;
    D_1F800000.r0 = 0;
    D_1F800000.b0 = 0;
    D_1F800000.h = 0x18;
    *p = *(TILE *)0x1F800000;
    p->addr = CURRENT_OT[6];
    NEXT_PRIM++;
    CURRENT_OT[6] = (CURRENT_OT[6] & 0xFF000000) | ((unsigned long)p & 0xFFFFFF);
}
#endif
