// VERSION SLPM_80165 slpm80165 80017FF8 248
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only: text window setup with every field as an argument (the fixed-value form is func_80017E6C): glyph
// source, sizes, CLUT position, and the two draw-mode packets with the texture page of (tx, ty) worked out inline
// (the high-resolution form when func_8005453C_slpm80165 is 1 or 2).
typedef struct {
    short w0; short pad2; void *src; char p8[0x20 - 8]; short x20, y20; char p24[0x2a - 0x24]; short w2a, w2c, w2e, w30;
} TB;
extern int func_8005453C_slpm80165(void);
extern void SetDrawMode(void *, int, int, int, void *);

void func_80017FF8(TB *o, int a, void *src, int b, int c, int d, int e, int x20, int y20, int tx, int ty)
{
    o->w0 = a;
    o->src = src;
    o->w2a = b;
    o->w2c = c;
    o->w2e = d;
    o->w30 = e;
    o->x20 = x20;
    o->y20 = y20;
    if (func_8005453C_slpm80165() == 1 || func_8005453C_slpm80165() == 2)
        tx = ((ty & 0x300) >> 3) | ((tx & 0x3ff) >> 6);
    else
        tx = (((ty & 0x100) >> 4) | ((tx & 0x3ff) >> 6)) | ((ty & 0x200) << 2);
    SetDrawMode((char *)o + 8, 0, 0, tx, 0);
    SetDrawMode((char *)o + 0x14, 0, 0, tx, 0);
}
#endif
