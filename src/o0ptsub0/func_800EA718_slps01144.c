// VERSION SLPS_01144 optsub00 800EA718 220
#if defined(VERSION_SLPS_01144)
// JP buttonConfigDrawText (PAL o0ptsub0 800e9254): sprite lists
typedef struct { short x, y, u, v, w, h, cx, cy; } SprDef;
extern SprDef *D_800EAED8[];
extern SprDef D_800EAAA8[];
extern unsigned short func_8005AC9C(int tp, int abr, int x, int y); /* GetTPage */
extern void titleDrawSprite(SprDef *d, short tpage, unsigned char pri);

void func_800EA718(int cfg)
{
    SprDef *d;
    short tp;
    tp = func_8005AC9C(0, 0, 0x200, 0);
    d = D_800EAED8[cfg];
    while (d->x != -1) {
        titleDrawSprite(d++, tp, 0);
    }
    d = D_800EAAA8;
    while (d->x != -1) {
        titleDrawSprite(d++, tp, 0);
    }
}
#endif
