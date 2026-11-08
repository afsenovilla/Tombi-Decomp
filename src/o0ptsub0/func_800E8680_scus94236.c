// VERSION SCUS_94236 optsub00 800E8680 404
#if defined(VERSION_SCUS_94236)
// USA version of PAL o0ptsub0 800e950c (screenAdjustDrawText.c)
typedef struct { short x, y, u, v, w, h, cx, cy; } SprDef;
extern SprDef SCREEN_ADJUST_SPRITES[];
extern short SCREEN_OFFSET_X, SCREEN_OFFSET_Y;
extern unsigned short GetTPage(int tp, int abr, int x, int y);
extern void titleDrawSprite(SprDef *d, short tpage, unsigned char pri);
extern void optionsDrawLabel(short x, short y, int i);
extern void optionsDrawLabelPal2(short x, short y, int i);
extern void optionsDrawLabelPal(short x, short y, int i, int c);
extern void screenAdjustDrawNumber(short x, short y, int n);

void screenAdjustDrawText(int dir)
{
    SprDef *d;
    short tp;
    tp = GetTPage(0, 0, 0x200, 0);
    d = SCREEN_ADJUST_SPRITES;
    while (d->x != -1) {
        titleDrawSprite(d++, tp, 0);
    }
    optionsDrawLabel(0x68, 0x10, 4);
    optionsDrawLabel(0xA0, 0x50, 0xD);
    optionsDrawLabel(0xA0, 0x70, 0xE);
    optionsDrawLabelPal(0x20, 0x60, 0x1D, dir == 3 ? 3 : 0);
    optionsDrawLabelPal(0x60, 0x60, 0x1E, dir == 4 ? 3 : 0);
    optionsDrawLabelPal(0x40, 0x40, 0x1F, dir == 1 ? 3 : 0);
    optionsDrawLabelPal(0x40, 0x80, 0x20, dir == 2 ? 3 : 0);
    screenAdjustDrawNumber(0x100, 0x50, SCREEN_OFFSET_X);
    screenAdjustDrawNumber(0x100, 0x70, SCREEN_OFFSET_Y - 8);
    optionsDrawLabelPal2(0x60, 0xB0, 0x16);
    optionsDrawLabelPal2(0x60, 0xC0, 0x15);
}
#endif
