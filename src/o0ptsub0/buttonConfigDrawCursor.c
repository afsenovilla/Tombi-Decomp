// FUNC 800e8fd8 248 O0PTSUB0
// MATCHING 800e8fd8 248
typedef struct {
    unsigned addr: 24;
    unsigned len: 8;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short w, h;
} TILE;
extern TILE D_1F800000;
extern TILE *D_1F800164;
extern unsigned long *DAT_1f8001e0;

void buttonConfigDrawCursor(void)
{
    TILE *p = D_1F800164;
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
    p->addr = DAT_1f8001e0[6];
    D_1F800164++;
    DAT_1f8001e0[6] = (DAT_1f8001e0[6] & 0xFF000000) | ((unsigned long)p & 0xFFFFFF);
}
