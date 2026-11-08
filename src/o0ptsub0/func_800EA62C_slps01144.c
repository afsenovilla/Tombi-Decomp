// VERSION SLPS_01144 optsub00 800EA62C 236
#if defined(VERSION_SLPS_01144)
// JP optionsMenuDrawText (PAL o0ptsub0 800e90d0): the labels are sprites
typedef struct { short x, y, u, v, w, h, cx, cy; } SprDef;
extern SprDef D_800EA968[], D_800EAA38[], D_800EAA78[];
extern unsigned short func_8005AC9C(int tp, int abr, int x, int y); /* GetTPage */
extern void titleDrawSprite(SprDef *d, short tpage, unsigned char pri);

void func_800EA62C(int sound, int stereo)
{
    SprDef *d;
    short tp;
    tp = func_8005AC9C(0, 0, 0x200, 0);
    d = D_800EA968;
    while (d->x != -1) {
        titleDrawSprite(d++, tp, 0);
    }
    titleDrawSprite(&D_800EAA38[sound], tp, 0);
    titleDrawSprite(&D_800EAA78[stereo], tp, 0);
}
#endif
